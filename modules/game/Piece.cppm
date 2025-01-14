export module Engine.Game.Piece;

import Engine.Type.Number;
import Engine.Game.Side;

using namespace Engine::Type;

export namespace Engine::Game {
    enum class PieceType : u8 {
        None,
        Pawn,
        Knight,
        Bishop,
        Rook,
        Queen,
        King
    };
    class Piece {
    private:
        Side side;
        PieceType type;
    public:
        inline constexpr Piece() noexcept :
            side(Side::None),
            type(PieceType::None) {}
        inline constexpr Piece(const Piece& other) noexcept :
            side(other.side),
            type(other.type) {}
        Piece(Piece&& other) = delete;
        inline constexpr Piece(Side side, PieceType type) noexcept :
            side(side),
            type(type) {}
        inline constexpr Side GetSide() const noexcept {
            return side;
        }
        inline constexpr PieceType GetPieceType() const noexcept {
            return type;
        }
        inline constexpr bool SameSide(const Piece& other) const noexcept {
            return side == other.side;
        }
        inline constexpr Piece& operator=(const Piece& other) noexcept {
            side = other.side;
            type = other.type;
            return *this;
        }
        Piece& operator=(Piece&& other) = delete;
    };
}