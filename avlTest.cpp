#include <iostream>
#include "avl.h"


int main() {
    AvlTree<int> tree;
    AvlTree<int> *treep = &tree;
    int y = 1;
    int* x= &y;
    tree.insert(x);
    try {
        tree.insert(x);
    }
    catch (...) {
        std::cout << "exists" << std::endl;
    }
    std::cout << *(tree.find(x)) << std::endl;
    std::vector<int> v = tree.inOrder();
    std::cout<<v.size()<<std::endl;
    for (int i: v) {
        std::cout << i << std::endl;
    }
    tree.remove(x);
    std::cout << *(tree.find(x)) << std::endl;

}


