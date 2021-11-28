#ifndef AVL_H
#define AVL_H

#include "exceptions.h"
#include <vector>

template<class T>
class AvlTree;

class GetInfo {
public:
    int operator()(int x) {
        return x;
    }
};


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
        inOrderAux(root->rightSon,vec);
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

//        void roll_LL() {
//
//        }
//
//        void roll_LR() {
//
//        }
//
//        void roll_RR() {
//
//        }
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

    template<class get_info>
    void insert(T x, get_info getInfo) {
        if(size == 0){
            this->data = x;
            size += 1;
            return;
        }
        AvlTree<T> *current = this;
        AvlTree<T> *leaf = new AvlTree<T>(x, nullptr, nullptr, nullptr);
        leaf->size = 1;
        int thisInfo, otherInfo;
        do {
            thisInfo = getInfo(current->data);
            otherInfo = getInfo(leaf->data);
            if (thisInfo == otherInfo) {
                throw exceptions::AlreadyExist();
            }
            leaf->father = current;
            current = otherInfo < thisInfo ? current->leftSon : current->rightSon;
        } while (current != nullptr);
        if ((int) getInfo(leaf->data) > (int) getInfo(leaf->father->data))
            leaf->father->rightSon = leaf;
        else
            leaf->father->leftSon = leaf;
//            TODO: balance();
//            TODO: updateHeights();
//            TODO: updateSize();
    }

    template<class get_info>
    AvlTree<T> *find(T info, get_info getInfo) {
        AvlTree<T> *current = this;
        AvlTree<T> *leaf = new AvlTree<T>(info, nullptr, nullptr, nullptr);
        int thisInfo, otherInfo;
        do {
            thisInfo = getInfo(current->data);
            otherInfo = getInfo(leaf->data);
            if (thisInfo == otherInfo) {
                return current;
            }
            leaf->father = current;
            current = otherInfo < thisInfo ? current->leftSon : current->rightSon;
        } while (current != nullptr);
        return nullptr;

    }

    std::vector<T> inOrder() {
        std::vector<T> vec;
        inOrderAux(this, vec);
        return vec;
    }

    template<class U>
    friend void switchNodes(AvlTree<U> *node1, AvlTree<U> *node2);


    template<class get_info>
    void remove(T info, get_info getInfo) {
        AvlTree<T> *toRemove = find(info, getInfo);
        if (toRemove == nullptr)
            throw exceptions::NotExist();
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