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
    AvlTree<PlayerLevel>* recursiveAvl(std::vector<PlayerLevel> vector, AvlTree<PlayerLevel>* father);

    void AddGroup(int groupID);
    void AddPlayer(int playerID, int groupID, int level);
    void RemovePlayer(int PlayerID);
    void ReplaceGroup(int groupID, int replacementID);
    void IncreaseLevel(int playerID, int levelIncrease);
    int getHighestLevel(int groupID);
    std::vector<std::shared_ptr<PlayerById>> GetAllPlayersByLevel(int groupID);
    std::vector<std::shared_ptr<PlayerById>>  getGroupsHighestLevel(int numOfGroups);


    std::vector<PlayerLevel> sliceVec(std::vector<PlayerLevel> vector, int start, int end);
};


#endif //HW1_GAME_H
