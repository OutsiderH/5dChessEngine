module;

#include <stdexcept>

export module Engine.Collection.String;

import Engine.Type.Number;
import Engine.Type.Util;
import Engine.Collection.Vector;

using namespace Engine::Type;
using namespace Engine::Collection;

namespace Engine::Collection {
    export enum class IntStringFormat : u8 {
        BIN,
        OCT,
        DEC,
        HEXLower,
        HEXUpper
    };
    // todo:
    // move version for some methods
    export class String {
    private:
        static constexpr const char* digitsBIN = "01";
        static constexpr const char* digitsOCT = "01234567";
        static constexpr const char* digitsDEC = "0123456789";
        static constexpr const char* digitsHEXLower = "0123456789abcdef";
        static constexpr const char* digitsHEXUpper = "0123456789ABCDEF";
        Vector<char> container;
    public:
        inline static constexpr u32 FromIntLengthCheck(u32 value, IntStringFormat format = IntStringFormat::DEC) {
            u32 result = 0;
            u32 base = 10;
            switch (format) {
                case IntStringFormat::BIN:
                    base = 2;
                    break;
                case IntStringFormat::OCT:
                    base = 8;
                    break;
                case IntStringFormat::HEXLower:
                case IntStringFormat::HEXUpper:
                    base = 16;
                    break;
                case IntStringFormat::DEC:
                default:
                    break;
            }
            do {
                value /= base;
                ++result;
            } while (value > 0);
            return result;
        }
        inline static String FromInt(u32 value, IntStringFormat format = IntStringFormat::DEC) {
            String result;
            u32 base;
            const char* digits;
            switch (format) {
                case IntStringFormat::BIN:
                    base = 2;
                    digits = digitsBIN;
                    break;
                case IntStringFormat::OCT:
                    base = 8;
                    digits = digitsOCT;
                    break;
                case IntStringFormat::DEC:
                    base = 10;
                    digits = digitsDEC;
                    break;
                case IntStringFormat::HEXLower:
                    base = 16;
                    digits = digitsHEXLower;
                    break;
                case IntStringFormat::HEXUpper:
                    base = 16;
                    digits = digitsHEXUpper;
                    break;
                default:
                    throw std::invalid_argument("Invalid IntStringFormat");
            }
            u32 length = FromIntLengthCheck(value, format);
            result.container.Resize(length);
            for (u32 i = 0; i < length; ++i) {
                result.container[length - i - 1] = digits[value % base];
                value /= base;
            }
            return result;
        }
        inline String() :
            container(1, '\0') {}
        inline String(const String& other) :
            container(other.container) {}
        inline String(String&& other) noexcept :
            container(Move(other.container)) {}
        inline String(const char* str, u64 length) {
            container.Reserve(length + 1);
            for (u64 i = 0; i < length; ++i) {
                container.Append(str[i]);
            }
            container.Append('\0');
        }
        inline Vector<char>& GetContainer() noexcept {
            return container;
        }
        inline const Vector<char>& GetContainer() const noexcept {
            return container;
        }
        inline u32 Length() const noexcept {
            return container.GetSize() - 1;
        }
        inline bool Empty() const noexcept {
            return Length() == 0;
        }
        inline char* CStr() noexcept {
            return reinterpret_cast<char*>(container.GetData());
        }
        inline const char* CStr() const noexcept {
            return reinterpret_cast<const char*>(container.GetData());
        }
        inline u64 Find(char value, u64 from) const {
            return container.Find(value, from);
        }
        inline bool TryFind(char value, u64 from, u64& out_index) const noexcept {
            return container.TryFind(value, from, out_index);
        }
        inline String SubString(u64 start) const {
            if (start > Length()) {
                throw std::invalid_argument("SubString out of bounds");
            }
            return SubString(start, Length() - start);
        }
        inline String SubString(u64 start, u64 length) const {
            if (start + length > Length()) {
                throw std::invalid_argument("SubString out of bounds");
            }
            return String(&container[start], length);
        }
        inline String SubString(u64 start, char until) const {
            if (start > Length()) {
                throw std::invalid_argument("SubString out of bounds");
            }
            return String(&container[start], container.Find(until, start) - start);
        }
        template <typename DeferredSelfUsage = String>
        requires SameAs<DeferredSelfUsage, String>
        inline Vector<DeferredSelfUsage> Split(char separator) const {
            Vector<String> result;
            if (!Empty()) {
                u64 first = 0;
                for (u64 i = 0; i < Length(); ++i) {
                    if (container[i] == separator) {
                        result.Append(SubString(first, i - first));
                        first = i + 1;
                    }
                }
                result.Append(SubString(first, Length() - first));
            }
            return result;
        }
        inline u32 ToInt(IntStringFormat format = IntStringFormat::DEC) const {
            if (Empty()) {
                throw std::invalid_argument("String is empty");
            }
            u32 result = 0;
            u32 mult = 1;
            u32 base;
            const char* digits;
            switch (format) {
                case IntStringFormat::BIN:
                    base = 2;
                    digits = digitsBIN;
                    break;
                case IntStringFormat::OCT:
                    base = 8;
                    digits = digitsOCT;
                    break;
                case IntStringFormat::DEC:
                    base = 10;
                    digits = digitsDEC;
                    break;
                case IntStringFormat::HEXLower:
                    base = 16;
                    digits = digitsHEXLower;
                    break;
                case IntStringFormat::HEXUpper:
                    base = 16;
                    digits = digitsHEXUpper;
                    break;
                default:
                    throw std::invalid_argument("Invalid IntStringFormat");
            }
            for (u32 i = 0; i < Length(); ++i) {
                u32 digit = 0;
                while (digits[digit] != container[Length() - i - 1]) {
                    ++digit;
                    if (digit >= base) {
                        throw std::invalid_argument("Invalid character in number string");
                    }
                }
                result += digit * mult;
                mult *= base;
            }
            return result;
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
            result.container.InsertRange(Length(), right.container);
            return result;
        }
        inline String& operator+=(const String& right) {
            container.InsertRange(Length(), right.container);
            return *this;
        }
        inline String& operator+=(char right) {
            container.Insert(Length(), right);
            return *this;
        }
        inline char& operator[](u64 index) {
            return container[index];
        }
        inline const char& operator[](u64 index) const {
            return container[index];
        }
    };
    export inline String operator""_S(const char* str, u64 length) {
        return String(str, length);
    }
}