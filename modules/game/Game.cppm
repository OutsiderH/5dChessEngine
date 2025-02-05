export module Engine.Game;

import Engine.Type.Number;
import Engine.Type.Util;
import Engine.Game.Side;
import Engine.Game.Position;
import Engine.Game.Move;
import Engine.Game.Board;
import Engine.Game.Timeline;
import Engine.Game.TimelineIndex;
import Engine.Collection.Vector;
import Engine.Collection.String;
import <stdexcept>;

using namespace Engine::Type;
using namespace Engine::Collection;

namespace Engine::Game {
    export class Game {
    private:
        Vector<Timeline> timelines;
        bool twoOriginalTimelines = false;
        u32 positive0Index = 0;
        Side currentSide = Side::None;
        u32 present = 0;
        u32 whiteTimelinesCount = 0;
        u32 blackTimelinesCount = 0;
        inline void FindLinearMoves(const Vector<PositionDelta>& moves, Vector<Move>& result, Position from, Piece piece, bool canCapture = true, u32 maxDist = 0xFFFF) const {
            for (u64 i = 0; i < moves.GetSize(); ++i) {
                for (u32 j = 1; j <= maxDist; ++j) {
                    Position to = from + moves[i] * j;
                    if (!to.OnBoardXY()) {
                        break;
                    }
                    if (!TimelineExist(to.GetL())) {
                        break;
                    }
                    const Timeline& toTimeline = IndexTimeline(to.GetL());
                    if (to.GetT() < 0 || static_cast<u32>(to.GetT()) >= toTimeline.BoardCount()) {
                        break;
                    }
                    const Board& toBoard = toTimeline[to.GetT()];
                    Piece toPiece = toBoard.Index(to.GetX(), to.GetY());
                    bool isCapture = piece.OppositeSide(toPiece);
                    if (toPiece.GetPieceType() != PieceType::None && (piece.SameSide(toPiece) || (!canCapture && isCapture))) {
                        break;
                    }
                    result.Append(Move(from, to));
                    if (canCapture && isCapture) {
                        break;
                    }
                }
            }
        }
        inline void FindNonLinearMoves(const Vector<PositionDelta>& moves, Vector<Move>& result, Position from, Piece piece, bool canNotCapture = true) const {
            for (u64 i = 0; i < moves.GetSize(); ++i) {
                Position to = from + moves[i];
                if (!to.OnBoardXY()) {
                    continue;
                }
                if (!TimelineExist(to.GetL())) {
                    continue;
                }
                const Timeline& toTimeline = IndexTimeline(to.GetL());
                if (to.GetT() < 0 || static_cast<u32>(to.GetT()) >= toTimeline.BoardCount()) {
                    continue;
                }
                const Board& toBoard = toTimeline[to.GetT()];
                Piece toPiece = toBoard.Index(to.GetX(), to.GetY());
                if (!canNotCapture && toPiece.GetPieceType() == PieceType::None) {
                    continue;
                }
                if (toPiece.GetPieceType() != PieceType::None && piece.SameSide(toPiece)) {
                    continue;
                }
                result.Append(Move(from, to));
            }
        }
    public:
        inline Game() noexcept {}
        inline Game(const Game& other) :
            timelines(other.timelines),
            twoOriginalTimelines(other.twoOriginalTimelines),
            positive0Index(other.positive0Index),
            currentSide(other.currentSide),
            present(other.present),
            whiteTimelinesCount(other.whiteTimelinesCount),
            blackTimelinesCount(other.blackTimelinesCount) {}
        inline Game(Game&& other) noexcept :
            timelines(TypeMove(other.timelines)),
            twoOriginalTimelines(other.twoOriginalTimelines),
            positive0Index(other.positive0Index),
            currentSide(other.currentSide),
            present(other.present),
            whiteTimelinesCount(other.whiteTimelinesCount),
            blackTimelinesCount(other.blackTimelinesCount) {}
        inline explicit Game(const String& notation) {
            // refactor this shit
            const Vector<String> splits = notation.Split('\n');
            if (splits.GetSize() < 2) {
                throw std::invalid_argument("Invalid notation, insufficient lines");
            }
            Side side;
            u64 index = 0;
            if (splits[0][index++] == '2') {
                twoOriginalTimelines = true;
            }
            if (splits[0][index++] != ' ') {
                throw std::invalid_argument("Invalid notation, space expected");
            }
            if (!SideFunctions::TryFromChar(splits[0][index++], side)) {
                throw std::invalid_argument("Invalid notation, current side expected");
            }
            currentSide = side;
            if (splits[0][index++] != ' ') {
                throw std::invalid_argument("Invalid notation, space expected");
            }
            present = splits[0].SubString(index, ' ').ToInt();
            index = splits[0].Find(' ', index) + 1;
            whiteTimelinesCount = splits[0].SubString(index, ' ').ToInt();
            index = splits[0].Find(' ', index) + 1;
            blackTimelinesCount = splits[0].SubString(index).ToInt();
            for (u64 i = 1; i < splits.GetSize(); ++i) {
                const String& timelineInfo = splits[i];
                Vector<Board> boards;
                Vector<String> boardSplits = timelineInfo.Split('|');
                for (u64 j = 1; j < boardSplits.GetSize(); ++j) {
                    boards.Append(Board(boardSplits[j]));
                }
                if (boardSplits[0][0] == '+') {
                    positive0Index = i - 1;
                    timelines.Append(Timeline(boards, boardSplits[0].SubString(1).ToInt()));
                }
                else {
                    timelines.Append(Timeline(boards, boardSplits[0].ToInt()));
                }
            }
        }
        inline TimelineIndex CreateTimelineIndex(u32 local) const noexcept {
            i32 delta = static_cast<i32>(local) - positive0Index;
            if (delta >= 0) {
                return TimelineIndex(delta, TimelineIndexSign::Positive);
            }
            else {
                return TimelineIndex(twoOriginalTimelines ? delta + 1 : delta, TimelineIndexSign::Negative);
            }
        }
        inline Vector<Vector<Move>> FindLegalMoves() const {
            Vector<Vector<Move>> result(timelines.GetSize());
            for (u64 i = 0; i < timelines.GetSize(); ++i) {
                result[i].Reserve(timelines[i].BoardCount() * 160);
            }
            for (u32 l = 0; l < timelines.GetSize(); ++l) {
                TimelineIndex timelineIndex = CreateTimelineIndex(l);
                const Timeline& timeline = timelines[l];
                for (u32 t = timeline.GetStartTime(), tLocal = 0; tLocal < timelines[l].BoardCount(); ++t, ++tLocal) {
                    for (u8 x = 0; x < 8; ++x) {
                        for (u8 y = 0; y < 8; ++y) {
                            Piece pieceToMove = timelines[l][tLocal].Index(x, y);
                            if (pieceToMove.GetSide() != currentSide) {
                                continue;
                            }
                            if (pieceToMove.GetPieceType() == PieceType::Pawn) {
                                bool isWhite = pieceToMove.GetSide() == Side::White;
                                FindLinearMoves(
                                    isWhite ? PiecesPositionDelta::WP : PiecesPositionDelta::BP,
                                    result[l],
                                    Position(x, y, t, timelineIndex),
                                    pieceToMove,
                                    false,
                                    isWhite ? y == 1 ? 2 : 1 : y == 6 ? 2 : 1
                                );
                                FindNonLinearMoves(
                                    isWhite ? PiecesPositionDelta::WPT : PiecesPositionDelta::BPT,
                                    result[l],
                                    Position(x, y, t, timelineIndex),
                                    pieceToMove,
                                    false
                                );
                            }
                            else if (pieceToMove.GetPieceType() == PieceType::Knight || pieceToMove.GetPieceType() == PieceType::King) {
                                FindNonLinearMoves(
                                    pieceToMove.GetPieceType() == PieceType::Knight ? PiecesPositionDelta::N : PiecesPositionDelta::KQ,
                                    result[l],
                                    Position(x, y, t, timelineIndex),
                                    pieceToMove
                                );
                            }
                            else if (pieceToMove.GetPieceType() != PieceType::None) {
                                FindLinearMoves(
                                    pieceToMove.GetPieceType() == PieceType::Queen ? PiecesPositionDelta::KQ : pieceToMove.GetPieceType() == PieceType::Rook ? PiecesPositionDelta::R : PiecesPositionDelta::B,
                                    result[l],
                                    Position(x, y, t, timelineIndex),
                                    pieceToMove
                                );
                            }
                        }
                    }
                }
            }
            return result;
        }
        inline void MakeMove(Move move) {
            bool sameTime = move.DeltaT() == 0;
            bool sameLine = move.DeltaL() == 0;
            if (sameTime && sameLine) {
                Timeline& timeline = IndexTimeline(move.GetFrom().GetL());
                Board newBoard(timeline[move.GetFrom().GetT()]);
                Piece pieceToMove = newBoard.Index(move.GetFrom().GetX(), move.GetFrom().GetY());
                newBoard.Index(move.GetFrom().GetX(), move.GetFrom().GetY()) = Piece();
                newBoard.Index(move.GetTo().GetX(), move.GetTo().GetY()) = pieceToMove;
                timeline.AppendBoard(TypeMove(newBoard));
            }
            else if (sameTime && move.GetTo().GetT() == present && IndexTimeline(move.GetTo().GetL()).LastBoardIndex() == move.GetTo().GetT()) {
                Timeline& timelineFrom = IndexTimeline(move.GetFrom().GetL());
                Timeline& timelineTo = IndexTimeline(move.GetTo().GetL());
                Board newBoardFrom(timelineFrom[move.GetFrom().GetT()]);
                Board newBoardTo(timelineTo[move.GetTo().GetT()]);
                Piece pieceToMove = newBoardFrom.Index(move.GetFrom().GetX(), move.GetFrom().GetY());
                newBoardFrom.Index(move.GetFrom().GetX(), move.GetFrom().GetY()) = Piece();
                newBoardTo.Index(move.GetTo().GetX(), move.GetTo().GetY()) = pieceToMove;
                timelineFrom.AppendBoard(TypeMove(newBoardFrom));
                timelineTo.AppendBoard(TypeMove(newBoardTo));
            }
            else {
                Timeline& timelineFrom = IndexTimeline(move.GetFrom().GetL());
                Timeline& timelineTo = IndexTimeline(move.GetTo().GetL());
                Board newBoardFrom(timelineFrom[move.GetFrom().GetT()]);
                Board newBoardTo(timelineTo[move.GetTo().GetT()]);
                Piece pieceToMove = newBoardFrom.Index(move.GetFrom().GetX(), move.GetFrom().GetY());
                newBoardFrom.Index(move.GetFrom().GetX(), move.GetFrom().GetY()) = Piece();
                newBoardTo.Index(move.GetTo().GetX(), move.GetTo().GetY()) = pieceToMove;
                timelineFrom.AppendBoard(TypeMove(newBoardFrom));
                bool movePresent = false;
                if (currentSide == Side::White) {
                    timelines.Append(Timeline(newBoardTo, move.GetTo().GetT() + 1));
                    ++whiteTimelinesCount;
                    movePresent = whiteTimelinesCount - blackTimelinesCount < 2;
                }
                else {
                    timelines.Prepend(Timeline(newBoardTo, move.GetTo().GetT() + 1));
                    ++positive0Index;
                    ++blackTimelinesCount;
                    movePresent = whiteTimelinesCount - blackTimelinesCount < 2;
                }
                if (movePresent) {
                    present = move.GetTo().GetT() + 1;
                }
            }
        }
        inline bool CanMove(Move move) {
            if (move.GetFrom().GetT() != IndexTimeline(move.GetFrom().GetL()).LastBoardIndex()) {
                return false;
            }
            return true;
        }
        inline void SubmitAndUpdateState() {
            bool presentAtCurrentSide = (currentSide == Side::White && (present & 1) == 0) || (currentSide == Side::Black && (present & 1) == 1);
            if (presentAtCurrentSide) {
                for (u32 i = 0; i < timelines.GetSize(); ++i) {
                    if (timelines[i].GetStartTime() + timelines[i].BoardCount() - 1 == present) {
                        throw std::logic_error("There is still a timeline at present is not moved");
                    }
                }
            }
            currentSide = SideFunctions::Opposite(currentSide);
            if (presentAtCurrentSide) {
                ++present;
            }
        }
        inline bool ContainMustMovedTimelines() const {
            bool presentAtCurrentSide = (currentSide == Side::White && (present & 1) == 0) || (currentSide == Side::Black && (present & 1) == 1);
            if (presentAtCurrentSide) {
                for (u32 i = 0; i < timelines.GetSize(); ++i) {
                    if (timelines[i].GetStartTime() + timelines[i].BoardCount() - 1 == present) {
                        return true;
                    }
                }
            }
            return false;
        }
        inline Vector<TimelineIndex> MustMovedTimelines() const {
            Vector<TimelineIndex> result;
            bool presentAtCurrentSide = (currentSide == Side::White && (present & 1) == 0) || (currentSide == Side::Black && (present & 1) == 1);
            if (presentAtCurrentSide) {
                for (u32 i = 0; i < timelines.GetSize(); ++i) {
                    if (timelines[i].GetStartTime() + timelines[i].BoardCount() - 1 == present) {
                        result.Append(CreateTimelineIndex(i));
                    }
                }
            }
            return result;
        }
        inline bool WillDoTimeTravel(Move move) const noexcept {
            return move.DeltaT() != 0 && move.GetTo().GetT() != present;
        }
        inline String ToNotation() const {
            String result;
            u64 reserveSize = 40;
            for (u64 i = 0; i < timelines.GetSize(); ++i) {
                reserveSize += timelines[i].BoardCount() * 80;
            }
            result.GetContainer().Reserve(reserveSize);
            result += twoOriginalTimelines ? '2' : '-';
            result += ' ';
            result += SideFunctions::ToChar(currentSide);
            result += ' ';
            result += String::FromInt(whiteTimelinesCount);
            result += ' ';
            result += String::FromInt(blackTimelinesCount);
            for (u64 i = 0; i < timelines.GetSize(); ++i) {
                const Timeline& timeline = timelines[i];
                result += '\n';
                if (i == positive0Index) {
                    result += '+';
                }
                result += String::FromInt(timeline.GetStartTime());
                for (u64 j = 0; j < timeline.BoardCount(); ++j) {
                    result += '|';
                    timeline[j].AppendNotationToString(result);
                }
            }
            return result;
        }
        inline bool TimelineExist(TimelineIndex index) const {
            if (index.IsPositive()) {
                return positive0Index + index.GetValue() < timelines.GetSize();
            }
            else {
                u32 negative0Index = twoOriginalTimelines ? positive0Index - 1 : positive0Index;
                return negative0Index - index.GetValue() < timelines.GetSize();
            }
        }
        inline Timeline& IndexTimeline(TimelineIndex index) {
            if (index.IsPositive()) {
                return timelines[positive0Index + index.GetValue()];
            }
            else {
                u32 negative0Index = twoOriginalTimelines ? positive0Index - 1 : positive0Index;
                return timelines[negative0Index - index.GetValue()];
            }
        }
        inline const Timeline& IndexTimeline(TimelineIndex index) const {
            if (index.IsPositive()) {
                return timelines[positive0Index + index.GetValue()];
            }
            else {
                u32 negative0Index = twoOriginalTimelines ? positive0Index - 1 : positive0Index;
                return timelines[negative0Index - index.GetValue()];
            }
        }
        inline Game& operator=(const Game& other) {
            timelines = other.timelines;
            twoOriginalTimelines = other.twoOriginalTimelines;
            positive0Index = other.positive0Index;
            currentSide = other.currentSide;
            present = other.present;
            whiteTimelinesCount = other.whiteTimelinesCount;
            blackTimelinesCount = other.blackTimelinesCount;
            return *this;
        }
        inline Game& operator=(Game&& other) noexcept {
            timelines = TypeMove(other.timelines);
            twoOriginalTimelines = other.twoOriginalTimelines;
            positive0Index = other.positive0Index;
            currentSide = other.currentSide;
            present = other.present;
            whiteTimelinesCount = other.whiteTimelinesCount;
            blackTimelinesCount = other.blackTimelinesCount;
            return *this;
        }
    };
    export const Game standardGame("- w 0 0 0\n+0|rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR KQkq -");
    export const Game twoTimelinesGame("2 w 0 0 0\n0|rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR KQkq -\n+0|rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR KQkq -");
    export const Game& GetGameByName(String name) {
        if (name == "standard") {
            return standardGame;
        }
        if (name == "twotimelines") {
            return twoTimelinesGame;
        }
        throw std::invalid_argument("Invalid game name");
    }
}