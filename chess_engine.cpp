import Engine.Type.Number;
import Engine.Game.Move;
import Engine.Game;
import Engine.Collection.Vector;
import Engine.Collection.String;
import Engine.Collection.MutiBranchTree;
import Engine.Analysis.MCTSNode;
import Engine.IO.Console;

using namespace Engine::Type;
using namespace Engine::Game;
using namespace Engine::Analysis;
using namespace Engine::Collection;
using namespace Engine::IO;

int main() {
    enum class State : u8 {
        Idle,
        Running
    } state = State::Idle;
    Game runningGame;
    while (true) {
        String input = Console::ReadLine();
        String command;
        Vector<String> args;
        if (input.Contains(' ')) {
            command += input.SubString(0, ' ');
            u64 currIndex = input.Find(' ') + 1;
            while (currIndex < input.Length()) {
                if (input[currIndex] == '"') {
                    if (!input.Contains('"', currIndex + 1)) {
                        args.Clear();
                        Console::WriteLine("Argument failed to parse, quote not closed");
                        break;
                    }
                    args.Append(input.SubString(currIndex + 1, '"'));
                    currIndex = currIndex + args.Last().Length() + 2;
                    continue;
                }
                else if (input[currIndex] == ' ') {
                    ++currIndex;
                    continue;
                }
                else {
                    if (input.Contains(' ', currIndex)) {
                        args.Append(input.SubString(currIndex, ' '));
                        currIndex = currIndex + args.Last().Length() + 1;
                    }
                    else {
                        args.Append(input.SubString(currIndex));
                        break;
                    }
                }
            }
        }
        else {
            command = input;
        }
        if (command == "help" || command == "h" || command == "?") {
            Console::WriteLine("Commands:");
            Console::WriteLine("    help, h, ?");
            Console::WriteLine("    hello");
            Console::WriteLine("    echo <...>");
            Console::WriteLine("    start [template name]");
            Console::WriteLine("    goodbye");
            Console::WriteLine("When running:");
            Console::WriteLine("    move [--b] [move notation]");
        }
        else if (command == "hello") {
            Console::WriteLine("Hello, world!");
        }
        else if (command == "echo") {
            if (args.Empty() || args[0] == "--help" || args[0] == "--h" || args[0] == "--?") {
                Console::WriteLine("Usage:");
                Console::WriteLine("    echo <args to echo...>");
                continue;
            }
            for (u64 i = 0; i < args.GetSize(); ++i) {
                Console::WriteLine(args[i]);
            }
        }
        else if (state == State::Idle) {
            if (command == "goodbye") {
                Console::WriteLine("Goodbye, cruel world!");
                break;
            }
            else if (command == "start") {
                if ((!args.Empty() && args[0] == "--help" || args[0] == "--h" || args[0] == "--?") || args.GetSize() > 1) {
                    Console::WriteLine("Usage:");
                    Console::WriteLine("    start [template name]");
                    Console::WriteLine("Templates:");
                    Console::WriteLine("    standard");
                    Console::WriteLine("    twotimelines");
                    continue;
                }
                runningGame = GetGameByName(args.Empty() ? "standard" : args[0]);
                state = State::Running;
            }
            else {
                Console::WriteLine("Unknown command, use 'help' for more information");
            }
        }
        else if (state == State::Running) {
            if (command == "move") {
    
            }
            else if (command == "d") {
                //
            }
            else {
                Console::WriteLine("Unknown command, use 'help' for more information");
            }
        }
        else {
            Console::WriteLine("Unknown command, use 'help' for more information");
        }
    }
    return 0;
}
