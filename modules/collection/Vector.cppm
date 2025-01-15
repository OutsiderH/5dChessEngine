module;

#include <initializer_list>
#include <stdexcept>

export module Engine.Collection.Vector;

import Engine.Type.Number;
import Engine.Type.Concept;
import Engine.Type.Util;

using namespace Engine::Type;

namespace Engine::Collection {
    // todo:
    // use standard collection interface at AppendRange()
    // move version for some methods
    export template <typename ElementType, u64 align = sizeof(ElementType)>
    requires (
        !IsReference<ElementType>::Result
        && CopyConstructible<ElementType>
        && DefaultConstructible<ElementType>
        && Destructible<ElementType>
        && align >= sizeof(ElementType)
    )
        class Vector {
        private:
            static constexpr bool MC = MoveConstructible<ElementType>;
            static constexpr bool NTD = NonThrowDestructible<ElementType>;
            byte* data = nullptr;
            u64 capacity = 0;
            u64 size = 0;
            inline ElementType* IndexToPointer(u64 index) noexcept {
                return reinterpret_cast<ElementType*>(data + index * align);
            }
            inline const ElementType* IndexToPointer(u64 index) const noexcept {
                return reinterpret_cast<const ElementType*>(data + index * align);
            }
        public:
            inline Vector() noexcept {}
            inline Vector(const Vector& other) :
                data(new byte[other.capacity * align]),
                capacity(other.capacity),
                size(other.size) {
                for (u64 i = 0; i < size; ++i) {
                    new (IndexToPointer(i)) ElementType(other[i]);
                }
            }
            inline Vector(Vector&& other) noexcept :
                data(other.data),
                capacity(other.capacity),
                size(other.size) {
                other.data = nullptr;
            }
            inline Vector(std::initializer_list<ElementType> list) :
                capacity(list.size()) {
                data = new byte[capacity * align];
                size = capacity;
                for (u64 i = 0; i < size; ++i) {
                    if constexpr (MC) {
                        new (IndexToPointer(i)) ElementType(Move(list.begin()[i]));
                    }
                    else {
                        new (IndexToPointer(i)) ElementType(list.begin()[i]);
                    }
                }
            }
            inline Vector(u64 size) :
                data(new byte[size * align]),
                capacity(size),
                size(size) {
                for (u64 i = 0; i < size; ++i) {
                    new (IndexToPointer(i)) ElementType();
                }
            }
            inline Vector(u64 size, const ElementType& fill) :
                data(new byte[size * align]),
                capacity(size),
                size(size) {
                for (u64 i = 0; i < size; ++i) {
                    new (IndexToPointer(i)) ElementType(fill);
                }
            }
            inline ~Vector() noexcept(NTD) {
                for (u64 i = 0; i < size; ++i) {
                    IndexToPointer(i)->~ElementType();
                }
                delete[] data;
            }
            inline byte* GetData() noexcept {
                return data;
            }
            inline const byte* GetData() const noexcept {
                return data;
            }
            inline u64 GetCapacity() const noexcept {
                return capacity;
            }
            inline u64 GetSize() const noexcept {
                return size;
            }
            inline bool Empty() const noexcept {
                return size == 0;
            }
            inline u64 Find(const ElementType& value, u64 from) const {
                for (u64 i = from; i < size; ++i) {
                    if (operator[](i) == value) {
                        return i;
                    }
                }
                throw std::logic_error("Value not found");
            }
            inline bool TryFind(const ElementType& value, u64 from, u64& out_index) const noexcept {
                for (u64 i = from; i < size; ++i) {
                    if (operator[](i) == value) {
                        out_index = i;
                        return true;
                    }
                }
                return false;
            }
            inline void Append(const ElementType& value) {
                if (size == capacity) {
                    Reserve(capacity * 2 + 1);
                }
                new (IndexToPointer(size++)) ElementType(value);
            }
            inline void AppendRange(const Vector& values) {
                if (size + values.size > capacity) {
                    Reserve(capacity * 2);
                }
                for (u64 i = 0; i < values.size; ++i) {
                    new (IndexToPointer(size++)) ElementType(values[i]);
                }
            }
            inline void Insert(u64 index, const ElementType& value) {
                if (size == capacity) {
                    Reserve(capacity * 2 + 1);
                }
                for (u64 i = size; i > index; --i) {
                    new (IndexToPointer(i)) ElementType(operator[](i - 1));
                }
                new (IndexToPointer(index)) ElementType(value);
                ++size;
            }
            inline void InsertRange(u64 index, const Vector& values) {
                if (size + values.size > capacity) {
                    Reserve(capacity * 2);
                }
                for (u64 i = size; i > index + values.size - 1; --i) {
                    new (IndexToPointer(i)) ElementType(operator[](i - 1));
                }
                for (u64 i = 0; i < values.size; ++i) {
                    new (IndexToPointer(index + i)) ElementType(values[i]);
                }
                size += values.size;
            }
            inline void Reserve(u64 newCapacity) {
            if (newCapacity < capacity) {
                throw std::invalid_argument("Must reserve larger capacity than current capacity");
            }
            byte* newData = new byte[newCapacity * align];
            for (u64 i = 0; i < size; ++i) {
                ElementType* pointer = IndexToPointer(i);
                if constexpr (MC) {
                    new (newData + i * align) ElementType(Move(*pointer));
                }
                else {
                    new (newData + i * align) ElementType(*pointer);
                }
                pointer->~ElementType();
            }
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }
            inline void Resize(u64 newSize) {
            if (newSize > capacity) {
                Reserve(newSize);
            }
            for (u64 i = size; i < newSize; ++i) {
                new (IndexToPointer(i)) ElementType();
            }
            size = newSize;
        }
            inline ElementType& IndexBoundCheck(u64 index) {
            if (index >= size) {
                throw std::out_of_range("Index out of range");
            }
            return operator[](index);
        }
            inline const ElementType& IndexBoundCheck(u64 index) const {
            if (index >= size) {
                throw std::out_of_range("Index out of range");
            }
            return operator[](index);
        }
            inline Vector& operator=(const Vector& other) {
            if (this != &other) {
                for (u64 i = 0; i < size; ++i) {
                    IndexToPointer(i)->~ElementType();
                }
                delete[] data;
                data = new byte[other.capacity * align];
                capacity = other.capacity;
                size = other.size;
                for (u64 i = 0; i < size; ++i) {
                    new (IndexToPointer(i)) ElementType(other[i]);
                }
            }
            return *this;
        }
            inline Vector& operator=(Vector&& other) {
            if (this != &other) {
                for (u64 i = 0; i < size; ++i) {
                    IndexToPointer(i)->~ElementType();
                }
                delete[] data;
                data = other.data;
                capacity = other.capacity;
                size = other.size;
                other.data = nullptr;
            }
            return *this;
        }
            inline ElementType& operator[](u64 index) {
            return *IndexToPointer(index);
        }
            inline const ElementType& operator[](u64 index) const {
            return *IndexToPointer(index);
        }
    };
}