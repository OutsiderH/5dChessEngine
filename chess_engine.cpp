import Engine.Game;
import Engine.Collection.String;

using namespace Engine::Game;
using namespace Engine::Collection;

int main() {
    Game test("w 0 0\n0|rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR KQkq -"_S);
    String test2 = test.ToNotation();
    return 0;
}
// notation example
// [current side] [amount of created timelines by white] [amount of created timelines by black]
// [start time of timeline]|[board state 0]|[board state 1]
// board state example
// [rank 8]/[rank 7]/[rank 6]/[rank 5]/[rank 4]/[rank 3]/[rank 2]/[rank 1] [castling abilities(KQkq)] [en passant target square]
