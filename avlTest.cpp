#include <iostream>
#include "avl.h"


int main() {
    AvlTree<int> tree;
    AvlTree<int> *treep = &tree;
    GetInfo g;
    tree.insert<GetInfo>(1, g);
    tree.insert<GetInfo>(3, g);
    tree.insert<GetInfo>(5, g);
    try {
        tree.insert<GetInfo>(3, g);
    }
    catch (...) {
        std::cout << "exists" << std::endl;
    }
    std::cout << (tree.find<GetInfo>(3, g))->getData() << std::endl;
    std::vector<int> v = tree.inOrder();
    std::cout<<v.size()<<std::endl;
    for (int i: v) {
        std::cout << i << std::endl;
    }
    tree.remove<GetInfo>(3, g);
    std::cout << ((tree.find<GetInfo>(3, g)) == nullptr ? "Melachim" : "Stumim") << std::endl;

}


