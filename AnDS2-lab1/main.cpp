#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

class BinarySearchTree {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* copyTree(const Node* other) {
        if (!other)
            return nullptr;
        Node* newNode = new Node(other->key);
        newNode->left = copyTree(other->left);
        newNode->right = copyTree(other->right);
        return newNode;
    }

    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    bool insertNode(Node*& node, int key) {
        if (!node) {
            node = new Node(key);
            return true;
        }

        if (key < node->key)
            return insertNode(node->left, key);
        else if (key > node->key)
            return insertNode(node->right, key);

        return false; // Duplicate key, not inserted
    }

    bool containsNode(const Node* node, int key) const {
        if (!node)
            return false;

        if (key < node->key)
            return containsNode(node->left, key);
        else if (key > node->key)
            return containsNode(node->right, key);

        return true; // Key found
    }

    bool eraseNode(Node*& node, int key) {
        if (!node)
            return false;

        if (key < node->key)
            return eraseNode(node->left, key);
        else if (key > node->key)
            return eraseNode(node->right, key);
        else {
            if (!node->left && !node->right) {
                delete node;
                node = nullptr;
            }
            else if (!node->left) {
                Node* temp = node;
                node = node->right;
                delete temp;
            }
            else if (!node->right) {
                Node* temp = node;
                node = node->left;
                delete temp;
            }
            else {
                Node* minRight = findMin(node->right);
                node->key = minRight->key;
                eraseNode(node->right, minRight->key);
            }
            return true;
        }
    }

    Node* findMin(Node* node) const {
        while (node->left)
            node = node->left;
        return node;
    }

    void printTree(const Node* node) const {
        if (node) {
            printTree(node->left);
            std::cout << node->key << " ";
            printTree(node->right);
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}

    BinarySearchTree(const BinarySearchTree& other) : root(nullptr) {
        root = copyTree(other.root);
    }

    ~BinarySearchTree() {
        destroyTree(root);
    }

    BinarySearchTree& operator=(const BinarySearchTree& other) {
        if (this != &other) {
            destroyTree(root);
            root = copyTree(other.root);
        }
        return *this;
    }

    void print() const {
        printTree(root);
        std::cout << std::endl;
    }

    bool insert(int key) {
        return insertNode(root, key);
    }

    bool contains(int key) const {
        return containsNode(root, key);
    }

    bool erase(int key) {
        return eraseNode(root, key);
    }
};

