export module Engine.Game.Side;

import Engine.Type.Number;

using namespace Engine::Type;

namespace Engine::Game {
    export enum class Side : u8 {
        None,
        White,
        Black
    };
    export class SideFunctions {
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
        inline static constexpr Side FromChar(char c) noexcept {
            Side result = Side::None;
            switch (c) {
                case 'w':
                    result = Side::White;
                    break;
                case 'b':
                    result = Side::Black;
                    break;
                default:
                    break;
            }
            return result;
        }
        inline static constexpr bool TryFromChar(char c, Side& out_result) noexcept {
            switch (c) {
                case 'w':
                    out_result = Side::White;
                    return true;
                case 'b':
                    out_result = Side::Black;
                    return true;
                default:
                    out_result = Side::None;
                    return false;
            }
        }
    };
}