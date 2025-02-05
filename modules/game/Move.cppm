export module Engine.Game.Move;

import Engine.Type.Number;
import Engine.Game.Piece;
import Engine.Game.Position;
import Engine.Collection.Vector;

using namespace Engine::Type;
using namespace Engine::Collection;

namespace Engine::Game {
    export class Move {
    private:
        Position from;
        Position to;
    public:
        inline constexpr Move() noexcept {}
        inline constexpr Move(const Move& other) noexcept :
            from(other.from),
            to(other.to) {}
        inline constexpr Move(Move&& other) noexcept :
            from(other.from),
            to(other.to) {}
        inline constexpr Move(Position from, Position to) noexcept :
            from(from),
            to(to) {}
        inline constexpr Position GetFrom() const noexcept {
            return from;
        }
        inline constexpr Position GetTo() const noexcept {
            return to;
        }
        inline constexpr i32 DeltaL() const noexcept {
            return to.GetL() - from.GetL();
        }
        inline constexpr i32 DeltaT() const noexcept {
            return to.GetT() - from.GetT();
        }
        inline constexpr Move& operator=(const Move& other) noexcept {
            from = other.from;
            to = other.to;
            return *this;
        }
        inline constexpr Move& operator=(Move&& other) noexcept {
            from = other.from;
            to = other.to;
            return *this;
        }
    };
    export class PiecesPositionDelta {
    public:
        inline static const Vector<PositionDelta> WP{
            PositionDelta(0, 1, 0, 0), PositionDelta(0, 0, 0, 1)
        };
        inline static const Vector<PositionDelta> BP{
            PositionDelta(0, -1, 0, 0), PositionDelta(0, 0, 0, -1)
        };
        inline static const Vector<PositionDelta> WPT{
            PositionDelta(1, 1, 0, 0), PositionDelta(-1, 1, 0, 0), PositionDelta(0, 0, 2, 1), PositionDelta(0, 0, -2, 1)
        };
        inline static const Vector<PositionDelta> BPT{
            PositionDelta(1, -1, 0, 0), PositionDelta(-1, -1, 0, 0), PositionDelta(0, 0, 2, -1), PositionDelta(0, 0, -2, -1)
        };
        inline static const Vector<PositionDelta> N{
            PositionDelta(2, 1, 0, 0), PositionDelta(2, 0, 2, 0), PositionDelta(2, 0, 0, 1), PositionDelta(0, 2, 2, 0), PositionDelta(0, 2, 0, 1), PositionDelta(0, 0, 4, 1),
            PositionDelta(-2, 1, 0, 0), PositionDelta(-2, 0, 2, 0), PositionDelta(-2, 0, 0, 1), PositionDelta(0, -2, 2, 0), PositionDelta(0, -2, 0, 1), PositionDelta(0, 0, -4, 1),
            PositionDelta(2, -1, 0, 0), PositionDelta(2, 0, -2, 0), PositionDelta(2, 0, 0, -1), PositionDelta(0, 2, -2, 0), PositionDelta(0, 2, 0, -1), PositionDelta(0, 0, 4, -1),
            PositionDelta(-2, -1, 0, 0), PositionDelta(-2, 0, -2, 0), PositionDelta(-2, 0, 0, -1), PositionDelta(0, -2, -2, 0), PositionDelta(0, -2, 0, -1), PositionDelta(0, 0, -4, -1),
            PositionDelta(1, 2, 0, 0), PositionDelta(1, 0, 4, 0), PositionDelta(1, 0, 0, 2), PositionDelta(0, 1, 4, 0), PositionDelta(0, 1, 0, 2), PositionDelta(0, 0, 2, 2),
            PositionDelta(-1, 2, 0, 0), PositionDelta(-1, 0, 4, 0), PositionDelta(-1, 0, 0, 2), PositionDelta(0, -1, 4, 0), PositionDelta(0, -1, 0, 2), PositionDelta(0, 0, -2, 2),
            PositionDelta(1, -2, 0, 0), PositionDelta(1, 0, -4, 0), PositionDelta(1, 0, 0, -2), PositionDelta(0, 1, -4, 0), PositionDelta(0, 1, 0, -2), PositionDelta(0, 0, 2, -2),
            PositionDelta(-1, -2, 0, 0), PositionDelta(-1, 0, -4, 0), PositionDelta(-1, 0, 0, -2), PositionDelta(0, -1, -4, 0), PositionDelta(0, -1, 0, -2), PositionDelta(0, 0, -2, -2)
        };
        inline static const Vector<PositionDelta> R{
            PositionDelta(1, 0, 0, 0), PositionDelta(0, 1, 0, 0), PositionDelta(0, 0, 2, 0), PositionDelta(0, 0, 0, 1),
            PositionDelta(-1, 0, 0, 0), PositionDelta(0, -1, 0, 0), PositionDelta(0, 0, -2, 0), PositionDelta(0, 0, 0, -1)
        };
        inline static const Vector<PositionDelta> B{
            PositionDelta(1, 1, 0, 0), PositionDelta(1, -1, 0, 0), PositionDelta(-1, 1, 0, 0), PositionDelta(-1, -1, 0, 0),
            PositionDelta(1, 0, 2, 0), PositionDelta(1, 0, -2, 0), PositionDelta(-1, 0, 2, 0), PositionDelta(-1, 0, -2, 0),
            PositionDelta(1, 0, 0, 1), PositionDelta(1, 0, 0, -1), PositionDelta(-1, 0, 0, 1), PositionDelta(-1, 0, 0, -1),
            PositionDelta(0, 1, 2, 0), PositionDelta(0, 1, -2, 0), PositionDelta(0, -1, 2, 0), PositionDelta(0, -1, -2, 0),
            PositionDelta(0, 1, 0, 1), PositionDelta(0, 1, 0, -1), PositionDelta(0, -1, 0, 1), PositionDelta(0, -1, 0, -1),
            PositionDelta(0, 0, 2, 1), PositionDelta(0, 0, 2, -1), PositionDelta(0, 0, -2, 1), PositionDelta(0, 0, -2, -1)
        };
        inline static const Vector<PositionDelta> KQ{
            PositionDelta(1, 0, 0, 0), PositionDelta(0, 1, 0, 0), PositionDelta(0, 0, 2, 0), PositionDelta(0, 0, 0, 1),
            PositionDelta(-1, 0, 0, 0), PositionDelta(0, -1, 0, 0), PositionDelta(0, 0, -2, 0), PositionDelta(0, 0, 0, -1),
            PositionDelta(1, 1, 0, 0), PositionDelta(1, 0, 2, 0), PositionDelta(1, 0, 0, 1), PositionDelta(0, 1, 2, 0), PositionDelta(0, 1, 0, 1), PositionDelta(0, 0, 2, 1),
            PositionDelta(-1, 1, 0, 0), PositionDelta(-1, 0, 2, 0), PositionDelta(-1, 0, 0, 1), PositionDelta(0, -1, 2, 0), PositionDelta(0, -1, 0, 1), PositionDelta(0, 0, -2, 1),
            PositionDelta(1, -1, 0, 0), PositionDelta(1, 0, -2, 0), PositionDelta(1, 0, 0, -1), PositionDelta(0, 1, -2, 0), PositionDelta(0, 1, 0, -1), PositionDelta(0, 0, 2, -1),
            PositionDelta(-1, -1, 0, 0), PositionDelta(-1, 0, -2, 0), PositionDelta(-1, 0, 0, -1), PositionDelta(0, -1, -2, 0), PositionDelta(0, -1, 0, -1), PositionDelta(0, 0, -2, -1),
            PositionDelta(1, 1, 2, 0), PositionDelta(1, 1, 0, 1), PositionDelta(1, 0, 2, 1), PositionDelta(0, 1, 2, 1),
            PositionDelta(-1, 1, 2, 0), PositionDelta(-1, 1, 0, 1), PositionDelta(-1, 0, 2, 1), PositionDelta(0, -1, 2, 1),
            PositionDelta(1, -1, 2, 0), PositionDelta(1, -1, 0, 1), PositionDelta(1, 0, -2, 1), PositionDelta(0, 1, -2, 1),
            PositionDelta(1, 1, -2, 0), PositionDelta(1, 1, 0, -1), PositionDelta(1, 0, 2, -1), PositionDelta(0, 1, 2, -1),
            PositionDelta(-1, -1, 2, 0), PositionDelta(-1, -1, 0, 1), PositionDelta(-1, 0, -2, 1), PositionDelta(0, -1, -2, 1),
            PositionDelta(-1, 1, -2, 0), PositionDelta(-1, 1, 0, -1), PositionDelta(-1, 0, 2, -1), PositionDelta(0, -1, 2, -1),
            PositionDelta(1, -1, -2, 0), PositionDelta(1, -1, 0, -1), PositionDelta(1, 0, -2, -1), PositionDelta(0, 1, -2, -1),
            PositionDelta(-1, -1, -2, 0), PositionDelta(-1, -1, 0, -1), PositionDelta(-1, 0, -2, -1), PositionDelta(0, -1, -2, -1),
            PositionDelta(1, 1, 2, 1),
            PositionDelta(-1, 1, 2, 1), PositionDelta(1, -1, 2, 1), PositionDelta(1, 1, -2, 1), PositionDelta(1, 1, 2, -1),
            PositionDelta(-1, -1, 2, 1), PositionDelta(-1, 1, -2, 1), PositionDelta(-1, 1, 2, -1), PositionDelta(1, -1, -2, 1), PositionDelta(1, -1, 2, -1), PositionDelta(1, 1, -2, -1),
            PositionDelta(-1, -1, -2, 1), PositionDelta(-1, -1, 2, -1), PositionDelta(-1, 1, -2, -1), PositionDelta(1, -1, -2, -1),
            PositionDelta(-1, -1, -2, -1)
        };
    };
}