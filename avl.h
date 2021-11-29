#ifndef AVL_H
#define AVL_H

#include "exceptions.h"
#include <vector>

template<class T>
class InnerAvlTree {
    T data;
    InnerAvlTree<T> *max;
    int height;
    int size;
    InnerAvlTree<T> *father;
    InnerAvlTree<T> *rightSon;
    InnerAvlTree<T> *leftSon;


    void inOrderAux(InnerAvlTree<T> *root, std::vector<T> &vec) {
        if (root == nullptr) return;
        inOrderAux(root->leftSon, vec);
        vec.push_back(*(root->data));
        inOrderAux(root->rightSon, vec);
    }

    static void switchNodes(InnerAvlTree<T> *node1, InnerAvlTree<T> *node2) {
        InnerAvlTree<T> *tmp = node1->father;
        node1->father = node2->father;
        node2->father = tmp;
        tmp = node1->rightSon;
        node1->rightSon = node2->rightSon;
        node2->rightSon = tmp;
        tmp = node1->leftSon;
        node1->leftSon = node2->leftSon;
        node2->leftSon = tmp;
    }

public:
    InnerAvlTree() {
        father = nullptr;
        rightSon = nullptr;
        leftSon = nullptr;
        data = nullptr;
        max = nullptr;
        height = 0;
        size = 0;
    }

    explicit InnerAvlTree(T x) : data(x) {
        father = nullptr;
        rightSon = nullptr;
        leftSon = nullptr;
        max = nullptr;
        height = 1;
        size = 1;
    }

    void setData(T *data) {
        this->data = data;
    }

    void setSize(int size) {
        this->size = size;
    }

    void setFather(InnerAvlTree<T> *father) {
        this->father = father;
    }

    void setRightSon(InnerAvlTree<T> *rightSon) {
        this->rightSon = rightSon;
    }

    void setLeftSon(InnerAvlTree<T> *leftSon) {
        this->leftSon = leftSon;
    }

    InnerAvlTree(T *data, InnerAvlTree<T> *best_player, InnerAvlTree<T> *rightSon, InnerAvlTree<T> *leftSon,
                 InnerAvlTree<T> *father) :
            data(data),
            max(best_player),
            rightSon(rightSon),
            leftSon(leftSon),
            father(father) {
        height = 0;
        size = 1;
    }

    virtual ~InnerAvlTree() = default;

    void setMax() {
        InnerAvlTree<T> *current = this;
        while (current->rightSon != nullptr) {
            current = current->rightSon;
        }
        max = current;
    }

    InnerAvlTree<T> *getMax() {
        return max;
    }


    T getData() const {
        return data;
    }

    int getHeight() const {
        return height;
    }

    void setHeight() {
        if (leftSon->height - rightSon->height > 0) {
            height = 1 + leftSon->height;
        } else {
            height = 1 + rightSon->height;
        }
    }

    int getSize() const {
        return size;
    }
//
//        void updateSize() {
//
//        }

    int BF() {
        return leftSon->getHeight() - rightSon->getHeight();
    }

    void roll_LL() {
        InnerAvlTree<T> *parent = this;
        InnerAvlTree<T> *left_son = parent->leftSon;
        parent->leftSon = left_son->rightSon;
        parent->leftSon->father = parent;
        left_son->rightSon = parent;
        left_son->father = parent->father;
        parent->father = left_son;
        this = left_son;
        parent->height--;
    }

    void roll_LR() {
        InnerAvlTree<T> *current = this;
        InnerAvlTree<T> *left_son = current->leftSon;
        InnerAvlTree<T> *right_grandson = current->leftSon;

    }

    void roll_RR() {
        InnerAvlTree<T> *parent = this;
        InnerAvlTree<T> *right_son = parent->rightSon;
        parent->rightSon = right_son->leftSon;
        parent->rightSon->father = parent;
        right_son->leftSon = parent;
        right_son->father = parent->father;
        parent->father = right_son;
        this = right_son;
        parent->height--;
    }
//
//        void roll_RL() {
//
//        }

//        void balance() {
//            const int bf = BF();
//            switch (bf) {
//                case 2:
//                    if (leftSon->BF() >= 0)
//                        //            TODO: roll_LL();
//                    if (leftSon->BF() == -1)
//                        //            TODO: roll_LR();
//                    break;
//                case -2:
//                    if (leftSon->BF() <= 0)
//                        //            TODO: roll_RR();
//                    if (leftSon->BF() == 1)
//                        //            TODO: roll_RL();
//                    break;
//                default:
//                    break;
//            }
//        }

//        void updateHeights() {
//
//        }


    void insert(T *x) {
        if (size == 0) {
            this->data = x;
            size += 1;
            max = this;
            return;
        }
        InnerAvlTree<T> *current = this;
        InnerAvlTree<T> *leaf = new InnerAvlTree<T>(x, nullptr, nullptr, nullptr, nullptr);
        leaf->size = 1;
        do {
            if (current->data == leaf->data) {
                delete leaf;
                throw AlreadyExist();
            }
            leaf->father = current;
            current = *(leaf->data) > *(current->data) ? current->rightSon : current->leftSon;
        } while (current != nullptr);
        if (leaf->data > leaf->father->data)
            leaf->father->rightSon = leaf;
        else
            leaf->father->leftSon = leaf;
        setMax();
//            TODO: balance();
//            TODO: updateHeights();
//            TODO: updateSize();
    }

private:
    InnerAvlTree<T> *internal_find(T *info) {
        InnerAvlTree<T> *current = this;
        if (current->data == nullptr)
            return nullptr;
        do {
            if (*(current->data) == *(info)) {
                return current;
            }
            current = *(info) > *(current->data) ? current->rightSon : current->leftSon;
        } while (current != nullptr);
        return nullptr;

    }

public:
    T *find(T *info) {
        InnerAvlTree<T> *result = internal_find(info);
        if (result == nullptr)
            throw NotExist();
        return result->data;
    }

    std::vector<T*> inOrder() {
        std::vector<T*> vec;
        inOrderAux(this, vec);
        return vec;
    }


    void remove(T *info) {
        InnerAvlTree<T> *toRemove = internal_find(info);
        if (toRemove == nullptr)
            throw NotExist();
        InnerAvlTree<T> *left = toRemove->leftSon;
        InnerAvlTree<T> *right = toRemove->rightSon;
        InnerAvlTree<T> *curr_father = toRemove->father;
        if (left != nullptr && right != nullptr) {
            InnerAvlTree<T> *current = toRemove->rightSon;
            while (current->leftSon != nullptr) {
                current = current->leftSon;
            }
            switchNodes(current, toRemove);
            left = toRemove->leftSon;
            right = toRemove->rightSon;
        }
        if (left == nullptr && right == nullptr) {
            delete toRemove;
            setMax();
            return;
        }
        if (left == nullptr || right == nullptr) {
            InnerAvlTree<T> *put = left == nullptr ? right : left;
            (curr_father->leftSon == toRemove ? curr_father->leftSon : curr_father->rightSon) = put;
            put->father = curr_father;
            delete toRemove;
            setMax();
            return;
        }
//            TODO: updateSize();
//            TODO: balance();


    }
};

template<class T>
class AvlTree {
    InnerAvlTree<T> *data;

public:
    AvlTree() {
        data = nullptr;
    }

    ~AvlTree() {
        delete data;
    }

    // Returns nullptr on empty
    T *getMax() {
        return data != nullptr ? data->getMax() : nullptr;
    }

    int getSize() const {
        return data != nullptr ? data->getSize() : 0;
    }

    void insert(T x) {
        if (data != nullptr)
            data->insert(x);
        else
            data = new InnerAvlTree<T>(x);
    }

    T &find(const T &info) {
        return data == nullptr ? throw NotExist() : data->find(info);
    }

    std::vector<T*> inOrder() {
        return data == nullptr ? std::vector<T *>() : data->inOrder();

    }

    void remove(const T &info) {
        if (data != nullptr)
            data->remove(info);
    }
};

#endif