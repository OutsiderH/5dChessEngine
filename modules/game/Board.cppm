export module Engine.Game.Board;

import Engine.Type.Number;
import Engine.Type.Util;
import Engine.Game.Piece;
import Engine.Collection.Array;
import Engine.Collection.String;

using namespace Engine::Type;
using namespace Engine::Collection;

export namespace Engine::Game {
    class Board {
    private:
        Array<Array<Piece, 8>, 8> pieces;
    public:
        inline Board() noexcept {}
        inline Board(const Board& other) noexcept :
            pieces(other.pieces) {}
        inline Board(Board&& other) noexcept :
            pieces(Move(other.pieces)) {}
        inline Board(const Array<Array<Piece, 8>, 8>& pieces) noexcept :
            pieces(pieces) {}
        inline String ToNotation() const {

        }
        inline Piece& Index(u8 x, u8 y) noexcept {
            return pieces[x][y];
        }
        inline const Piece& Index(u8 x, u8 y) const noexcept {
            return pieces[x][y];
        }
        inline Board& operator=(const Board& other) noexcept {
            pieces = other.pieces;
            return *this;
        }
        inline Board& operator=(Board&& other) {
            pieces = other.pieces;
            return *this;
        }
    };
    const Board emptyBoard;
    const Board standardBoard({
        {Piece(Side::White, PieceType::Rook), Piece(Side::White, PieceType::Knight), Piece(Side::White, PieceType::Bishop), Piece(Side::White, PieceType::Queen), Piece(Side::White, PieceType::King), Piece(Side::White, PieceType::Bishop), Piece(Side::White, PieceType::Knight), Piece(Side::White, PieceType::Rook)},
        {Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn)},
        {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
        {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
        {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
        {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
        {Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn)},
        {Piece(Side::Black, PieceType::Rook), Piece(Side::Black, PieceType::Knight), Piece(Side::Black, PieceType::Bishop), Piece(Side::Black, PieceType::Queen), Piece(Side::Black, PieceType::King), Piece(Side::Black, PieceType::Bishop), Piece(Side::Black, PieceType::Knight), Piece(Side::Black, PieceType::Rook)}
    });
}