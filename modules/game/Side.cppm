export module Engine.Game.Side;

import Engine.Type.Number;

using namespace Engine::Type;

export namespace Engine::Game {
    enum class Side : u8 {
        None,
        White,
        Black
    };
    class SideFunctions {
    public:
        inline static constexpr char ToChar(Side side) noexcept {
            char result = '?';
            switch (side) {
                case Side::White:
                    result = 'w';
                    break;
                case Side::Black:
                    result = 'b';
                    break;
                default:
                    break;
            }
            return result;
        }
    };
}