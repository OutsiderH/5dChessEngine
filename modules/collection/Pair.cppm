export module Engine.Collection.Pair;

import Engine.Type.Number;

using namespace Engine::Type;

namespace Engine::Collection {
    export template <typename ElementType1, typename ElementType2>
    class Pair {
    private:
        ElementType1 e1;
        ElementType2 e2;
    public:
        inline Pair() noexcept {}
        inline Pair(ElementType1 e1, ElementType2 e2) : e1(e1), e2(e2) {}
        inline ElementType1& GetE1() noexcept {
            return e1;
        }
        inline const ElementType1& GetE1() const noexcept {
            return e1;
        }
        inline ElementType2& GetE2() noexcept {
            return e2;
        }
        inline const ElementType2& GetE2() const noexcept {
            return e2;
        }
    };
}