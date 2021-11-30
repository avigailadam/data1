#include <cassert>
#include <cstring>
#include "avl.h"

using std::string;

int main(){
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
    }
    expect = std::vector<string>();
    expect.push_back("aba");
    expect.push_back("hi");
    assert(actual == expect);

}