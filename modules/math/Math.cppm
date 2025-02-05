export module Engine.Math;

import Engine.Type.Number;

using namespace Engine::Type;

namespace Engine::Math {
    export constexpr f64 infinityP = 1e+300 * 1e+300;
    export constexpr f64 infinityN = -1e+300 * 1e+300;
    export constexpr f64 nan = infinityP * 0.0;
    export constexpr f64 sqrt2 = 1.4142135623730951;
    export constexpr f64 ln2 = 0.6931471805599453;
    export inline f64 FastSqrt(f64 num) noexcept {
        if (num < 0.0) {
            return nan;
        }
        f64 half = num * 0.5;
        i64 bits = *reinterpret_cast<i64*>(&num);
        bits = 0x5FE6EC85E7DE30DA - (bits >> 1);
        f64 num1 = *reinterpret_cast<f64*>(&bits);
        num1 *= 1.5 - half * num1 * num1;
        num1 *= 1.5 - half * num1 * num1;
        return num * num1;
    }
    export inline f64 FastLn(f64 num) {
        if (num < 0.0) {
            return nan;
        }
        else if (num == 0.0) {
            return infinityN;
        }
        i64 bits = *reinterpret_cast<i64*>(&num);
        i64 bitsMantissa = bits & 0xFFFFFFFFFFFFF | 0x3FF0000000000000;
        i32 exponent = ((bits >> 52) & 0x7FF) - 1023;
        f64 mantissa = *reinterpret_cast<f64*>(&bitsMantissa);
        f64 num1 = (mantissa - 1) / (mantissa + 1);
        f64 num2 = num1 * num1;
        return (2.0 * num1 * (1.0 + num2 * (0.3333333333333333 + num2 * 0.2 + num2 * (0.1428571428571429 + num2 * 0.1111111111111111)))) + exponent * ln2;
    }
}