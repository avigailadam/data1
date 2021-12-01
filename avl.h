#ifndef AVL_H
#define AVL_H

#include "exceptions.h"
#include <vector>

template<class T>
std::vector<T> sliceVec(std::vector<T> vector, int start, int end) {
    typename std::vector<T>::const_iterator first = vector.begin() + start;
    typename std::vector<T>::const_iterator last = vector.begin() + end + 1;
    std::vector<T> newVec(first, last);
    return newVec;
}


template<class T>
class InnerAvlTree {
    T data;
    InnerAvlTree<T> *max;
    int height;
    InnerAvlTree<T> *father;
    InnerAvlTree<T> *rightSon;
    InnerAvlTree<T> *leftSon;


    void inOrderAux(InnerAvlTree<T> *root, std::vector<T *> &vec) {
        if (root == nullptr) return;
        inOrderAux(root->leftSon, vec);
        vec.push_back(&(root->data));
        inOrderAux(root->rightSon, vec);
    }

public:

    explicit InnerAvlTree(T x) : data(x) {
        father = nullptr;
        rightSon = nullptr;
        leftSon = nullptr;
        max = this;
        height = 1;
    }


    explicit InnerAvlTree(std::unique_ptr<T> x) : data(std::move(x)) {
        father = nullptr;
        rightSon = nullptr;
        leftSon = nullptr;
        max = this;
        height = 1;
    }

    ~InnerAvlTree() {
        delete rightSon;
        delete leftSon;
    }

    explicit InnerAvlTree(const std::vector<T> &vector) : data(vector[vector.size() / 2]), height(0), father(nullptr) {
        int size = vector.size();
        if (size <= 0) {
            return;
        }
        std::vector<T> rightVec = sliceVec(vector, (size / 2) + 1, size - 1);
        rightSon = rightVec.empty() ? nullptr : new InnerAvlTree<T>(rightVec);
        std::vector<T> leftVec = sliceVec(vector, 0, (size / 2) - 1);
        leftSon = leftVec.empty() ? nullptr : new InnerAvlTree<T>(leftVec);
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
        //todo assert balance factor
        setMax();
    }

    void setMax() {
        InnerAvlTree<T> *current = this;
        while (current->rightSon != nullptr) {
            current = current->rightSon;
        }
        max = current;
    }

    InnerAvlTree<T> *getMax() const {
        assert(max != nullptr);
        return max;
    }


    T getData() const {
        return data;
    }

    int getHeight() const {
        return height;
    }

    bool exists(const T &info) const {
        if (info == data)
            return true;
        if (info < data) {
            return leftSon != nullptr && leftSon->exists(info);
        } else {
            assert(info > data);
            return rightSon != nullptr && rightSon->exists(info);
        }
    }

    void setHeight() {
        if (leftSon->height - rightSon->height > 0) {
            height = 1 + leftSon->height;
        } else {
            height = 1 + rightSon->height;
        }
    }

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
        if (data == info) {
            return this;
        }
        if (info > data) {
            if (rightSon != nullptr) {
                return rightSon->internalFind(info);
            }
            return nullptr;
        }
        assert(info < data);
        return leftSon == nullptr ? nullptr : leftSon->internalFind(info);
    }


public:

    void insert(T x) {
        if (x == data)
            throw AlreadyExist();
        if (x > data) {
            if (rightSon == nullptr) {
                rightSon = new InnerAvlTree<T>(x);
                rightSon->father = this;
            } else
                rightSon->insert(x);
        } else if (x < data) {
            if (leftSon == nullptr) {
                leftSon = new InnerAvlTree<T>(x);
                leftSon->father = this;
            } else
                leftSon->insert(x);
        }
        setMax();
    }

    void insert_unique(std::unique_ptr<T> x) {
        InnerAvlTree<T> *current = this;
        InnerAvlTree<T> *leaf = new InnerAvlTree<T>(std::move(x));
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

    InnerAvlTree<T> *remove(const T &info) {
        if (data == info) {
            auto next = rightSon;
            if (next == nullptr) {
                auto result = leftSon;
                leftSon = nullptr;
                return result;
            }
            while (next->leftSon != nullptr)
                next = next->leftSon;
            if (next != rightSon) {
                if (next->rightSon != nullptr) {
                    next->rightSon->father = next->father;
                }
                next->father->leftSon = next->rightSon;
                next->rightSon = rightSon;
            }
            next->leftSon = leftSon;
            if (leftSon) {
                leftSon->father = next;
            }
            rightSon = nullptr;
            leftSon = nullptr;
            next->father = father;
            return next;
        } else if (data > info) {
            if (leftSon == nullptr)
                throw NotExist();
            auto newRoot = this->leftSon->remove(info);
            if (newRoot != leftSon)
                delete leftSon;
            leftSon = newRoot;
            return this;
        } else {
            assert (data < info);
            if (rightSon == nullptr)
                throw NotExist();
            auto newRoot = rightSon->remove(info);
            if (newRoot != rightSon)
                delete rightSon;
            rightSon = newRoot;
            return this;
        }
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

    T getMax() const {
        if (tree == nullptr)
            throw NotExist();
        InnerAvlTree<T> *maxNode = tree->getMax();
        assert(maxNode != nullptr);
        return maxNode->getData();
    }

    bool isEmpty() const {
        return tree == nullptr;
    }

    void insert(T x) {
        if (tree != nullptr) {
            // TODO explain why we need this extra check here?
            if (tree->exists(x))
                throw AlreadyExist();
            tree->insert(x);
        } else
            tree = new InnerAvlTree<T>(x);
    }


    void insert_unique(std::unique_ptr<T> x) {
        if (tree != nullptr)
            tree->insert_unique(std::move(x));
        else
            tree = new InnerAvlTree<T>(std::move(x));
    }

    T &find(const T &info) {
        if (tree == nullptr)
            throw NotExist();
        return tree->find(info);
    }

    std::vector<T *> inOrder() {
        return tree == nullptr ? std::vector<T *>() : tree->inOrder();

    }

    void remove(const T &info) {
        if (tree == nullptr)
            return;
        InnerAvlTree<T> *newTree = tree->remove(info);
        if (newTree != tree)
            delete tree;
        tree = newTree;
        if (tree != nullptr)
            tree->setMax();
    }

    void recursiveAvl(const std::vector<T> &vector) {
        if (vector.empty())
            return;
        tree = new InnerAvlTree<T>(vector);
    }
};

#endif