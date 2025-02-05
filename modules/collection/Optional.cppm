export module Engine.Collection.Optional;

import Engine.Type.Number;
import Engine.Type.Util;

using namespace Engine::Type;

namespace Engine::Collection {
    export template <typename ElementType>
    class Optional {
    private:
        ElementType value;
        bool hasValue = false;
    public:
        inline Optional() noexcept : value() {}
        inline Optional(const Optional& other) noexcept : value(other.value), hasValue(other.hasValue) {}
        inline Optional(Optional&& other) noexcept : value(TypeMove(other.value)), hasValue(other.hasValue) {}
        inline explicit Optional(const ElementType& value) : value(value), hasValue(true) {}
        inline bool HasValue() const noexcept {
            return hasValue;
        }
        inline ElementType& GetValue() noexcept {
            return value;
        }
        inline const ElementType& GetValue() const noexcept {
            return value;
        }
        inline bool TryGetValue(ElementType& out_value) const noexcept {
            if (hasValue) {
                out_value = value;
                return true;
            }
            return false;
        }
    };
}