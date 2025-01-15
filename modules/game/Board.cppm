module;

#include <stdexcept>

export module Engine.Game.Board;

import Engine.Type.Number;
import Engine.Type.Util;
import Engine.Game.Piece;
import Engine.Collection.Array;
import Engine.Collection.Vector;
import Engine.Collection.String;

using namespace Engine::Type;
using namespace Engine::Collection;

namespace Engine::Game {
    export class Board {
    private:
        Array<Array<Piece, 8>, 8> pieces;
        bool castling[4];
        u8 enPassant[2];
    public:
        inline Board() noexcept {}
        inline Board(const Board& other) noexcept :
            pieces(other.pieces) {
            castling[0] = other.castling[0];
            castling[1] = other.castling[1];
            castling[2] = other.castling[2];
            castling[3] = other.castling[3];
            enPassant[0] = other.enPassant[0];
            enPassant[1] = other.enPassant[1];
        }
        inline Board(Board&& other) noexcept :
            pieces(Move(other.pieces)) {
            castling[0] = other.castling[0];
            castling[1] = other.castling[1];
            castling[2] = other.castling[2];
            castling[3] = other.castling[3];
            enPassant[0] = other.enPassant[0];
            enPassant[1] = other.enPassant[1];
        }
        inline Board(const Array<Array<Piece, 8>, 8>& pieces, bool castlingFlag0, bool castlingFlag1, bool castlingFlag2, bool castlingFlag3, u8 enPassant0, u8 enPassant1) noexcept :
            pieces(pieces) {
            castling[0] = castlingFlag0;
            castling[1] = castlingFlag1;
            castling[2] = castlingFlag2;
            castling[3] = castlingFlag3;
            enPassant[0] = enPassant0;
            enPassant[1] = enPassant1;
        }
        inline Board(const String& notation) {
            Vector<String> splits = notation.Split(' ');
            if (splits.GetSize() < 3) {
                throw std::invalid_argument("Invalid notation, insufficient parts");
            }
            Vector<String> piecesSplits = splits[0].Split('/');
            if (piecesSplits.GetSize() != 8) {
                throw std::invalid_argument("Invalid notation, incorrect pieces info");
            }
            for (u64 i = 0, splitIndex = 7; i < 8; ++i, --splitIndex) {
                u64 pieceIndex = 0;
                for (u64 j = 0; j < piecesSplits[splitIndex].Length(); ++j) {
                    char pieceChar = piecesSplits[splitIndex][j];
                    if (IsDigit(pieceChar)) {
                        pieceIndex += pieceChar - '0';
                    }
                    else {
                        Index(pieceIndex, i) = Piece::FromSideRelativeChar(pieceChar);
                        ++pieceIndex;
                    }
                }
            }
            castling[0] = splits[1][0] == 'K';
            castling[1] = splits[1][1] == 'Q';
            castling[2] = splits[1][2] == 'k';
            castling[3] = splits[1][3] == 'q';
            if (splits[2][0] == '-') {
                enPassant[0] = 0xFF;
                enPassant[1] = 0xFF;
            }
            else {
                enPassant[0] = splits[2][0] - 'a';
                enPassant[1] = splits[2][1] - '1';
            }
        }
        inline String ToNotation() const {
            String result;
            result.GetContainer().Reserve(80);
            AppendNotationToString(result);
            return result;
        }
        inline void AppendNotationToString(String& str) const {
            for (u32 y = 7; y >= 0; --y) {
                u32 emptyCount = 0;
                for (u32 x = 0; x < 8; ++x) {
                    char pieceChar = Index(x, y).ToSideRelativeChar();
                    if (pieceChar == '-') {
                        ++emptyCount;
                    }
                    else if (emptyCount > 0) {
                        str += '0' + emptyCount;
                    }
                    else {
                        str += pieceChar;
                    }
                }
                if (emptyCount > 0) {
                    str += '0' + emptyCount;
                }
                if (y == 0) {
                    break;
                }
                str += '/';
            }
            str += ' ';
            str += castling[0] ? 'K' : '-';
            str += castling[1] ? 'Q' : '-';
            str += castling[2] ? 'k' : '-';
            str += castling[3] ? 'q' : '-';
            str += ' ';
            if (enPassant[0] == 0xFF) {
                str += '-';
            }
            else {
                str += 'a' + enPassant[0];
                str += '1' + enPassant[1];
            }
        }
        inline Piece& Index(u8 x, u8 y) noexcept {
            return pieces[y][x];
        }
        inline const Piece& Index(u8 x, u8 y) const noexcept {
            return pieces[y][x];
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
    export const Board standardBoard({
        {Piece(Side::White, PieceType::Rook), Piece(Side::White, PieceType::Knight), Piece(Side::White, PieceType::Bishop), Piece(Side::White, PieceType::Queen), Piece(Side::White, PieceType::King), Piece(Side::White, PieceType::Bishop), Piece(Side::White, PieceType::Knight), Piece(Side::White, PieceType::Rook)},
        {Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn), Piece(Side::White, PieceType::Pawn)},
        {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
        {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
        {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
        {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
        {Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn), Piece(Side::Black, PieceType::Pawn)},
        {Piece(Side::Black, PieceType::Rook), Piece(Side::Black, PieceType::Knight), Piece(Side::Black, PieceType::Bishop), Piece(Side::Black, PieceType::Queen), Piece(Side::Black, PieceType::King), Piece(Side::Black, PieceType::Bishop), Piece(Side::Black, PieceType::Knight), Piece(Side::Black, PieceType::Rook)}
    }, true, true, true, true, 0xFF, 0xFF);
}