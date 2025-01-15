module;

#include <initializer_list>
#include <stdexcept>

export module Engine.Collection.Array;

import Engine.Type.Number;
import Engine.Type.Concept;
import Engine.Type.Util;

using namespace Engine::Type;

namespace Engine::Collection {
    export template <typename ElementType, u64 size, u64 align = sizeof(ElementType)>
    requires (
        !IsReference<ElementType>::Result
        && CopyConstructible<ElementType>
        && DefaultConstructible<ElementType>
        && Destructible<ElementType>
        && size != 0
        && align >= sizeof(ElementType)
    )
    class Array {
    private:
        static constexpr bool MC = MoveConstructible<ElementType>;
        static constexpr bool NTDC = NonThrowDefaultConstructible<ElementType>;
        static constexpr bool NTCC = NonThrowCopyConstructible<ElementType>;
        static constexpr bool NTMC = NonThrowMoveConstructible<ElementType>;
        static constexpr bool NTD = NonThrowDestructible<ElementType>;
        byte data[size * align];
        inline ElementType* IndexToPointer(u64 index) noexcept {
            return reinterpret_cast<ElementType*>(data + index * align);
        }
        inline const ElementType* IndexToPointer(u64 index) const noexcept {
            return reinterpret_cast<const ElementType*>(data + index * align);
        }
    public:
        inline Array() noexcept(NTDC) {
            for (u64 i = 0; i < size; ++i) {
                new (IndexToPointer(i)) ElementType();
            }
        }
        inline Array(const Array& other) noexcept(NTCC) {
            for (u64 i = 0; i < size; ++i) {
                new (IndexToPointer(i)) ElementType(other[i]);
            }
        }
        inline Array(Array&& other) noexcept(MC ? NTMC : NTCC) {
            for (u64 i = 0; i < size; ++i) {
                if constexpr (MC) {
                    new (IndexToPointer(i)) ElementType(Move(other[i]));
                }
                else {
                    new (IndexToPointer(i)) ElementType(other[i]);
                }
            }
        }
        inline Array(std::initializer_list<ElementType> list) noexcept(MC ? NTMC : NTCC) {
            for (u64 i = 0; i < size; ++i) {
                if constexpr (MC) {
                    new (IndexToPointer(i)) ElementType(Move(list.begin()[i]));
                }
                else {
                    new (IndexToPointer(i)) ElementType(list.begin()[i]);
                }
            }
        }
        inline explicit Array(const ElementType& fill) noexcept(NTCC) {
            for (u64 i = 0; i < size; ++i) {
                new (IndexToPointer(i)) ElementType(fill);
            }
        }
        inline ~Array() noexcept(NTD) {
            for (u64 i = 0; i < size; ++i) {
                IndexToPointer(i)->~ElementType();
            }
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
        inline Array& operator=(const Array& other) noexcept(NTCC && NTD) {
            if (this != &other) {
                for (u64 i = 0; i < size; ++i) {
                    ElementType* pointer = IndexToPointer(i);
                    pointer->~ElementType();
                    new (pointer) ElementType(other[i]);
                }
            }
            return *this;
        }
        inline Array& operator=(Array&& other) noexcept((MC ? NTMC : NTCC) && NTD) {
            if (this != &other) {
                for (u64 i = 0; i < size; ++i) {
                    ElementType* pointer = IndexToPointer(i);
                    pointer->~ElementType();
                    if constexpr (MC) {
                        new (pointer) ElementType(Move(other[i]));
                    }
                    else {
                        new (pointer) ElementType(other[i]);
                    }
                }
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