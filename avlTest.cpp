#include <cassert>
#include <cstring>
#include <iostream>
#include "avl.h"

using std::string;
using std::vector;

class BoxToGal{
    int x;
public:
    BoxToGal(int x) : x(x) {}
    BoxToGal(std::unique_ptr<BoxToGal> other): x(other->x){}
    bool operator==(const BoxToGal& other){
        return x == other.x;
    }
    bool operator>(const BoxToGal& other){
        return x > other.x;
    }
    bool operator<(const BoxToGal& other){
        return x < other.x;
    }
};
template <class T>
vector<T> makeActual(vector<T *> origin) {
    vector<T> res;
    for (T *i: origin) {
        res.push_back(*i);
    }
    return res;
}
int main() {
    AvlTree<string> t1;
    assert(t1.inOrder().size() == 0);
    t1.insert("hi");
    std::vector<string> expect;
    expect.push_back("hi");
        std::vector<string> actual;
    for (auto s : t1.inOrder()) {
        actual.push_back(*s);
    }
    assert(actual == expect);
    t1.insert("mama");
    t1.insert("aba");
    assert(t1.validateHeight());
    expect = std::vector<string>();
    expect.push_back("aba");
    expect.push_back("hi");
    expect.push_back("mama");
    actual = std::vector<string>();
    for (auto s : t1.inOrder()) {
        actual.push_back(*s);
    }
    assert(actual == expect);
    assert(t1.getMax() == "mama");
    t1.remove("mama");
    assert(t1.validateHeight());
    actual = std::vector<string>();
    for (auto s : t1.inOrder()) {
        actual.push_back(*s);
    }
    expect = std::vector<string>();
    expect.push_back("aba");
    expect.push_back("hi");
    assert(actual == expect);
    assert(t1.getMax() == "hi");

    std::vector<int> toTree;
    for (int i = 0; i < 10; ++i) {
        toTree.push_back(i);
    }
    AvlTree<int> recTree;
    recTree.recursiveAvl(toTree);
    assert(recTree.validateHeight());
    std::vector<int> actualInt;
    for (auto s : recTree.inOrder()) {
        actualInt.push_back(*s);
    }
    assert(actualInt == toTree);
    assert(recTree.getMax() == 9);
//    std::vector<int> sliced = sliceVec(toTree,1,5);
//    for (int i = 1; i <= 5; i++) {
//        actualInt.push_back(i);
//    }
//    assert(actualInt == sliced);

    assert(recTree.find(3) == 3);
    try{
        recTree.find(10);
        assert(0);
    }
    catch (NotExist &x){

    }
    AvlTree<BoxToGal> box;
    BoxToGal b1(2);
    BoxToGal b2(4);
    std::unique_ptr<BoxToGal> sp(new BoxToGal(1));
    box.insert(b1);
    box.insert(b2);
    box.insert_unique(std::move(sp));
    box.remove(b1);

    AvlTree<string> foo;
    foo.insert("1");
    foo.insert("3");
    foo.insert("2");
    foo.insert("4");
    foo.insert("5");
    auto actualFoo = makeActual(foo.inOrder());
    vector<string> expectedFoo = {"1", "2", "3", "4", "5"};
    assert(actualFoo == expectedFoo);
    foo.remove("3");
    actualFoo = makeActual(foo.inOrder());
    expectedFoo = {"1", "2", "4", "5"};
    assert(actualFoo == expectedFoo);
    foo.remove("1");
    actualFoo = makeActual(foo.inOrder());
    expectedFoo = {"2", "4", "5"};
    assert(actualFoo == expectedFoo);
};