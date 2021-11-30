#include <cassert>
#include <cstring>
#include <iostream>
#include "avl.h"

using std::string;

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
    actual = std::vector<string>();
    for (auto s : t1.inOrder()) {
        actual.push_back(*s);
        std::cout << *s << std::endl;
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
    std::vector<int> actualInt;
    for (auto s : recTree.inOrder()) {
        actualInt.push_back(*s);
        std::cout << *s << std::endl;
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
    box.insert(std::move(sp));
    box.remove(b1);


};