#ifndef HW1_GAME_H
#define HW1_GAME_H
#include "avl.h"
#include "PlayerById.h"
#include "group.h"
#include "PlayerLevel.h"

class Game{
    AvlTree<PlayerById> playersTree;
    AvlTree<PlayerLevel> levelsTree;
    AvlTree<Group> groupTree;

    std::vector<PlayerLevel> merge(std::vector<PlayerLevel*> v1, std::vector<PlayerLevel*> v2);
public:
    void AddGroup(int groupID);
    void AddPlayer(int playerID, int groupID, int level);
    void RemovePlayer(int PlayerID);
    void ReplaceGroup(int groupID, int replacementID);
    void IncreaseLevel(int playerID, int levelIncrease);
    int getHighestLevel(int groupID);
    std::vector<int> GetAllPlayersByLevel(int groupID);
    std::vector<PlayerById> getGroupsHighestLevel(int numOfGroups);
};


#endif //HW1_GAME_H
