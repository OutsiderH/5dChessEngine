export module Engine.Type.Concept;

import Engine.Type.Util;

namespace Engine::Type {
    export template <typename Type1, typename Type2>
    concept SameAs = IsSame<Type1, Type2>::Result;
    export template <typename Type>
    concept ObjectType = !(IsReference<Type>::Result || IsPointer<Type>::Result);
    export template <typename Type>
    concept DefaultConstructible = requires() {
        { typename RemoveReference<Type>::Result() } -> SameAs<typename RemoveReference<Type>::Result>;
    };
    export template <typename Type>
    concept NonThrowDefaultConstructible = requires() {
        { typename RemoveReference<Type>::Result() } noexcept -> SameAs<typename RemoveReference<Type>::Result>;
    };
    export template <typename Type>
    concept CopyConstructible = requires(const RemoveReference<Type>::Result& other) {
        { typename RemoveReference<Type>::Result(other) } -> SameAs<typename RemoveReference<Type>::Result>;
    };
    export template <typename Type>
    concept NonThrowCopyConstructible = requires(const RemoveReference<Type>::Result & other) {
        { typename RemoveReference<Type>::Result(other) } noexcept -> SameAs<typename RemoveReference<Type>::Result>;
    };
    export template <typename Type>
    concept MoveConstructible = requires(RemoveReference<Type>::Result&& other) {
        { typename RemoveReference<Type>::Result(Move(other)) } -> SameAs<typename RemoveReference<Type>::Result>;
    };
    export template <typename Type>
    concept NonThrowMoveConstructible = requires(RemoveReference<Type>::Result&& other) {
        { typename RemoveReference<Type>::Result(Move(other)) } noexcept -> SameAs<typename RemoveReference<Type>::Result>;
    };
    export template <typename Type>
    concept Destructible = requires(typename RemoveReference<Type>::Result instance) {
        { instance.~Type() };
    };
    export template <typename Type>
    concept NonThrowDestructible = requires(typename RemoveReference<Type>::Result instance) {
        { instance.~Type() } noexcept;
    };
    export template <typename Type>
    concept CopyAssignable = requires(typename RemoveReference<Type>::Result left, const typename RemoveReference<Type>::Result& right) {
        { left = right } -> SameAs<typename RemoveReference<Type>::Result&>;
    };
    export template <typename Type>
    concept NonThrowCopyAssignable = requires(typename RemoveReference<Type>::Result left, const typename RemoveReference<Type>::Result& right) {
        { left = right } noexcept -> SameAs<typename RemoveReference<Type>::Result&>;
    };
    export template <typename Type>
    concept MoveAssignable = requires(typename RemoveReference<Type>::Result left, typename RemoveReference<Type>::Result && right) {
        { left = Move(right) } -> SameAs<typename RemoveReference<Type>::Result&>;
    };
    export template <typename Type>
    concept NonThrowMoveAssignable = requires(typename RemoveReference<Type>::Result left, typename RemoveReference<Type>::Result && right) {
        { left = Move(right) } noexcept -> SameAs<typename RemoveReference<Type>::Result&>;
    };
    export template <typename Type>
    concept EqualityComparable = requires(typename RemoveReference<Type>::Result left, typename RemoveReference<Type>::Result right) {
        { left == right } -> SameAs<bool>;
    };
    export template <typename Type>
    concept NonThrowEqualityComparable = requires(typename RemoveReference<Type>::Result left, typename RemoveReference<Type>::Result right) {
        { left == right } noexcept -> SameAs<bool>;
    };
}