export module Engine.Analysis.MCTSNode;

import Engine.Type.Number;
import Engine.Type.Util;
import Engine.Game.Move;
import Engine.Game.TimelineIndex;
import Engine.Collection.Vector;
import Engine.Collection.MutiBranchTree;
import Engine.Game;

using namespace Engine::Type;
using namespace Engine::Collection;
using namespace Engine::Game;

namespace Engine::Analysis {
    export class MCTSNode {
        friend class MCTSTreeContainer;
    private:
        ::Game state;
        u32 visitCount = 0;
        f64 score = 0.0;
    public:
        inline explicit MCTSNode(const ::Game& state) :
            state(state) {}
    };
}