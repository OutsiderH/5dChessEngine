export module Engine.Game.Position;

import Engine.Type.Number;
import Engine.Game.TimelineIndex;

using namespace Engine::Type;

namespace Engine::Game {
    export class PositionDelta {
        friend class Position;
    private:
        i8 x = 0;
        i8 y = 0;
        i32 t = 0;
        i32 l = 0;
    public:
        inline constexpr PositionDelta() noexcept {}
        inline constexpr PositionDelta(const PositionDelta& other) noexcept :
            x(other.x),
            y(other.y),
            t(other.t),
            l(other.l) {}
        inline constexpr PositionDelta(PositionDelta&& other) noexcept :
            x(other.x),
            y(other.y),
            t(other.t),
            l(other.l) {}
        inline constexpr PositionDelta(i8 x, i8 y, i32 t, i32 l) noexcept :
            x(x),
            y(y),
            t(t),
            l(l) {}
        inline constexpr i8 GetX() const noexcept {
            return x;
        }
        inline constexpr i8 GetY() const noexcept {
            return y;
        }
        inline constexpr i32 GetT() const noexcept {
            return t;
        }
        inline constexpr i32 GetL() const noexcept {
            return l;
        }
        inline constexpr PositionDelta operator*(i32 right) const noexcept {
            PositionDelta result(*this);
            return (result *= right);
        }
        inline constexpr PositionDelta& operator*=(i32 right) noexcept {
            x *= right;
            y *= right;
            t *= right;
            l *= right;
            return *this;
        }
        inline constexpr PositionDelta& operator=(const PositionDelta& other) noexcept {
            x = other.x;
            y = other.y;
            t = other.t;
            l = other.l;
            return *this;
        }
        inline constexpr PositionDelta& operator=(PositionDelta&& other) noexcept {
            x = other.x;
            y = other.y;
            t = other.t;
            l = other.l;
            return *this;
        }
    };
    export class Position {
    private:
        u8 x = 0;
        u8 y = 0;
        u32 t = 0;
        TimelineIndex l;
    public:
        inline constexpr Position() noexcept {}
        inline constexpr Position(const Position& other) noexcept :
            x(other.x),
            y(other.y),
            t(other.t),
            l(other.l) {}
        inline constexpr Position(Position&& other) noexcept :
            x(other.x),
            y(other.y),
            t(other.t),
            l(other.l) {}
        inline constexpr Position(i8 x, i8 y, i32 t, TimelineIndex l) noexcept :
            x(x),
            y(y),
            t(t),
            l(l) {}
        inline constexpr u8 GetX() const noexcept {
            return x;
        }
        inline constexpr u8 GetY() const noexcept {
            return y;
        }
        inline constexpr u32 GetT() const noexcept {
            return t;
        }
        inline constexpr TimelineIndex GetL() const noexcept {
            return l;
        }
        inline constexpr bool OnBoardXY() const noexcept {
            return x < 8 && y < 8;
        }
        inline constexpr Position operator+(const PositionDelta& right) const noexcept {
            Position result(*this);
            return (result += right);
        }
        inline constexpr Position& operator+=(const PositionDelta& right) noexcept {
            x += right.x;
            y += right.y;
            t += right.t;
            l += right.l;
            return *this;
        }
        inline constexpr Position& operator=(const Position& other) noexcept {
            x = other.x;
            y = other.y;
            t = other.t;
            l = other.l;
            return *this;
        }
        inline constexpr Position& operator=(Position&& other) noexcept {
            x = other.x;
            y = other.y;
            t = other.t;
            l = other.l;
            return *this;
        }
    };

}