#ifndef AVL_H
#define AVL_H

#include "exceptions.h"
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include "my_vector.h"

using std::cout;
using std::endl;

template<class T>
my_vector<T> sliceVec(my_vector<T> vector, int start, int end) {
    if (end == -1 || end < start)
        return my_vector<T>();
    typename my_vector<T>::const_iterator first = vector.begin() + start;
    typename my_vector<T>::const_iterator last = vector.begin() + end + 1;
    my_vector<T> newVec(first, last);
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


    void inOrderAux(InnerAvlTree<T> *root, my_vector<T *> &vec) {
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

    InnerAvlTree<T> *getFather() {
        return father;
    }


    ~InnerAvlTree() {
        delete rightSon;
        delete leftSon;
    }

    explicit InnerAvlTree(const my_vector<T> &vector) : data(vector[vector.size() / 2]), height(0), father(nullptr) {
        int size = vector.size();
        if (size <= 0) {
            return;
        }
        my_vector<T> rightVec = sliceVec(vector, (size / 2) + 1, size - 1);
        rightSon = rightVec.empty() ? nullptr : new InnerAvlTree<T>(rightVec);
        int rightHeight = rightSon == nullptr ? -1 : rightSon->height;
        my_vector<T> leftVec = sliceVec(vector, 0, (size / 2) - 1);
        leftSon = leftVec.empty() ? nullptr : new InnerAvlTree<T>(leftVec);
        int leftHeight = leftSon == nullptr ? -1 : leftSon->height;
        height = std::max(leftHeight, rightHeight) + 1;
        if (rightSon != nullptr) {
            rightSon->father = this;
        }
        if (leftSon != nullptr) {
            leftSon->father = this;
        }

        updateHeight();
        assert(balanceFactor() < 2 && balanceFactor() > -2);
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


    T &getData() {
        return data;
    }

    int getHeight() const {
        return height;
    }


    void updateHeight() {
        int leftHeight = leftSon == nullptr ? -1 : leftSon->height;
        int rightHeight = rightSon == nullptr ? -1 : rightSon->height;
        height = std::max(leftHeight, rightHeight) + 1;
//        if (father != nullptr)
//            father->updateHeight();
//        assert(validate_height());
    }

private:

    int balanceFactor() const {
        int leftHeight = leftSon == nullptr ? -1 : leftSon->height;
        int rightHeight = rightSon == nullptr ? -1 : rightSon->height;
        return leftHeight - rightHeight;
    }

    InnerAvlTree<T> *Left_rotate() {
        if (rightSon == nullptr)return this;
        InnerAvlTree<T> *newRightSon = rightSon;
        newRightSon->father = father;
        if (father != nullptr) {
            father->rightSon == this ? father->rightSon = newRightSon : father->leftSon = newRightSon;
            father->updateHeight();
        }
        InnerAvlTree<T> *temp = newRightSon->leftSon;
        if (temp != nullptr)
            temp->father = this;
        father = newRightSon;
        newRightSon->leftSon = this;
        rightSon = temp;
        updateHeight();
        newRightSon->updateHeight();
        if (newRightSon->father != nullptr) {
            newRightSon->father->updateHeight();
        }
        return newRightSon;
    }

    InnerAvlTree<T> *Right_rotate() {
        if (leftSon == nullptr) return this;
        InnerAvlTree<T> *newLeftSon = leftSon;
        newLeftSon->father = father;
        if (father != nullptr) {
            father->leftSon == this ? father->leftSon = newLeftSon : father->rightSon = newLeftSon;
            father->updateHeight();
        }
        InnerAvlTree<T> *temp = newLeftSon->rightSon;
        if (temp != nullptr)
            temp->father = this;
        father = newLeftSon;
        newLeftSon->rightSon = this;
        leftSon = temp;
        updateHeight();
        newLeftSon->updateHeight();
        if (newLeftSon->father != nullptr)
            newLeftSon->father->updateHeight();
        return newLeftSon;

    }


public:
    void nullifyFather() {
        father = nullptr;
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

    InnerAvlTree<T> *internalFind_unique(std::unique_ptr<T> info) {
        if (data == *info) {
            return this;
        }
        if (*info > data) {
            if (rightSon != nullptr) {
                return rightSon->internalFind(std::move(info));
            }
            return nullptr;
        }
        assert(*info < data);
        return leftSon == nullptr ? nullptr : leftSon->internalFind(std::move(info));
    }

    InnerAvlTree<T> *balance() {
        int balanceF = balanceFactor();
        assert(std::abs(balanceF) <= 2);
        if (balanceF > 1) {
            assert(leftSon != nullptr);
            if (leftSon->balanceFactor() >= 0) {
                return Right_rotate();
            } else if (leftSon->balanceFactor() == -1) {
                leftSon = leftSon->Left_rotate();
                return Right_rotate();
            }
        }
        if (balanceF < -1) {
            assert(rightSon != nullptr);
            if (rightSon->balanceFactor() <= 0) {
                return Left_rotate();
            } else if (rightSon->balanceFactor() == 1) {
                rightSon = rightSon->Right_rotate();
                return Left_rotate();
            }
        }
        return this;
    }

    InnerAvlTree<T> *insert(T x) {
        if (x == data)
            throw AlreadyExist();
        if (x > data) {
            if (rightSon == nullptr) {
                rightSon = new InnerAvlTree<T>(x);
                rightSon->father = this;
                return rightSon;
            } else {
                return rightSon->insert(x);
            }
        } else if (x < data) {
            if (leftSon == nullptr) {
                leftSon = new InnerAvlTree<T>(x);
                leftSon->father = this;
                return leftSon;
            } else {
                return leftSon->insert(x);
            }
        }
        updateHeight();
        setMax();
    }

    InnerAvlTree<T> *insert_unique(std::unique_ptr<T> x) {
        T *info = x.get();
        if (*info == data)
            throw AlreadyExist();
        if (*info > data) {
            if (rightSon == nullptr) {
                rightSon = new InnerAvlTree<T>(std::move(x));
                rightSon->father = this;
                return rightSon;
            } else {
                return rightSon->insert_unique(std::move(x));
            }
        } else if (*info < data) {
            if (leftSon == nullptr) {
                leftSon = new InnerAvlTree<T>(std::move(x));
                leftSon->father = this;
                return leftSon;
            } else {
                return leftSon->insert_unique(std::move(x));
            }
        }
        updateHeight();
        setMax();
    }

    T &find(const T &info) {
        InnerAvlTree<T> *result = internalFind(info);
        if (result == nullptr)
            throw NotExist();
        return result->data;
    }

    my_vector<T *> inOrder() {
        my_vector<T *> vec;
        inOrderAux(this, vec);
        return vec;
    }

    InnerAvlTree<T> *remove(const T &info) {
        if (data == info) {
            auto next = rightSon;
            if (next == nullptr) {
                auto result = leftSon;
                leftSon = nullptr;
//                if (result != nullptr)
//                    result->father = father;
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

                InnerAvlTree<T> *fatherToUpdate = next->father;
                while (fatherToUpdate != nullptr && fatherToUpdate != this) {
                    fatherToUpdate->updateHeight();
                    fatherToUpdate = fatherToUpdate->father;
                }
                rightSon->father = next;
            }
            next->leftSon = leftSon;
            if (leftSon) {
                leftSon->father = next;
            }
            next->updateHeight();
            rightSon = nullptr;
            leftSon = nullptr;
            InnerAvlTree<T> *fatherToUpdate = next->father;
            while (fatherToUpdate != nullptr && fatherToUpdate != this && fatherToUpdate != next) {
                fatherToUpdate->updateHeight();
                fatherToUpdate = fatherToUpdate->balance();
                fatherToUpdate = fatherToUpdate->father;
            }
            next->father = father;
            return next;
        }
        if (data > info) {
            if (leftSon == nullptr)
                throw NotExist();
            auto newRoot = this->leftSon->remove(info);
            if (newRoot != leftSon)
                delete leftSon;
            leftSon = newRoot;
            if (leftSon != nullptr) {
                leftSon->updateHeight();
                leftSon->father = this;
                leftSon = leftSon->balance();
            }
            updateHeight();
            return balance();
        }
        assert (data < info);
        if (rightSon == nullptr)
            throw NotExist();
        auto newRoot = rightSon->remove(info);
        if (newRoot != rightSon)
            delete rightSon;
        rightSon = newRoot;
        if (rightSon != nullptr) {
            rightSon->updateHeight();
            rightSon->father = this;
            rightSon = rightSon->balance();
        }
        updateHeight();
        return balance();
    }

    void debugTree(int depth) {
        for (int i = 0; i < depth; ++i) {
            cout << "  ";
        }
        cout << data << endl;
        if (leftSon != nullptr) {
            for (int i = 0; i < depth; ++i) {
                cout << "  ";
            }
            cout << "Left son" << endl;
            leftSon->debugTree(depth + 1);
        }
        if (rightSon != nullptr) {
            for (int i = 0; i < depth; ++i) {
                cout << "  ";
            }
            cout << "Right son" << endl;
            rightSon->debugTree(depth + 1);
        }
    }

private:
    int calcHeight() const {
        int leftHeight = leftSon == nullptr ? -1 : leftSon->calcHeight();
        int rightHeight = rightSon == nullptr ? -1 : rightSon->calcHeight();
        return std::max(leftHeight, rightHeight) + 1;

    }

    void validate_pointers() const {
        if (father != nullptr)
            assert(father->leftSon == this || father->rightSon == this);
        if (rightSon != nullptr) {
            assert(rightSon->father == this);
            rightSon->validate_pointers();
        }
        if (leftSon != nullptr) {
            assert(leftSon->father == this);
            leftSon->validate_pointers();
        }
    }


public:

    bool notExists(InnerAvlTree<T> *check) const {
        assert(check != this);
        if (rightSon != nullptr)
            rightSon->notExists(check);
        if (leftSon != nullptr)
            leftSon->notExists(check);
        return true;
    }

    void validate_unique() {
        auto vec = inOrder();
        for (int i = 0; i < vec.size() - 1; ++i) {
            if (false == (*vec.at(i) < *vec.at(i + 1))) {
                debugTree(0);
                assert(false);
            }
        }
    }

    bool validate_height() {
        validate_unique();
        validate_pointers();
        return calcHeight() == height;
    }


    bool allBalanceFactorUnder1() const {
        bool right = true, left = true;
        if (rightSon != nullptr)
            right = rightSon->allBalanceFactorUnder1();
        if (leftSon != nullptr)
            left = leftSon->allBalanceFactorUnder1();
        return std::abs(balanceFactor()) <= 1 && right && left;
    }

    void getNLowest(int n, my_vector<T> *vec) {
        assert(vec->size() <= n);
        if (vec->size() == n)
            return;
        if (leftSon != nullptr)
            leftSon->getNLowest(n, vec);
        assert(vec->size() <= n);
        if (n == vec->size())
            return;
        vec->push_back(data);
        if (rightSon != nullptr)
            rightSon->getNLowest(n, vec);
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
        if (tree == nullptr) {
            tree = new InnerAvlTree<T>(x);
            return;
        }

        auto inserted = tree->insert(x);
        balance(inserted);
        assert(tree->validate_height());
        assert(tree->allBalanceFactorUnder1());
        tree->setMax();
    }


    void insert_unique(std::unique_ptr<T> x) {

        if (tree == nullptr) {
            tree = new InnerAvlTree<T>(std::move(x));
            return;
        }
        auto inserted = tree->insert_unique(std::move(x));
        balance(inserted);
        assert(tree->validate_height());
        assert(tree->allBalanceFactorUnder1());
        tree->setMax();
    }

    T &find(const T &info) {
        if (tree == nullptr)
            throw NotExist();
        return tree->find(info);
    }

    my_vector<T *> inOrder() {
        return tree == nullptr ? my_vector<T *>() : tree->inOrder();

    }

    void remove(const T &info) {
        if (tree == nullptr)
            throw NotExist();
        bool willRootBeDeleted = info == tree->getData();
        InnerAvlTree<T> *newTree = tree->remove(info);
        if (willRootBeDeleted) {
            assert(tree != newTree);
            assert(newTree == nullptr || newTree->notExists(tree));
            if (newTree != nullptr) {
                newTree->nullifyFather();
                newTree = newTree->balance();
            }
            delete tree;
        }
        tree = newTree;
//        balance(fatherinfo);
        if (tree != nullptr) {
            tree->updateHeight();
            tree->setMax();
        }
        assert(tree == nullptr || tree->validate_height());
        assert(tree == nullptr || tree->allBalanceFactorUnder1());
    }

    void recursiveAvl(const my_vector<T> &vector) {
        if (vector.empty())
            return;
        tree = new InnerAvlTree<T>(vector);
    }

    bool validateHeight() {
        return tree == nullptr ? true : tree->validate_height();
    }

    void balance(InnerAvlTree<T> *current) {
        assert(current != nullptr);
        while (current != nullptr) {
            current = current->balance();
            if (current->getFather() == nullptr) {
                break;
            }
            current = current->getFather();
            current->updateHeight();
        }
        tree = current;
        if (tree != nullptr) {
            assert(tree->validate_height());
            assert(tree->allBalanceFactorUnder1());
        }

    }

    my_vector<T> getNLowest(int n) {
        my_vector<T> vec;
        if (tree != nullptr)
            tree->getNLowest(n, &vec);
        return vec;
    }

    void debugTree() {
        if (tree == nullptr) {
            cout << "null" << endl;
            return;
        }
        tree->debugTree(0);
    }

};

#endif