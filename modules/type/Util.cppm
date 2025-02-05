export module Engine.Type.Util;

namespace Engine::Type {
    export template <typename Type>
    class RemoveReference {
    public:
        using Result = Type;
    };
    export template <typename Type>
    class RemoveReference<Type&> {
    public:
        using Result = Type;
    };
    export template <typename Type>
    class RemoveReference<Type&&> {
    public:
        using Result = Type;
    };
    export template <typename Type>
    class IsReference {
    public:
        constexpr static bool Result = false;
    };
    export template <typename Type>
    class IsReference<Type&> {
    public:
        constexpr static bool Result = true;
    };
    export template <typename Type>
    class IsReference<Type&&> {
    public:
        constexpr static bool Result = true;
    };
    export template <typename Type>
    class IsPointer {
    public:
        constexpr static bool Result = false;
    };
    export template <typename Type>
    class IsPointer<Type*> {
    public:
        constexpr static bool Result = true;
    };
    export template <typename Type1, typename Type2>
    class IsSame {
    public:
        constexpr static bool Result = false;
    };
    export template <typename Type>
    class IsSame<Type, Type> {
    public:
        constexpr static bool Result = true;
    };
    export template <typename Type>
    inline constexpr RemoveReference<Type>::Result&& TypeMove(Type&& value) noexcept {
        return static_cast<RemoveReference<Type>::Result&&>(value);
    }
    export template <typename Type>
    inline constexpr Type&& TypeValue() noexcept {
        static_assert(false, "This function can not be called at evaluated expression");
    }
    export inline constexpr bool IsDigit(char c) noexcept {
        return c >= '0' && c <= '9';
    }
}