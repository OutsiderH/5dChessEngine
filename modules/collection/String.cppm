module;

#include <stdexcept>

export module Engine.Collection.String;

import Engine.Type.Number;
import Engine.Type.Util;
import Engine.Collection.Vector;

using namespace Engine::Type;
using namespace Engine::Collection;

export namespace Engine::Collection {
    enum class IntToStringFormat : u8 {
        BIN,
        OCT,
        DEC,
        HEXLower,
        HEXUpper
    };
    // todo:
    // move version for some methods
    class String {
    private:
        Vector<char> container;
    public:
        inline static constexpr u32 FromIntLengthCheck(u32 value, IntToStringFormat format = IntToStringFormat::DEC) {
            u32 result = 0;
            u32 base = 10;
            switch (format) {
                case IntToStringFormat::BIN:
                    base = 2;
                    break;
                case IntToStringFormat::OCT:
                    base = 8;
                    break;
                case IntToStringFormat::HEXLower:
                case IntToStringFormat::HEXUpper:
                    base = 16;
                    break;
                case IntToStringFormat::DEC:
                default:
                    break;
            }
            do {
                value /= base;
                ++result;
            } while (value > 0);
            return result;
        }
        inline static String FromInt(u32 value, IntToStringFormat format = IntToStringFormat::DEC) {
            constexpr const char* digitsBIN = "01";
            constexpr const char* digitsOCT = "01234567";
            constexpr const char* digitsDEC = "0123456789";
            constexpr const char* digitsHEXLower = "0123456789abcdef";
            constexpr const char* digitsHEXUpper = "0123456789ABCDEF";
            String result;
            u32 base;
            const char* digits;
            switch (format) {
                case IntToStringFormat::BIN:
                    base = 2;
                    digits = digitsBIN;
                    break;
                case IntToStringFormat::OCT:
                    base = 8;
                    digits = digitsOCT;
                    break;
                case IntToStringFormat::DEC:
                    base = 10;
                    digits = digitsDEC;
                    break;
                case IntToStringFormat::HEXLower:
                    base = 16;
                    digits = digitsHEXLower;
                    break;
                case IntToStringFormat::HEXUpper:
                    base = 16;
                    digits = digitsHEXUpper;
                    break;
                default:
                    throw std::invalid_argument("Invalid IntToStringFormat");
            }
            u32 length = FromIntLengthCheck(value, format);
            result.container.Resize(length);
            for (u32 i = 0; i < length; ++i) {
                result.container[length - i - 1] = digits[value % base];
                value /= base;
            }
            return result;
        }
        inline String() noexcept {}
        inline String(const String& other) :
            container(other.container) {}
        inline String(String&& other) noexcept :
            container(Move(other.container)) {}
        inline Vector<char>& GetContainer() noexcept {
            return container;
        }
        inline const Vector<char>& GetContainer() const noexcept {
            return container;
        }
        inline String& operator=(const String& other) {
            if (this != &other) {
                container = other.container;
            }
            return *this;
        }
        inline String& operator=(String&& other) noexcept {
            if (this != &other) {
                container = Move(other.container);
            }
            return *this;
        }
        inline String operator+(const String& right) {
            String result(*this);
            result.container.AppendRange(right.container);
            return result;
        }
        inline String& operator+=(const String& right) {
            container.AppendRange(right.container);
            return *this;
        }
        inline String& operator+=(char right) {
            container.Append(right);
            return *this;
        }
        inline char& operator[](u64 index) {
            return container[index];
        }
        inline const char& operator[](u64 index) const {
            return container[index];
        }
    };
}