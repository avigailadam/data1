#ifndef AVL_H
#define AVL_H

#include "exceptions.h"
#include <vector>
#include <cassert>

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
        setHeight();
//        height = leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
        balance();
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

    void setHeight() {

        int leftHeight = leftSon == nullptr ? -1 : leftSon->height;
        int rightHeight = rightSon == nullptr ? -1 : rightSon->height;
        if (leftHeight - rightHeight > 0) {
            height = 1 + leftHeight;
        } else {
            height = 1 + rightHeight;
        }
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
            InnerAvlTree<T> *LeftSon = leftSon;
            LeftSon->father = father;
            InnerAvlTree<T> *temp = LeftSon->rightSon;
            temp->father = this;
            father = LeftSon;
            LeftSon->rightSon = this;
            leftSon = temp;
            setHeight();
            LeftSon->setHeight();
            leftSon = LeftSon->leftSon;
            father = LeftSon->father;
            data = LeftSon->data;
            height = LeftSon->height;
            max = LeftSon->max;

        }

        void balance() {
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
                    rightSon->insert(std::move(x));
            } else if (*info < data) {
                if (leftSon == nullptr) {
                    leftSon = new InnerAvlTree<T>(std::move(x));
                    leftSon->father = this;
                } else
                    leftSon->insert(std::move(x));
            }
            balance();
            setMax();
        }
//    void insert_unique(std::unique_ptr<T> x) {
//        InnerAvlTree<T> *current = this;
//        InnerAvlTree<T> *leaf = new InnerAvlTree<T>(std::move(x));
//        do {
//            if (current->data == leaf->data) {
//                delete leaf;
//                throw AlreadyExist();
//            }
//            leaf->father = current;
//            current = (leaf->data) > (current->data) ? current->rightSon : current->leftSon;
//        } while (current != nullptr);
//        if (leaf->data > leaf->father->data)
//            leaf->father->rightSon = leaf;
//        else
//            leaf->father->leftSon = leaf;
//        setMax();
//    }

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
                return next;
            } else if (data > info) {
                if (leftSon == nullptr)
                    throw NotExist();
                auto newRoot = this->leftSon->remove(info);
                if (newRoot != leftSon)
                    delete leftSon;
                leftSon = newRoot;
                balance();
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