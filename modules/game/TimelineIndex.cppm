export module Engine.Game.TimelineIndex;

import Engine.Type.Number;

using namespace Engine::Type;

namespace Engine::Game {
    export enum class TimelineIndexSign : u8 {
        Positive,
        Negative
    };
    export class TimelineIndex {
    private:
        u32 value = 0;
        TimelineIndexSign sign = TimelineIndexSign::Positive;
    public:
        inline constexpr TimelineIndex() noexcept {}
        inline constexpr TimelineIndex(const TimelineIndex& other) noexcept :
            value(other.value),
            sign(other.sign) {}
        inline constexpr TimelineIndex(TimelineIndex&& other) noexcept :
            value(other.value),
            sign(other.sign) {}
        inline constexpr TimelineIndex(u32 value, TimelineIndexSign sign) noexcept :
            value(value),
            sign(sign) {}
        inline constexpr u32 GetValue() const noexcept {
            return value;
        }
        inline constexpr bool IsPositive() const noexcept {
            return sign == TimelineIndexSign::Positive;
        }
        inline constexpr bool IsNegative() const noexcept {
            return sign == TimelineIndexSign::Negative;
        }
        inline constexpr TimelineIndex operator+(i32 right) const noexcept {
            TimelineIndex result(*this);
            result += right;
            return result;
        }
        inline constexpr TimelineIndex operator-(i32 right) const noexcept {
            TimelineIndex result(*this);
            result -= right;
            return result;
        }
        inline constexpr i32 operator-(TimelineIndex right) const noexcept {
            if (IsPositive() && right.IsPositive()) {
                return static_cast<i32>(value) - right.value;
            }
            else if (IsPositive() && right.IsNegative()) {
                return static_cast<i32>(value) - right.value + 1;
            }
            else if (IsNegative() && right.IsPositive()) {
                return 0 - static_cast<i32>(value) - right.value - 1;
            }
            else {
                return 0 - static_cast<i32>(value) - right.value;
            }
        }
        inline constexpr TimelineIndex& operator+=(i32 right) noexcept {
            if (IsPositive()) {
                if (static_cast<i32>(value) >= 0 - right) {
                    value += right;
                }
                else {
                    value = 0 - right - value - 1;
                    sign = TimelineIndexSign::Negative;
                }
            }
            else {
                if (static_cast<i32>(value) >= right) {
                    value -= right;
                }
                else {
                    value = right - value - 1;
                    sign = TimelineIndexSign::Positive;
                }
            }
            return *this;
        }
        inline constexpr TimelineIndex& operator-=(i32 right) noexcept {
            return *this += 0 - right;
        }
        inline constexpr TimelineIndex& operator=(const TimelineIndex& other) noexcept {
            value = other.value;
            sign = other.sign;
            return *this;
        }
        inline constexpr TimelineIndex& operator=(TimelineIndex&& other) noexcept {
            value = other.value;
            sign = other.sign;
            return *this;
        }
    };
}