#include <iostream>
using namespace std;

// Maximum keys a node can hold (3) and children (4)
#define MAX_KEYS 3
#define MAX_CHILDREN 4

// Node structure for 2-3-4 Tree
struct Node {
    int keys[MAX_KEYS]; // Holds up to 3 keys
    Node* children[MAX_CHILDREN]; // Holds up to 4 children
    int numKeys; // Current number of keys
    bool isLeaf; // True if leaf node

    // Constructor
    Node(bool leaf) {
        numKeys = 0;
        isLeaf = leaf;
        for (int i = 0; i < MAX_CHILDREN; i++)
            children[i] = nullptr;
    }
};

// 2-3-4 Tree class
class TwoThreeFourTree {
private:
    Node* root;

    // Traverses the tree (Inorder)
    void inorder(Node* node) {
        if (!node) return;

        for (int i = 0; i < node->numKeys; i++) {
            // Visit left child before printing key
            inorder(node->children[i]);
            cout << node->keys[i] << " ";
        }
        // Visit last child
        inorder(node->children[node->numKeys]);
    }

    // Searches for a key in the tree
    Node* search(Node* node, int key) {
        if (!node) return nullptr;

        int i = 0;
        while (i < node->numKeys && key > node->keys[i])
            i++;

        if (i < node->numKeys && node->keys[i] == key)
            return node;

        return node->isLeaf ? nullptr : search(node->children[i], key);
    }

    // Splits a full node (3 keys) into two nodes and moves the middle key up
    void splitChild(Node* parent, int index) {
        Node* child = parent->children[index];
        Node* newChild = new Node(child->isLeaf);

        // Move rightmost key to new child
        newChild->keys[0] = child->keys[2];

        if (!child->isLeaf) {
            // Move rightmost children
            newChild->children[0] = child->children[2];
            newChild->children[1] = child->children[3];
        }

        // Reduce original child key count
        child->numKeys = 1;
        newChild->numKeys = 1;

        // Shift parent's children and insert new child
        for (int j = parent->numKeys; j > index; j--) {
            parent->children[j + 1] = parent->children[j];
            parent->keys[j] = parent->keys[j - 1];
        }

        parent->keys[index] = child->keys[1]; // Move middle key up
        parent->children[index + 1] = newChild;
        parent->numKeys++;
    }

    // Inserts a key into a non-full node
    void insertNonFull(Node* node, int key) {
        int i = node->numKeys - 1;

        if (node->isLeaf) {
            // Find position to insert key
            while (i >= 0 && key < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
            node->numKeys++;
        } else {
            // Find child to insert key
            while (i >= 0 && key < node->keys[i]) i--;

            if (node->children[i + 1]->numKeys == MAX_KEYS) {
                splitChild(node, i + 1);

                // Determine the correct child to insert into
                if (key > node->keys[i + 1]) i++;
            }
            insertNonFull(node->children[i + 1], key);
        }
    }

public:
    // Constructor
    TwoThreeFourTree() { root = nullptr; }

    // Public wrapper for insertion
    void insert(int key) {
        if (!root) {
            root = new Node(true);
            root->keys[0] = key;
            root->numKeys = 1;
            return;
        }

        if (root->numKeys == MAX_KEYS) {
            // If root is full, split it and create a new root
            Node* newRoot = new Node(false);
            newRoot->children[0] = root;
            splitChild(newRoot, 0);

            // Determine the child to insert into
            int i = (key > newRoot->keys[0]) ? 1 : 0;
            insertNonFull(newRoot->children[i], key);

            root = newRoot; // Update root
        } else {
            insertNonFull(root, key);
        }
    }

    // Public wrapper for search
    bool search(int key) {
        return search(root, key) != nullptr;
    }

    // Public wrapper for inorder traversal
    void inorder() {
        inorder(root);
        cout << endl;
    }
};

// Main function
int main() {
    TwoThreeFourTree tree;

    // Insert elements
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(60);
    tree.insert(70);

    cout << "Inorder Traversal (Sorted Order): ";
    tree.inorder();

    // Search elements
    int searchKey = 40;
    cout << "Searching for " << searchKey << ": " << (tree.search(searchKey) ? "Found" : "Not Found") << endl;

    return 0;
}
