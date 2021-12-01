#include "game.h"
#include <iostream>
#define CATCH_NOT_EXIST(arg) { \
        try{(arg);             \
        assert(0);}            \
        catch(NotExist &e){}}

#define CATCH_ALREADY_EXIST(arg) { \
        try{(arg);             \
        assert(0);}            \
        catch(AlreadyExist &e){}}
#define ASSERT_EQUALS(e1, e2)  do {if ((e1) != (e2)) {std::cout << "EQUALITY FAILED; " << #e1 << " (left): " << (e1) << ", " << #e2 << " (right): " << (e2) << std::endl; exit(1);}} while(0)

int main() {
    Game g1;
    CATCH_NOT_EXIST(g1.RemovePlayer(1))
    g1.AddGroup(15);
    g1.AddPlayer(1, 15, 0);
    g1.IncreaseLevel(1,2);
    g1.AddPlayer(2,15,1);
    g1.AddPlayer(4,15,2);
    g1.AddGroup(2);
    g1.AddGroup(4);
    CATCH_ALREADY_EXIST(g1.AddPlayer(1, 4, 1))
    g1.AddPlayer(3,4,1);
    CATCH_NOT_EXIST(g1.AddPlayer(4, 6, 1))
    CATCH_NOT_EXIST(g1.getHighestLevel(6))
    ASSERT_EQUALS(g1.getHighestLevel(15), 1);
    assert(g1.getHighestLevel(-1) == 1);
    g1.AddPlayer(5,4,6);
    assert(g1.getHighestLevel(-1) == 5);
    g1.ReplaceGroup(15,4);


}
