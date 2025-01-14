export module Engine.Type.Util;

export namespace Engine::Type {
    template <typename Type>
    class RemoveReference {
    public:
        using Result = Type;
    };
    template <typename Type>
    class RemoveReference<Type&> {
    public:
        using Result = Type;
    };
    template <typename Type>
    class RemoveReference<Type&&> {
    public:
        using Result = Type;
    };
    template <typename Type>
    class IsReference {
    public:
        constexpr static bool Result = false;
    };
    template <typename Type>
    class IsReference<Type&> {
    public:
        constexpr static bool Result = true;
    };
    template <typename Type>
    class IsReference<Type&&> {
    public:
        constexpr static bool Result = true;
    };
    template <typename Type>
    class IsPointer {
    public:
        constexpr static bool Result = false;
    };
    template <typename Type>
    class IsPointer<Type*> {
    public:
        constexpr static bool Result = true;
    };
    template <typename Type1, typename Type2>
    class IsSame {
    public:
        constexpr static bool Result = false;
    };
    template <typename Type>
    class IsSame<Type, Type> {
    public:
        constexpr static bool Result = true;
    };
    template <typename Type>
    inline constexpr RemoveReference<Type>::Result&& Move(Type&& value) noexcept {
        return static_cast<RemoveReference<Type>::Result&&>(value);
    }
    template <typename Type>
    inline constexpr Type&& Value() noexcept {
        static_assert(false, "This function can not be called at evaluated expression");
    }
}