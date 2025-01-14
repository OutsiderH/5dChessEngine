export module Engine.Type.Number;

export namespace Engine::Type {
    static_assert(sizeof(signed char) == 1 && sizeof(unsigned char) == 1 && sizeof(short) == 2 && sizeof(int) == 4 && sizeof(long long) == 8 && sizeof(float) == 4 && sizeof(double) == 8);
    using i8 = signed char;
    using u8 = unsigned char;
    using i16 = short;
    using u16 = unsigned short;
    using i32 = int;
    using u32 = unsigned int;
    using i64 = long long;
    using u64 = unsigned long long;
    using f32 = float;
    using f64 = double;
    using byte = u8;
}