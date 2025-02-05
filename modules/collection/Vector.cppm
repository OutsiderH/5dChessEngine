export module Engine.Collection.Vector;

import Engine.Type.Number;
import Engine.Type.Concept;
import Engine.Type.Util;
import <initializer_list>;
import <stdexcept>;

using namespace Engine::Type;

namespace Engine::Collection {
    // todo:
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
        public:
            using _Element = ElementType;
            using _ElementRef = ElementType&;
            using _ElementRightRef = ElementType&&;
            using _ConstElementRef = const ElementType&;
        private:
            static constexpr bool MC = MoveConstructible<ElementType>;
            static constexpr bool NTD = NonThrowDestructible<ElementType>;
            static constexpr bool NTEC = NonThrowEqualityComparable<ElementType>;
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
                        new (IndexToPointer(i)) ElementType(TypeMove(list.begin()[i]));
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
            inline ElementType& First() noexcept {
                return operator[](0);
            }
            inline const ElementType& First() const noexcept {
                return operator[](0);
            }
            inline ElementType& Last() noexcept {
                return operator[](size - 1);
            }
            inline const ElementType& Last() const noexcept {
                return operator[](size - 1);
            }
            inline bool Contains(const ElementType& value, u64 from = 0) const noexcept(NTEC) {
                for (u64 i = from; i < size; ++i) {
                    if (operator[](i) == value) {
                        return true;
                    }
                }
                return false;
            }
            inline u64 Find(const ElementType& value, u64 from = 0) const {
                for (u64 i = from; i < size; ++i) {
                    if (operator[](i) == value) {
                        return i;
                    }
                }
                throw std::logic_error("Value not found");
            }
            inline bool TryFind(const ElementType& value, u64 from, u64& out_index) const noexcept(NTEC) {
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
            inline void Append(ElementType&& value) {
                if (size == capacity) {
                    Reserve(capacity * 2 + 1);
                }
                if constexpr (MC) {
                    new (IndexToPointer(size++)) ElementType(TypeMove(value));
                }
                else {
                    new (IndexToPointer(size++)) ElementType(value);
                }
            }
            template <IList CollectionType>
            inline void AppendRange(const CollectionType& values) {
                if (size + values.GetSize() > capacity) {
                    u64 newCapacity = capacity * 2 + 1;
                    while (newCapacity < size + values.GetSize()) {
                        newCapacity = newCapacity * 2 + 1;
                    }
                    Reserve(newCapacity);
                }
                for (u64 i = 0; i < values.GetSize(); ++i) {
                    new (IndexToPointer(size++)) ElementType(values[i]);
                }
            }
            inline void Prepend(const ElementType& value) {
                if (size == capacity) {
                    Reserve(capacity * 2 + 1);
                }
                for (u64 i = size; i > 0; --i) {
                    if constexpr (MC) {
                        new (IndexToPointer(i)) ElementType(TypeMove(operator[](i - 1)));
                    }
                    else {
                        new (IndexToPointer(i)) ElementType(operator[](i - 1));
                    }
                }
                new (IndexToPointer(0)) ElementType(value);
                ++size;
            }
            template <IList CollectionType>
            inline void PrependRange(const CollectionType& values) {
                if (size + values.GetSize() > capacity) {
                    u64 newCapacity = capacity * 2 + 1;
                    while (newCapacity < size + values.GetSize()) {
                        newCapacity = newCapacity * 2 + 1;
                    }
                    Reserve(newCapacity);
                }
                for (u64 i = size; i > 0; --i) {
                    if constexpr (MC) {
                        new (IndexToPointer(i + values.GetSize() - 1)) ElementType(TypeMove(operator[](i - 1)));
                    }
                    else {
                        new (IndexToPointer(i + values.GetSize() - 1)) ElementType(operator[](i - 1));
                    }
                }
                for (u64 i = 0; i < values.GetSize(); ++i) {
                    new (IndexToPointer(i)) ElementType(values[i]);
                }
                size += values.size;
            }
            inline void Insert(u64 index, const ElementType& value) {
                if (size == capacity) {
                    Reserve(capacity * 2 + 1);
                }
                for (u64 i = size; i > index; --i) {
                    if constexpr (MC) {
                        new (IndexToPointer(i)) ElementType(TypeMove(operator[](i - 1)));
                    }
                    else {
                        new (IndexToPointer(i)) ElementType(operator[](i - 1));
                    }
                }
                new (IndexToPointer(index)) ElementType(value);
                ++size;
            }
            template <IList CollectionType>
            inline void InsertRange(u64 index, const CollectionType& values) {
                if (size + values.GetSize() > capacity) {
                    u64 newCapacity = capacity * 2 + 1;
                    while (newCapacity < size + values.GetSize()) {
                        newCapacity = newCapacity * 2 + 1;
                    }
                    Reserve(newCapacity);
                }
                for (u64 i = size; i > index; --i) {
                    if constexpr (MC) {
                        new (IndexToPointer(i + values.size - 1)) ElementType(TypeMove(operator[](i - 1)));
                    }
                    else {
                        new (IndexToPointer(i + values.size - 1)) ElementType(operator[](i - 1));
                    }
                }
                for (u64 i = 0; i < values.GetSize(); ++i) {
                    new (IndexToPointer(index + i)) ElementType(values[i]);
                }
                size += values.GetSize();
            }
            template <IList CollectionType>
            inline void InsertRange(u64 index, const CollectionType& values, u64 count) {
                if (values.GetSize() < count) {
                    throw std::invalid_argument("Count must be less than or equal to collection size");
                }
                if (size + count > capacity) {
                    u64 newCapacity = capacity * 2 + 1;
                    while (newCapacity < size + count) {
                        newCapacity = newCapacity * 2 + 1;
                    }
                    Reserve(newCapacity);
                }
                for (u64 i = size; i > index; --i) {
                    if constexpr (MC) {
                        new (IndexToPointer(i + count - 1)) ElementType(TypeMove(operator[](i - 1)));
                    }
                    else {
                        new (IndexToPointer(i + count - 1)) ElementType(operator[](i - 1));
                    }
                }
                for (u64 i = 0; i < count; ++i) {
                    new (IndexToPointer(index + i)) ElementType(values[i]);
                }
                size += count;
            }
            inline void RemoveAt(u64 index) noexcept {
                IndexToPointer(index)->~ElementType();
                for (u64 i = index; i < size - 1; ++i) {
                    if constexpr (MC) {
                        new (IndexToPointer(i)) ElementType(TypeMove(operator[](i + 1)));
                    }
                    else {
                        new (IndexToPointer(i)) ElementType(operator[](i + 1));
                    }
                }
                --size;
            }
            inline void Clear() noexcept {
                for (u64 i = 0; i < size; ++i) {
                    IndexToPointer(i)->~ElementType();
                }
                size = 0;
            }
            inline void Reserve(u64 newCapacity) {
            if (newCapacity < capacity) {
                throw std::invalid_argument("Must reserve larger capacity than current capacity");
            }
            byte* newData = new byte[newCapacity * align];
            for (u64 i = 0; i < size; ++i) {
                ElementType* pointer = IndexToPointer(i);
                if constexpr (MC) {
                    new (newData + i * align) ElementType(TypeMove(*pointer));
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
            inline ElementType& operator[](u64 index) noexcept {
            return *IndexToPointer(index);
            }
            inline const ElementType& operator[](u64 index) const noexcept {
            return *IndexToPointer(index);
        }
    };
}