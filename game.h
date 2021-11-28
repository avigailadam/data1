#ifndef HW1_GAME_H
#define HW1_GAME_H
#include "avl.h"
#include "player.h"
#include "group.h"

class Game{
    AvlTree<std::shared_ptr<Player>> playersTree;
    AvlTree<std::shared_ptr<Player>> levelsTree;
    AvlTree<Group> groupsTree;
public:
    Game();

    virtual ~Game();

    void AddGroup(int groupID);
    void AddPlayer(int playerID, int groupID, int level);
    void RemovePlayer(int PlayerID);
    void ReplaceGroup(int groupID, int replacementID);
    void IncreaseLevel(int playerID, int levelIncrease);
    void getHeighestLevel(int groupID, int* playerID);
    void GetAllPlayersByLevel(int groupID, int** players, int* numOfPlayers);
    void getGroupsHighestLevel(int numOfGroups, int **Players);


};


#endif //HW1_GAME_H
