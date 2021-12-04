#ifndef HW1_GAME_H
#define HW1_GAME_H

#include "avl.h"
#include "Players.h"
#include "BestPlayerByGroup.h"

class Game {
    AvlTree<PlayerById> playersTree;
    AvlTree<PlayerLevel> levelsTree;
    AvlTree<Group> groupTree;
    AvlTree<BestPlayerByGroup> bestPlayersPerGroup;

    my_vector<PlayerLevel> merge(my_vector<PlayerLevel *> v1, my_vector<PlayerLevel *> v2);

public:
    void AddGroup(int groupID);

    void AddPlayer(int playerID, int groupID, int level);

    void RemovePlayer(int PlayerID);

    void ReplaceGroup(int groupID, int replacementID);

    void IncreaseLevel(int playerID, int levelIncrease);

    int getHighestLevel(int groupID);

    my_vector<int> GetAllPlayersByLevel(int groupID);

    my_vector<int> getGroupsHighestLevel(int numOfGroups);
};


#endif //HW1_GAME_H
