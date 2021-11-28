#include <iostream>
#include "avl.h"


int main() {
    AvlTree<int> tree;
    AvlTree<int> *treep = &tree;
    tree.insert(1);
    tree.insert(3);
    tree.insert(5);
    try {
        tree.insert(3);
    }
    catch (...) {
        std::cout << "exists" << std::endl;
    }
    std::cout << (tree.find(3)) << std::endl;
    std::vector<int> v = tree.inOrder();
    std::cout<<v.size()<<std::endl;
    for (int i: v) {
        std::cout << i << std::endl;
    }
    tree.remove(3);
    std::cout << (tree.find(3)) << std::endl;

}


