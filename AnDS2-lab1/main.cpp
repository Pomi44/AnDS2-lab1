#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <chrono>

#define SIZE 100000

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

    Node* insert(Node* node, int key) {
        if (node == nullptr) {
            return new Node(key);
        }

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);

        return node;

    }

    bool contains(const Node* node, int key) const {
        if (node == nullptr)
            return false;

        if (key == node->key)
            return true;
        if (key < node->key)
            return contains(node->left, key);
        else
            return contains(node->right, key);
    }

    Node* erase(Node* node, int key) {
        if (node == nullptr)
            return nullptr;

        if (key < node->key)
            node->left = erase(node->left, key);
        else if (key > node->key)
            node->right = erase(node->right, key);
        else {
            if (node->left == nullptr) {
                Node* rightChild = node->right;
                delete node;
                return rightChild;
            }
            if (node->right == nullptr) {
                Node* leftChild = node->left;
                delete node;
                return leftChild;
            }
            Node* minRight = findMin(node->right);
            node->key = minRight->key;
            node->right = erase(node->right, minRight->key);
        }
        return node;
     }
    

    Node* findMin(Node* node) const {
        while (node->left)
            node = node->left;
        return node;
    }

    void printTree(Node* node) const {
        if (node != nullptr) {
            printTree(node->left);
            std::cout << node->key << " ";
            printTree(node->right);
        }
    }

    size_t size(Node* node) const {
        if (node == nullptr)
            return 0;
        return 1 + size(node->left) + size(node->right);
    }

public:
    BinarySearchTree() : root(nullptr) {}

    BinarySearchTree(const BinarySearchTree& other){
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
        if (contains(key))
            return false;
        root = insert(root, key);
        return true;
    }

    bool contains(int key) const {
        return contains(root, key);
    }

    bool erase(int key) {
        if (!contains(key))
            return false;
        root = erase(root, key);
        return true;
    }

    size_t size() const {
        return size(root);
    }
 
};

BinarySearchTree intersection(const BinarySearchTree& tree1, const BinarySearchTree& tree2) {
    BinarySearchTree result;

    for (int i = 0; i <= result.size(); ++i) {
        if (tree1.contains(i) && tree2.contains(i))
            result.insert(i);
    }

    return result;
}

BinarySearchTree difference(const BinarySearchTree& tree1, const BinarySearchTree& tree2) {
    BinarySearchTree result = tree1;
    for (int i = 0; i <= result.size(); ++i) {
        if (result.contains(i) && tree2.contains(i))
            result.erase(i);
    }

    return result;
}

size_t lcg() {
    static size_t x = 0;
    x = (1021 * x + 24631) % 116640;
    return x;
}


void testDiff() {
    BinarySearchTree tree1;
    BinarySearchTree tree2;

    tree1.insert(1);
    tree1.insert(2);
    tree1.insert(4);
    tree1.insert(5);

    tree2.insert(3);
    tree2.insert(4);
    tree2.insert(5);
    tree2.insert(6);
    tree2.insert(7);

    BinarySearchTree diffResult = difference(tree1, tree2);
    diffResult.print();
}

void testIns() {
    BinarySearchTree tree1;
    BinarySearchTree tree2;

    tree1.insert(1);
    tree1.insert(2);
    tree1.insert(3);
    tree1.insert(4);
    tree1.insert(5);

    tree2.insert(3);
    tree2.insert(4);
    tree2.insert(5);
    tree2.insert(6);
    tree2.insert(7);

    BinarySearchTree intersectResult = intersection(tree1, tree2);
    intersectResult.print();
}

template<typename Func>
double measureTime(Func create_bst, Func function, int repetitions) {
    double totalTime = 0.0;
    for (int i = 0; i < repetitions; ++i) {
        create_bst(); // 
        auto start = std::chrono::steady_clock::now();
        function();
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> duration = end - start;
        totalTime += duration.count();
    }
    return totalTime / repetitions;
}


int main() {

    std::cout << "Intersection (Tree 1 & Tree 2): ";
    testDiff();
    std::cout << std::endl;
    std::cout << "Difference (Tree 1 - Tree 2): ";
    testIns();
    std::cout << std::endl;

    srand(static_cast<unsigned>(time(nullptr)));
    std::cout << "Tree" << std::endl;
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 100; ++i) {
            BinarySearchTree tree;
            for (int j = 0; j < SIZE; ++j)
                tree.insert(lcg());
        }
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        std::cout << "Average fill time " << SIZE << " elements: "
            << std::chrono::duration <double, std::milli>(diff).count() / 100 << " ms\n";
    }
    {
        BinarySearchTree tree;
        for (int j = 0; j < SIZE; ++j)
            tree.insert(lcg());

        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 1000; ++i) {
            tree.contains(lcg());
        }
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        std::cout << "Average search time " << SIZE << " elements: "
            << std::chrono::duration <double, std::micro>(diff).count() / 1000 << " us\n";
    }
    {
        BinarySearchTree tree;
        for (int j = 0; j < SIZE; ++j)
            tree.insert(lcg());

        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 1000; ++i) {
            tree.insert(lcg());
            tree.erase(lcg());
        }
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        std::cout << "Average insertion and deletion " << SIZE << " elements: "
            << std::chrono::duration <double, std::micro>(diff).count() / 1000 << " us\n";
    }
    std::cout << std::endl;
    std::cout << "Vector" << std::endl;
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 100; ++i) {
            std::vector<int> vec;
            for (int j = 0; j < SIZE; ++j)
                vec.push_back(lcg());
        }
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        std::cout << "Average fill time " << SIZE << " elements: "
            << std::chrono::duration <double, std::milli>(diff).count() / 100 << " ms\n";
    }


    {
        std::vector<int> vec;
        for (int j = 0; j < SIZE; ++j)
            vec.push_back(lcg());

        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 1000; ++i) {
            std::find(vec.begin(), vec.end(), lcg());
        }
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        std::cout << "Average search time " << SIZE << " elements: "
            << std::chrono::duration <double, std::micro>(diff).count() / 1000 << " us\n";
    }


    {
        std::vector<int> vec;
        for (int j = 0; j < SIZE; ++j)
            vec.push_back(lcg());

        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 1000; ++i) {
            vec.push_back(lcg());
            vec.erase(std::remove(vec.begin(), vec.end(), lcg()), vec.end());
        }
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        std::cout << "Average insertion and deletion time " << SIZE << " elements: "
            << std::chrono::duration <double, std::micro>(diff).count() / 1000 << " us\n";
    }
    return 0;
}