export module Engine.Collection.MutiBranchTree;

import Engine.Type.Number;
import Engine.Collection.Vector;

using namespace Engine::Type;
using namespace Engine::Collection;

namespace Engine::Collection {
    export template <typename ElementType>
    class MutiBranchTree {
    public:
        class Node {
            friend class MutiBranchTree;
        private:
            ElementType data;
            Node* parent = nullptr;
            Vector<Node*> children;
            inline Node(const ElementType& data) :
                data(data) {}
            inline Node(const ElementType& data, Node* parent) :
                data(data),
                parent(parent) {}
        public:
            inline ElementType& GetData() noexcept {
                return data;
            }
            inline const ElementType& GetData() const noexcept {
                return data;
            }
            inline Node* GetParent() noexcept {
                return parent;
            }
            inline const Node* GetParent() const noexcept {
                return parent;
            }
            inline bool IsLeaf() const noexcept {
                return children.Empty();
            }
            inline bool IsRoot() const noexcept {
                return parent == nullptr;
            }
            inline u64 Depth() const noexcept {
                if (IsRoot()) {
                    return 0;
                }
                return parent->Depth() + 1;
            }
            inline u64 BranchSize() const noexcept {
                u64 result = 0;
                for (u64 i = 0; i < children.GetSize(); ++i) {
                    result += children[i]->BranchSize();
                }
                return result;
            }
        };
    private:
        Node* root = nullptr;
        u64 size = 0;
    public:
        inline MutiBranchTree() noexcept {}
        inline MutiBranchTree(const MutiBranchTree& other) :
            size(other.size) {
            // not implemented
        }
        inline MutiBranchTree(MutiBranchTree&& other) noexcept :
            root(other.root),
            size(other.size) {
            other.root = nullptr;
        }
        inline MutiBranchTree(const ElementType& root) :
            root(new Node(root)), size(1) {}
        inline u64 GetSize() const noexcept {
            return size;
        }
        inline void AppendAt(Node* pos, const ElementType& value) {
            ++size;
            pos->children.Append(new Node(value, pos));
        }
    };
}