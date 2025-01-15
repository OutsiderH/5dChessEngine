module;

#include <stdexcept>

export module Engine.Game;

import Engine.Type.Number;
import Engine.Type.Util;
import Engine.Game.Side;
import Engine.Game.Board;
import Engine.Game.Timeline;
import Engine.Collection.Vector;
import Engine.Collection.String;

using namespace Engine::Type;
using namespace Engine::Collection;

namespace Engine::Game {
    // todo: introduce timeline index (+0, -0) must be considered
    export class Game {
    private:
        Vector<Timeline> timelines;
        Side currentSide = Side::None;
        u32 whiteTimelinesCount = 0;
        u32 blackTimelinesCount = 0;
    public:
        inline Game() noexcept {}
        inline Game(const Game& other) :
            timelines(other.timelines) {}
        inline Game(Game&& other) noexcept :
            timelines(Move(other.timelines)) {}
        inline explicit Game(const String& notation) {
            const Vector<String> splits = notation.Split('\n');
            if (splits.GetSize() < 2) {
                throw std::invalid_argument("Invalid notation, insufficient lines");
            }
            Side side;
            u64 index = 0;
            if (!SideFunctions::TryFromChar(splits[0][index++], side)) {
                throw std::invalid_argument("Invalid notation, current side expected");
            }
            currentSide = side;
            if (splits[0][index++] != ' ') {
                throw std::invalid_argument("Invalid notation, space expected");
            }
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
                timelines.Append(Timeline(boards, boardSplits[0].ToInt()));
            }
        }
        inline String ToNotation() const {
            String result;
            u64 reserveSize = 40;
            for (u64 i = 0; i < timelines.GetSize(); ++i) {
                reserveSize += timelines[i].BoardCount() * 80;
            }
            result.GetContainer().Reserve(reserveSize);
            result += SideFunctions::ToChar(currentSide);
            result += ' ';
            result += String::FromInt(whiteTimelinesCount);
            result += ' ';
            result += String::FromInt(blackTimelinesCount);
            for (u64 i = 0; i < timelines.GetSize(); ++i) {
                const Timeline& timeline = timelines[i];
                result += '\n';
                result += String::FromInt(timeline.GetStartTime());
                result += '|';
                for (u64 j = 0; j < timeline.BoardCount(); ++j) {
                    timeline[j].AppendNotationToString(result);
                }
            }
            return result;
        }
        inline Game& operator=(const Game& other) {
            timelines = other.timelines;
            return *this;
        }
        inline Game& operator=(Game&& other) noexcept {
            timelines = Move(other.timelines);
            return *this;
        }
    };
}