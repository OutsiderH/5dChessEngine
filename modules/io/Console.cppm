export module Engine.IO.Console;

import Engine.Type.Number;
import Engine.Collection.String;
import <Windows.h>;
import <stdexcept>;

using namespace Engine::Type;
using namespace Engine::Collection;

namespace Engine::IO {
    export class Console {
    public:
        inline static char Read() {
            char c;
            u32 actualCount;
            if (!ReadConsoleA(GetStdHandle(STD_INPUT_HANDLE), &c, 1, reinterpret_cast<LPDWORD>(&actualCount), nullptr)) {
                throw std::runtime_error("Failed to read console");
            }
            return c;
        }
        inline static String ReadLine() {
            String result;
            char buffer[256];
            u32 actualCount;
            if (!ReadConsoleA(GetStdHandle(STD_INPUT_HANDLE), &buffer, 256, reinterpret_cast<LPDWORD>(&actualCount), nullptr)) {
                throw std::runtime_error("Failed to read console");
            }
            for (u64 i = 0; i < actualCount; ++i) {
                if (buffer[i] == '\r') {
                    result += String(buffer, i);
                    return result;
                }
            }
            throw std::logic_error("Console buffer size has changed");
        }
        inline static void Write(const String& str) {
            u32 actualCount;
            if (!WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), str.CStr(), str.Length(), reinterpret_cast<LPDWORD>(&actualCount), nullptr)) {
                throw std::runtime_error("Failed to write console");
            }
        }
        inline static void WriteLine(const String& str) {
            Write(str + "\r\n");
        }
    };
}