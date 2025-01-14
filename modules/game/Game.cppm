export module Engine.Game;

import Engine.Type.Number;
import Engine.Type.Util;
import Engine.Game.Side;
import Engine.Game.Timeline;
import Engine.Collection.Vector;
import Engine.Collection.String;

using namespace Engine::Type;
using namespace Engine::Collection;

export namespace Engine::Game {
    // todo: introduce timeline index (+0, -0) must be considered
    class Game {
    private:
        Vector<Timeline> timelines;
        Side currentSide;
        u32 whiteTimelinesCount;
        u32 blackTimelinesCount;
    public:
        inline Game() noexcept {}
        inline Game(const Game& other) :
            timelines(other.timelines) {}
        inline Game(Game&& other) noexcept :
            timelines(Move(other.timelines)) {}
        inline explicit Game(const String& notation) {
            
        }
        inline String ToNotation() const {
            String result;
            u64 reserveSize = 8 + timelines.GetSize() * 4;
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
                    result += timeline[j].ToNotation();
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