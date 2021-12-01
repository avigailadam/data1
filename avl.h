#ifndef AVL_H
#define AVL_H

#include "exceptions.h"
#include <vector>
#include <cassert>
#include <cmath>

template<class T>
std::vector<T> sliceVec(std::vector<T> vector, int start, int end) {
    if (end == -1)
        return {};
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
        height = 0;
    }


    explicit InnerAvlTree(std::unique_ptr<T> x) : data(std::move(x)) {
        father = nullptr;
        rightSon = nullptr;
        leftSon = nullptr;
        max = this;
        height = 0;
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
        int rightHeight = rightSon == nullptr ? -1 : rightSon->height;
        std::vector<T> leftVec = sliceVec(vector, 0, (size / 2) - 1);
        leftSon = leftVec.empty() ? nullptr : new InnerAvlTree<T>(leftVec);
        int leftHeight = leftSon == nullptr ? -1 : leftSon -> height;
        height = std::max(leftHeight, rightHeight) + 1;
        if (rightSon != nullptr) {
            rightSon->father = this;
        }
        if (leftSon != nullptr) {
            leftSon->father = this;
        }
        //todo assert balance factor
        setMax();
    }

    void setMax() {
        max = this;
        while (max->rightSon != nullptr)
            max = max->rightSon;
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


    void setHeight() {
        int leftHeight = leftSon == nullptr ? -1 : leftSon->height;
        int rightHeight = rightSon == nullptr ? -1 : rightSon->height;
        height = std::max(leftHeight, rightHeight) + 1;
    }

private:

    int BalanceFactor() {
        int leftHeight = leftSon == nullptr ? -1 : leftSon->height;
        int rightHeight = rightSon == nullptr ? -1 : rightSon->height;
        return leftHeight - rightHeight;
    }

    void Left_rotate() {
        InnerAvlTree<T> *RightSon = rightSon;
        RightSon->father = father;
        InnerAvlTree<T> *temp = RightSon->leftSon;
        temp->father = this;
        father = RightSon;
        RightSon->leftSon = this;
        rightSon = temp;
        setHeight();
        RightSon->setHeight();
        leftSon = RightSon->leftSon;
        father = RightSon->father;
        data = RightSon->data;
        height = RightSon->height;
        max = RightSon->max;
    }

    void Right_rotate() {
        assert(leftSon != nullptr);
        InnerAvlTree<T> *originalLeftSon = leftSon;
        originalLeftSon->father = father;
        InnerAvlTree<T> *temp = originalLeftSon->rightSon;
        temp->father = this;
        father = originalLeftSon;
        originalLeftSon->rightSon = this;
        leftSon = temp;
        setHeight();
        originalLeftSon->setHeight();
        leftSon = originalLeftSon->leftSon;
        father = originalLeftSon->father;
        data = originalLeftSon->data;
        height = originalLeftSon->height;
        max = originalLeftSon->max;
    }

    void balance() {
        return;
        const int balanceFactor = BalanceFactor();
        if (balanceFactor > 1) {
            if (leftSon->BalanceFactor() >= 0) {
                Right_rotate();
                return;
            } else {
                leftSon->Left_rotate();
                Right_rotate();
                return;
            }
        }
        if (balanceFactor < -1) {
            if (rightSon->BalanceFactor() <= 0) {
                Left_rotate();
                return;
            } else {
                rightSon->Right_rotate();
                Left_rotate();
                return;
            }
        }
    }

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
        setHeight();
        balance();
        setMax();
    }

    void insert_unique(std::unique_ptr<T> x) {
        T *info = x.get();
        if (*info == data)
            throw AlreadyExist();
        if (*info > data) {
            if (rightSon == nullptr) {
                rightSon = new InnerAvlTree<T>(std::move(x));
                rightSon->father = this;
            } else
                rightSon->insert_unique(std::move(x));
        } else if (*info < data) {
            if (leftSon == nullptr) {
                leftSon = new InnerAvlTree<T>(std::move(x));
                leftSon->father = this;
            } else
                leftSon->insert_unique(std::move(x));
        }
        balance();
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
                balance();
                setHeight();
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
            balance();
            setHeight();
            return next;
        } else if (data > info) {
            if (leftSon == nullptr)
                throw NotExist();
            auto newRoot = this->leftSon->remove(info);
            if (newRoot != leftSon)
                delete leftSon;
            leftSon = newRoot;
            balance();
            setHeight();
            return this;
        } else {
            assert (data < info);
            if (rightSon == nullptr)
                throw NotExist();
            auto newRoot = rightSon->remove(info);
            if (newRoot != rightSon)
                delete rightSon;
            rightSon = newRoot;
            balance();
            setHeight();
            return this;
        }
    }
private:
    int calcHeight() const {
        int leftHeight = leftSon == nullptr ? -1 : leftSon -> calcHeight();
        int rightHeight = rightSon == nullptr ? -1 : rightSon -> calcHeight();
        return std::max(leftHeight, rightHeight) + 1;

    }
public:
    bool validate_height() const {
        if (leftSon == nullptr && rightSon == nullptr) {
            return height == 0;
        }
        if (leftSon == nullptr) {
            return height == rightSon->calcHeight() + 1 && rightSon->validate_height();
        }
        if (rightSon == nullptr) {
            return height == leftSon->calcHeight() + 1 && leftSon->validate_height();
        }
        int leftHeight = leftSon->calcHeight() + 1;
        int rightHeight = rightSon->calcHeight() + 1;
        int realHeight = leftHeight < rightHeight ? rightHeight : leftHeight;
        return realHeight == height && rightSon->validate_height() && leftSon->validate_height();
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
        if (tree != nullptr)
            tree->insert(x);
        else
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
            throw NotExist();
        InnerAvlTree<T> *newTree = tree->remove(info);
        if (newTree != tree)
            delete tree;
        tree = newTree;
        if (tree != nullptr)
            tree->setMax();
//        assert(tree == nullptr || tree->validate_height());
    }

    void recursiveAvl(const std::vector<T> &vector) {
        if (vector.empty())
            return;
        tree = new InnerAvlTree<T>(vector);
    }

    bool validateHeight() {
        return tree == nullptr ? true : tree->validate_height();
    }
};

#endif