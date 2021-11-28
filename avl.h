#ifndef AVL_H
#define AVL_H

#include "exceptions.h"
#include <vector>

template<class T>
class AvlTree;

template<class T>
void switchNodes(AvlTree<T> *node1, AvlTree<T> *node2) {
    AvlTree<T> *tmp = node1->father;
    node1->father = node2->father;
    node2->father = tmp;
    tmp = node1->rightSon;
    node1->rightSon = node2->rightSon;
    node2->rightSon = tmp;
    tmp = node1->leftSon;
    node1->leftSon = node2->leftSon;
    node2->leftSon = tmp;
}

template<class T>
class AvlTree {
    T data;
    int height;
    int size;
    AvlTree<T> *father;
    AvlTree<T> *rightSon;
    AvlTree<T> *leftSon;


    void inOrderAux(AvlTree<T> *root, std::vector<T> &vec) {
        if (root == nullptr) return;
        inOrderAux(root->leftSon, vec);
        vec.push_back(root->data);
        inOrderAux(root->rightSon, vec);
    }

public:
    AvlTree() {
        father = nullptr;
        rightSon = nullptr;
        leftSon = nullptr;
        height = 0;
        size = 0;
    }

    AvlTree(T data, AvlTree<T> *rightSon, AvlTree<T> *leftSon, AvlTree<T> *father) : data(data), rightSon(rightSon),
                                                                                     leftSon(leftSon) {
        height = 0;
        size = 1;
    }

    virtual ~AvlTree() = default;


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
        AvlTree<T> *parent = this;
        AvlTree<T> *left_son = parent->leftSon;
        parent->leftSon = left_son->rightSon;
        parent->leftSon->father = parent;
        left_son->rightSon = parent;
        left_son->father = parent->father;
        parent->father = left_son;
        this = left_son;
        parent->height--;
    }

    void roll_LR() {
        AvlTree<T> *current = this;
        AvlTree<T> *left_son = current->leftSon;
        AvlTree<T> *right_grandson = current->leftSon;

    }

    void roll_RR() {
        AvlTree<T> *parent = this;
        AvlTree<T> *right_son = parent->rightSon;
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


    void insert(T x) {
        if (size == 0) {
            this->data = x;
            size += 1;
            return;
        }
        AvlTree<T> *current = this;
        AvlTree<T> *leaf = new AvlTree<T>(x, nullptr, nullptr, nullptr);
        leaf->size = 1;
        do {
            if (current->data == leaf->data) {
                throw AlreadyExist();
            }
            leaf->father = current;
            current = leaf->data > current->data ? current->rightSon : current->leftSon;
        } while (current != nullptr);
        if (leaf->data > leaf->father->data)
            leaf->father->rightSon = leaf;
        else
            leaf->father->leftSon = leaf;
//            TODO: balance();
//            TODO: updateHeights();
//            TODO: updateSize();
    }

private:
    AvlTree<T> *internal_find(T info) {
        AvlTree<T> *current = this;
        AvlTree<T> *leaf = new AvlTree<T>(info, nullptr, nullptr, nullptr);
        do {
            if (current->data == leaf->data) {
                return current;
            }
            leaf->father = current;
            current = leaf->data > current->data ? current->rightSon : current->leftSon;
        } while (current != nullptr);
        return nullptr;

    }

public:
    T& find(const T& info){
        AvlTree<T> *result = internal_find(info);
        if (result == nullptr)
            throw NotExist();
        return result->data;
    }
    
    std::vector<T> inOrder() {
        std::vector<T> vec;
        inOrderAux(this, vec);
        return vec;
    }

    template<class U>
    friend void switchNodes(AvlTree<U> *node1, AvlTree<U> *node2);

    void remove(T info) {
        AvlTree<T> *toRemove = internal_find(info);
        if (toRemove == nullptr)
            throw NotExist();
        AvlTree<T> *left = toRemove->leftSon;
        AvlTree<T> *right = toRemove->rightSon;
        AvlTree<T> *curr_father = toRemove->father;
        if (left != nullptr && right != nullptr) {
            AvlTree<T> *current = toRemove->rightSon;
            while (current->leftSon != nullptr) {
                current = current->leftSon;
            }
            switchNodes(current, toRemove);
            left = toRemove->leftSon;
            right = toRemove->rightSon;
        }
        if (left == nullptr && right == nullptr) {
            delete toRemove;
            return;
        }
        if (left == nullptr || right == nullptr) {
            AvlTree<T> *put = left == nullptr ? right : left;
            (curr_father->leftSon == toRemove ? curr_father->leftSon : curr_father->rightSon) = put;
            put->father = curr_father;
            delete toRemove;
            return;
        }
//            TODO: updateSize();
//            TODO: balance();


    }
};

#endif