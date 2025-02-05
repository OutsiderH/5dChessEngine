export module Engine.Analysis.MCTSTreeContainer;

import Engine.Type.Number;
import Engine.Game;
import Engine.Analysis.MCTSNode;
import Engine.Collection.MutiBranchTree;
import Engine.Collection.Optional;
import Engine.Math;

using namespace Engine::Type;
using namespace Engine::Game;
using namespace Engine::Collection;
using namespace Engine::Math;

namespace Engine::Analysis
{
    export class MCTSTreeContainer {
    private:
        static constexpr f64 explorationFactor = 1.5;
        MutiBranchTree<MCTSNode> tree;
    public:
        inline f64 CalculateUTC(const MutiBranchTree<MCTSNode>::Node* node) const noexcept {
            const MCTSNode& data = node->GetData();
            if (data.visitCount == 0) {
                return Math::infinityP;
            }
            if (node->IsRoot()) {
                return data.score / data.visitCount;
            }
            return data.score / data.visitCount + explorationFactor * Math::FastSqrt(Math::FastLn(node->GetParent()->GetData().visitCount) / data.visitCount);
        }
        inline void Expand(MutiBranchTree<MCTSNode>::Node* at) {
            const MCTSNode& data = at->GetData();
            Vector<Vector<Move>> moves = data.state.FindLegalMoves();
            
        }
    };
}