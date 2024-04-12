#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

#define SIZE 10

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
                Node* temp = node->right;
                delete node;
                return temp;
            }
            if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            else {
                Node* minRight = findMin(node->right);
                node->key = minRight->key;
                node->right = erase(node->right, minRight->key);
            }
            return node;
        }
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
        return erase(root, key);
    }

 
};

BinarySearchTree intersection(const BinarySearchTree& tree1, const BinarySearchTree& tree2) {
    BinarySearchTree result; // Создаем пустое дерево для результата

    // Добавляем элементы в результирующее дерево, если они присутствуют в обоих деревьях
    for (int i = 0; i <= 100; ++i) {
        if (tree1.contains(i) && tree2.contains(i))
            result.insert(i);
    }

    return result;
}

BinarySearchTree difference(const BinarySearchTree& tree1, const BinarySearchTree& tree2) {
    BinarySearchTree result = tree1; // Создаем копию первого дерева для результата

    // Удаляем элементы из результирующего дерева, если они присутствуют во втором дереве
    for (int i = 0; i <= 100; ++i) {
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

void testDiff() {
    BinarySearchTree tree1;
    BinarySearchTree tree2;

    tree1.insert(1);
    tree1.insert(3);
    tree1.insert(4);
    tree1.insert(5);

    tree2.insert(3);
    tree2.insert(4);
    tree2.insert(5);
    tree2.insert(6);
    tree2.insert(7);

    BinarySearchTree diffResult = difference(tree1, tree2);
    diffResult.print(); // Ожидаемый результат: 1
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
    intersectResult.print(); // Ожидаемый результат: 3, 4, 5
}

int main() {

    std::cout << "Intersection (Tree 1 & Tree 2): ";
    testDiff();
    std::cout << "Difference (Tree 1 - Tree 2): ";
    testIns();



    srand(static_cast<unsigned>(time(nullptr)));

    BinarySearchTree bst;
    double insertTime, searchTime, eraseTime;
    std::cout << "BinarySearchTree: " << std::endl;
    insertTime = measureTime([&]() {
            bst = BinarySearchTree();
            for (int j = 0; j < SIZE; ++j) {
                bst.insert(lcg());
            }
        }, 100);
    std::cout << "Average Insertion Time: " << insertTime << " seconds" << std::endl;

    searchTime = measureTime([&]() {
        for (int i = 0; i < SIZE; ++i) {
            bst.contains(lcg());
        }
        }, 1000);
    std::cout << "Average Search Time: " << searchTime << " seconds" << std::endl;

    eraseTime = measureTime([&]() {
        for (int i = 0; i < SIZE; ++i) {
            bst.erase(lcg());
        }
        }, 1000); 

    std::cout << "Average Erase Time: " << eraseTime << " seconds" << std::endl;

    std::cout << "std::vector<int>: " << std::endl;
    std::vector<int> vec;
    insertTime = measureTime([&]() {
            vec.clear();
            for (int j = 0; j < SIZE; ++j) {
                vec.push_back(lcg());
            }
        }, 100); 

    std::cout << "Average Insertion Time: " << insertTime << " seconds" << std::endl;

    searchTime = measureTime([&]() {
        for (int i = 0; i < SIZE; ++i) {
            std::find(vec.begin(), vec.end(), lcg());
        }
        }, 1000); 

    std::cout << "Average Search Time: " << searchTime << " seconds" << std::endl;

    eraseTime = measureTime([&]() {
        for (int i = 0; i < SIZE; ++i) {
            vec.erase(std::remove(vec.begin(), vec.end(), lcg()), vec.end());
        }
        }, 1000);

    std::cout << "Average Erase Time: " << eraseTime << " seconds" << std::endl;

    return 0;
}