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

size_t lcg() {
    static size_t x = 0;
    x = (1021 * x + 24631) % 116640;
    return x;
}

template<typename Function>
double measureTime(Function&& func, int repetitions) {
    clock_t start, end;
    double totalTime = 0;

    for (int i = 0; i < repetitions; ++i) {
        start = clock();
        func();
        end = clock();
        totalTime += static_cast<double>(end - start) / CLOCKS_PER_SEC;
    }

    return totalTime / repetitions;
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    BinarySearchTree bst;
    double insertTime, searchTime, eraseTime;

    insertTime = measureTime([&]() {
        for (int i = 0; i < 100; ++i) { // 100 попыток заполнения
            bst = BinarySearchTree(); // Сброс дерева перед каждой попыткой
            for (int j = 0; j < 1000; ++j) {
                bst.insert(lcg());
            }
        }
        }, 100); // 100 попыток

    searchTime = measureTime([&]() {
        for (int i = 0; i < 1000; ++i) {
            bst.contains(lcg());
        }
        }, 1000); // 1000 попыток

    eraseTime = measureTime([&]() {
        for (int i = 0; i < 1000; ++i) {
            bst.erase(lcg());
        }
        }, 1000); // 1000 попыток

    std::cout << "BinarySearchTree: " << std::endl;
    std::cout << "Average Insertion Time: " << insertTime << " seconds" << std::endl;
    std::cout << "Average Search Time: " << searchTime << " seconds" << std::endl;
    std::cout << "Average Erase Time: " << eraseTime << " seconds" << std::endl;

    std::vector<int> vec;
    insertTime = measureTime([&]() {
        for (int i = 0; i < 100; ++i) {
            vec.clear(); // Очищаем вектор перед каждой попыткой
            for (int j = 0; j < 1000; ++j) {
                vec.push_back(lcg());
            }
        }
        }, 100); // 100 попыток

    searchTime = measureTime([&]() {
        for (int i = 0; i < 1000; ++i) {
            std::find(vec.begin(), vec.end(), lcg());
        }
        }, 1000); // 1000 попыток

    eraseTime = measureTime([&]() {
        for (int i = 0; i < 1000; ++i) {
            vec.erase(std::remove(vec.begin(), vec.end(), lcg()), vec.end());
        }
        }, 1000); // 1000 попыток

    std::cout << "std::vector<int>: " << std::endl;
    std::cout << "Average Insertion Time: " << insertTime << " seconds" << std::endl;
    std::cout << "Average Search Time: " << searchTime << " seconds" << std::endl;
    std::cout << "Average Erase Time: " << eraseTime << " seconds" << std::endl;

    return 0;
}