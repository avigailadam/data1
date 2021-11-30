#ifndef AVL_H
#define AVL_H

#include "exceptions.h"
#include <vector>

template<class T>
class InnerAvlTree {
    T data;
    InnerAvlTree<T>* max;
    int height;
    int size;
    InnerAvlTree<T> *father;
    InnerAvlTree<T> *rightSon;
    InnerAvlTree<T> *leftSon;


    void inOrderAux(InnerAvlTree<T> *root, std::vector<T*> &vec) {
        if (root == nullptr) return;
        inOrderAux(root->leftSon, vec);
        vec.push_back(&(root->data));
        inOrderAux(root->rightSon, vec);
    }

    static std::vector<T> sliceVec(std::vector<T> vector, int start, int end) {
        typename std::vector<T>::const_iterator first = vector.begin() + start;
        typename std::vector<T>::const_iterator last = vector.begin() + end + 1;
        std::vector<T> newVec(first, last);
        return std::vector<T>();
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

    explicit InnerAvlTree(T x) : data(x) {
        father = nullptr;
        rightSon = nullptr;
        leftSon = nullptr;
        max = this;
        height = 1;
        size = 1;
    }


    explicit InnerAvlTree(std::unique_ptr<T> x) : data(std::move(x)) {
        father = nullptr;
        rightSon = nullptr;
        leftSon = nullptr;
        max = this;
        height = 1;
        size = 1;
    }

    ~InnerAvlTree(){
        delete rightSon;
        delete leftSon;
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

    explicit InnerAvlTree(const std::vector<T>& vector): data(vector[vector.size() / 2]), size(vector.size()), height(0), father(nullptr){
        int sizeVec = vector.size();
        if (sizeVec <= 0) {
            return;
        }
        std::vector<T> rightVec = sliceVec(vector, (size / 2) + 1, size - 1);
        rightSon = rightVec.empty()? nullptr :new InnerAvlTree<T>(rightVec);
        std::vector<T> leftVec = sliceVec(vector, 0, (size / 2) - 1);
        leftSon = leftVec.empty()? nullptr: new InnerAvlTree<T>(leftVec);
        int leftHeight = 0, rightHeight = 0;
        if (rightSon != nullptr) {
            rightSon->father = this;
            rightHeight = rightSon->height;
        }
        if (leftSon != nullptr) {
            leftSon->father = this;
            leftHeight = leftSon->height;
        }
        height = leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
        //todo set max
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

    void setMax() {
        InnerAvlTree<T> *current = this;
        while (current->rightSon != nullptr) {
            current = current->rightSon;
        }
        max = current;
    }

    InnerAvlTree<T> *getMax() const {
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

private:
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

    InnerAvlTree<T> *internalFind(const T &info) {
        if (size == 0) {
            return nullptr;
        }
        if (data == info) {
            return this;
        }
        if (rightSon)
        InnerAvlTree<T> *leftFind = leftSon->internalFind(info);
        return leftFind == nullptr ? rightSon->internalFind(info) : leftFind;
    }


public:

    void insert(T x) {
        InnerAvlTree<T> *current = this;
        InnerAvlTree<T> *leaf = new InnerAvlTree<T>(x);
        if (leaf== nullptr) throw AllocationError();
        leaf->size = 1;
        do {
            if (current->data == leaf->data) {
                delete leaf;
                throw AlreadyExist();
            }
            leaf->father = current;
            current = (leaf->data) > (current->data) ? current->rightSon : current->leftSon;
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

    void insert(std::unique_ptr<T> x) {
        InnerAvlTree<T> *current = this;
        InnerAvlTree<T> *leaf = new InnerAvlTree<T>(std::move(x));
        if (leaf== nullptr) throw AllocationError();
        leaf->size = 1;
        do {
            if (current->data == leaf->data) {
                delete leaf;
                throw AlreadyExist();
            }
            leaf->father = current;
            current = (leaf->data) > (current->data) ? current->rightSon : current->leftSon;
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

    T &find(const T &info) {
        InnerAvlTree<T> *result = internalFind(info);
        if (result == nullptr)
            throw NotExist();
        return result->data;
    }

    std::vector<T *> inOrder() {
        std::vector<T *> vec;
        inOrderAux(this, vec);
        return vec;
    }


    void remove(const T &info) {
        InnerAvlTree<T> *toRemove = internalFind(info);
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
    InnerAvlTree<T> *tree;

public:
    AvlTree() {
        tree = nullptr;
    }


    ~AvlTree() {
        delete tree;
    }

    T getMax() const{
        if (tree == nullptr)
            throw NotExist();
        InnerAvlTree<T>* maxNode = tree->getMax();
        return maxNode->getData();
    }

    int getSize() const {
        return tree != nullptr ? tree->getSize() : 0;
    }

    void insert(T x) {
        if (tree != nullptr)
            tree->insert(x);
        else
            tree = new InnerAvlTree<T>(x);
    }


    void insert(std::unique_ptr<T> x) {
        if (tree != nullptr)
            tree->insert(std::move(x));
        else
            tree = new InnerAvlTree<T>(std::move(x));
    }

    T &find(const T &info) {
        if(tree == nullptr)
            throw NotExist();
        return tree->find(info);
    }

    std::vector<T *> inOrder() {
        return tree == nullptr ? std::vector<T *>() : tree->inOrder();

    }

    void remove(const T &info) {
        if (tree != nullptr)
            tree->remove(info);
    }

    void recursiveAvl(const std::vector<T>& vector) {
        if(vector.empty())
            return;
        tree = new InnerAvlTree<T>(vector);
    }

};

#endif