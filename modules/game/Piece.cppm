export module Engine.Game.Piece;

import Engine.Type.Number;
import Engine.Game.Side;

using namespace Engine::Type;

namespace Engine::Game {
    export enum class PieceType : u8 {
        None,
        Pawn,
        Knight,
        Bishop,
        Rook,
        Queen,
        King
    };
    export class Piece {
    private:
        Side side = Side::None;
        PieceType type = PieceType::None;
    public:
        inline static constexpr Piece FromSideRelativeChar(char c) noexcept {
            Piece result;
            switch (c) {
                case 'p':
                    result.side = Side::Black;
                    result.type = PieceType::Pawn;
                    break;
                case 'P':
                    result.side = Side::White;
                    result.type = PieceType::Pawn;
                    break;
                case 'n':
                    result.side = Side::Black;
                    result.type = PieceType::Knight;
                    break;
                case 'N':
                    result.side = Side::White;
                    result.type = PieceType::Knight;
                    break;
                case 'b':
                    result.side = Side::Black;
                    result.type = PieceType::Bishop;
                    break;
                case 'B':
                    result.side = Side::White;
                    result.type = PieceType::Bishop;
                    break;
                case 'r':
                    result.side = Side::Black;
                    result.type = PieceType::Rook;
                    break;
                case 'R':
                    result.side = Side::White;
                    result.type = PieceType::Rook;
                    break;
                case 'q':
                    result.side = Side::Black;
                    result.type = PieceType::Queen;
                    break;
                case 'Q':
                    result.side = Side::White;
                    result.type = PieceType::Queen;
                    break;
                case 'k':
                    result.side = Side::Black;
                    result.type = PieceType::King;
                    break;
                case 'K':
                    result.side = Side::White;
                    result.type = PieceType::King;
                    break;
                default:
                    break;
            }
            return result;
        }
        inline constexpr Piece() noexcept {}
        inline constexpr Piece(const Piece& other) noexcept :
            side(other.side),
            type(other.type) {}
        inline constexpr Piece(Piece&& other) :
            side(other.side),
            type(other.type) {}
        inline constexpr Piece(Side side, PieceType type) noexcept :
            side(side),
            type(type) {}
        inline constexpr Side GetSide() const noexcept {
            return side;
        }
        inline constexpr PieceType GetPieceType() const noexcept {
            return type;
        }
        inline constexpr char ToSideRelativeChar() const noexcept {
            constexpr const char* charsBlack = "?pnbrqk";
            constexpr const char* charsWhite = "?PNBRQK";
            const char* chars;
            switch (side) {
                case Side::White:
                    chars = charsWhite;
                    break;
                case Side::Black:
                    chars = charsBlack;
                    break;
                case Side::None:
                    return '-';
                default:
                    return '?';
            }
            return chars[static_cast<u8>(type)];
        }
        inline constexpr bool SameSide(const Piece& other) const noexcept {
            return side == other.side;
        }
        inline constexpr bool OppositeSide(const Piece& other) const noexcept {
            return side == SideFunctions::Opposite(other.side);
        }
        inline constexpr Piece& operator=(const Piece& other) noexcept {
            side = other.side;
            type = other.type;
            return *this;
        }
        inline constexpr Piece& operator=(Piece&& other) {
            side = other.side;
            type = other.type;
            return *this;
        }
    };
}