export module Engine.Game.Timeline;

import Engine.Type.Number;
import Engine.Type.Util;
import Engine.Game.Board;
import Engine.Collection.Vector;

using namespace Engine::Type;
using namespace Engine::Game;
using namespace Engine::Collection;

namespace Engine::Game {
    export class Timeline {
    private:
        Vector<Board> boards;
        u32 startTime = 0;
        inline u32 LocalToGlobal(u32 local) const noexcept {
            return startTime + local;
        }
        inline u32 GlobalToLocal(u32 global) const noexcept {
            return global - startTime;
        }
    public:
        inline Timeline() noexcept {}
        inline Timeline(const Timeline& other) :
            boards(other.boards),
            startTime(other.startTime) {}
        inline Timeline(Timeline&& other) noexcept :
            boards(TypeMove(other.boards)),
            startTime(other.startTime) {}
        inline Timeline(const Board& copyFrom, u32 startTime) :
            boards(1, copyFrom),
            startTime(startTime) {}
        inline Timeline(const Vector<Board>& boards, u32 startTime) :
            boards(boards),
            startTime(startTime) {}
        inline u32 GetStartTime() const noexcept {
            return startTime;
        }
        inline u32 BoardCount() const noexcept {
            return boards.GetSize();
        }
        inline u32 LastBoardIndex() const noexcept {
            return startTime + BoardCount() - 1;
        }
        inline void AppendBoard(const Board& board) {
            boards.Append(board);
        }
        inline void AppendBoard(Board&& board) {
            boards.Append(TypeMove(board));
        }
        inline Timeline& operator=(const Timeline& other) {
            boards = other.boards;
            startTime = other.startTime;
            return *this;
        }
        inline Timeline& operator=(Timeline&& other) {
            boards = TypeMove(other.boards);
            startTime = other.startTime;
            return *this;
        }
        inline Board& operator[](u32 index) noexcept {
            return boards[index];
        }
        inline const Board& operator[](u32 index) const noexcept {
            return boards[index];
        }
    };
}