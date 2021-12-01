#include <cassert>
#include <iostream>
#include "avl.h"
#include "PlayerLevel.h"

using std::vector;
using std::cout;
using std::endl;

template<class T>
vector<T> makeActual(vector<T *> origin) {
    vector<T> res;
    for (T *i: origin) {
        res.push_back(*i);
    }
    return res;
}

void printVec(vector<PlayerLevel> v) {
    for (PlayerLevel i: v) {
        cout << i.getId() << " " << i.getLevel() << endl;
    }
}

int main() {
    PlayerLevel p1(0, 123);
    PlayerLevel p2(0, 232);
    PlayerLevel p3(1, 232);
    PlayerLevel p4(1, 235);
    PlayerLevel p5 = p4;
    PlayerLevel p6(3, 100);
    PlayerLevel p7(5, 110);
    AvlTree<PlayerLevel> t1;
    t1.insert(p1);
    t1.insert(p2);
    vector<PlayerLevel> expect = {p2, p1};
    vector<PlayerLevel> actual = makeActual(t1.inOrder());
    assert(expect == actual);
    try {
        t1.insert(p3);
        assert(0);
    } catch (AlreadyExist &x) {}
    t1.remove(p2);
    t1.insert(p3);
    expect = {p1, p3};
    actual = makeActual(t1.inOrder());
    assert(expect == actual);
    t1.insert(p4);
    try {
        t1.insert(p5);
        assert(0);
    }
    catch (AlreadyExist) {}
    t1.insert(p6);
    t1.insert(p7);
    expect = {p1, p4, p3, p6, p7};
    actual = makeActual(t1.inOrder());
    assert(expect == actual);
    t1.remove(p3);
    expect = {p1, p4, p6, p7};
    actual = makeActual(t1.inOrder());
    assert(expect == actual);
    t1.remove(p1);
    expect = {p4, p6, p7};
    vector<PlayerLevel*> tmp = t1.inOrder();
    actual = makeActual(tmp);
    assert(expect == actual);
    t1.remove(p7);
    t1.remove(p4);
    t1.remove(p6);
    assert(t1.isEmpty());


}