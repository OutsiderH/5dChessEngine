export module Engine.Collection.Map;

import Engine.Type.Number;
import Engine.Type.Util;
import Engine.Collection.Pair;
import Engine.Collection.Vector;

using namespace Engine::Type;

namespace Engine::Collection {
    export template <typename KeyType, typename ValueType>
    class Map {
    private:
        Vector<Pair<KeyType, ValueType>> data;
    public:

    };
}