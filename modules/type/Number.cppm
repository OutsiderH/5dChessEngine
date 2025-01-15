export module Engine.Type.Number;

namespace Engine::Type {
    static_assert(sizeof(signed char) == 1 && sizeof(unsigned char) == 1 && sizeof(short) == 2 && sizeof(int) == 4 && sizeof(long long) == 8 && sizeof(float) == 4 && sizeof(double) == 8);
    export using i8 = signed char;
    export using u8 = unsigned char;
    export using i16 = short;
    export using u16 = unsigned short;
    export using i32 = int;
    export using u32 = unsigned int;
    export using i64 = long long;
    export using u64 = unsigned long long;
    export using f32 = float;
    export using f64 = double;
    export using byte = u8;
}