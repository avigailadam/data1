#include "game.h"
#include <iostream>
#include <vector>

using std::vector;
#define CATCH_NOT_EXIST(arg) { \
        try{(arg);             \
        assert(0);}            \
        catch(NotExist &e){}}

#define CATCH_ALREADY_EXIST(arg) { \
        try{(arg);             \
        assert(0);}            \
        catch(AlreadyExist &e){}}
#define ASSERT_EQUALS(e1, e2)  do {if ((e1) != (e2)) {std::cout << "EQUALITY FAILED; " << #e1 << " (left): " << (e1) << ", " << #e2 << " (right): " << (e2) << std::endl; exit(1);}} while(0)

template<class T>
vector<T> makeActual(vector<T *> origin) {
    vector<T> res;
    for (T *i: origin) {
        res.push_back(*i);
    }
    return res;
}

void printVec(vector<int> v) {
    for (int i: v) {
        std::cout << i << std::endl;
    }
}

int main() {
    Game g1;
    CATCH_NOT_EXIST(g1.RemovePlayer(1))
    g1.AddGroup(15);
    g1.AddPlayer(1, 15, 0);
    g1.IncreaseLevel(1, 2);
    g1.AddPlayer(2, 15, 1);
    g1.AddPlayer(4, 15, 2);
    g1.AddGroup(2);
    g1.AddGroup(4);
    CATCH_ALREADY_EXIST(g1.AddPlayer(1, 4, 1))
    g1.AddPlayer(3, 4, 1);
    CATCH_NOT_EXIST(g1.AddPlayer(4, 6, 1))
    CATCH_NOT_EXIST(g1.getHighestLevel(6))
    ASSERT_EQUALS(g1.getHighestLevel(15), 1);
    assert(g1.getHighestLevel(-1) == 1);
    g1.AddPlayer(5, 4, 6);
    assert(g1.getHighestLevel(-1) == 5);
    g1.AddGroup(5);
    g1.AddGroup(6);
    g1.AddPlayer(10, 5, 2);
    g1.AddPlayer(11, 5, 3);
    g1.AddPlayer(12, 5, 4);
    g1.AddPlayer(13, 5, 4);
    vector<int> best3 = g1.getGroupsHighestLevel(3);
    vector<int> expect = {5, 12, 1};
    assert(best3 == expect);
    g1.AddPlayer(9,5,4);
    expect = {5, 9, 1};
    best3 = g1.getGroupsHighestLevel(3);
    assert(best3 == expect);
    g1.ReplaceGroup(15, 4);
    vector<int> all = g1.GetAllPlayersByLevel(4);
    expect = {3,2,4,1, 5};//error, show just original 4
    assert(all == expect);
    g1.AddGroup(15);
    g1.AddPlayer(20,15,1);
    g1.AddGroup(30);
    g1.AddPlayer(21,30,2);
    g1.ReplaceGroup(30,15);
    expect = {20, 21};
    all = g1.GetAllPlayersByLevel(15);
    assert(expect == all);
}
