
#include "library1.h"
#include "Game.h"

void *Init() {
    Game* DS;
    try {
        DS = new Game();
    } catch (std::bad_alloc &e) {
        return nullptr;
    }
    return (void *) DS;
}

StatusType AddGroup(void *DS, int GroupID) {
    if (DS == nullptr || GroupID <= 0) return INVALID_INPUT;
    try { ((Game *) DS)->AddGroup(GroupID); }
    catch (std::bad_alloc &e) { return ALLOCATION_ERROR; }
    catch (AlreadyExist &res) { return FAILURE; }
    return SUCCESS;
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int Level) {
    if (DS == nullptr || GroupID <= 0 || PlayerID <= 0 || Level < 0) return INVALID_INPUT;
    try { ((Game *) DS)->AddPlayer(PlayerID, GroupID, Level); }
    catch (std::bad_alloc &e) { return ALLOCATION_ERROR; }
    catch (AlreadyExist &res) { return FAILURE; }
    catch (NotExist &res) { return FAILURE; }
    return SUCCESS;
}

StatusType RemovePlayer(void *DS, int PlayerID) {
    if (DS == nullptr || PlayerID <= 0) return INVALID_INPUT;
    try { ((Game *) DS)->RemovePlayer(PlayerID); }
    catch (NotExist &res) { return FAILURE; }
    return SUCCESS;
}

StatusType ReplaceGroup(void *DS, int GroupID, int ReplacementID) {
    if (DS == nullptr || GroupID <= 0 || ReplacementID <= 0 || GroupID == ReplacementID)
        return INVALID_INPUT;
    try { ((Game *) DS)->ReplaceGroup(GroupID, ReplacementID); }
    catch (std::bad_alloc &e) { return ALLOCATION_ERROR; }
    catch (NotExist &res) { return FAILURE; }
    return SUCCESS;

}

StatusType IncreaseLevel(void *DS, int PlayerID, int LevelIncrease) {
    if (DS == nullptr || LevelIncrease <= 0 || PlayerID <= 0)
        return INVALID_INPUT;
    try { ((Game *) DS)->IncreaseLevel(PlayerID, LevelIncrease); }
    catch (std::bad_alloc &e) { return ALLOCATION_ERROR; }
    catch (NotExist &res) { return FAILURE; }
    return SUCCESS;
}

StatusType GetHighestLevel(void *DS, int GroupID, int *PlayerID) {
    if (DS == nullptr || PlayerID == nullptr || GroupID == 0)
        return INVALID_INPUT;
    try { *PlayerID = ((Game *) DS)->getHighestLevel(GroupID); }
    catch (NotExist &res) { return FAILURE; }
    return SUCCESS;
}

StatusType GetAllPlayersByLevel(void *DS, int GroupID, int **Players, int *numOfPlayers) {
    if (DS == nullptr || GroupID == 0 || Players == nullptr || numOfPlayers == nullptr)
        return INVALID_INPUT;
    try {
        my_vector<int> vec = ((Game *) DS)->GetAllPlayersByLevel(GroupID);
        int size = vec.size();
        *numOfPlayers = size;
        int *arr = (int *) malloc(sizeof(arr) * size);
        for(int i = 0; i < size; i++)
            arr[i] = vec.at(size - 1 - i);
        *Players = arr;
    }
    catch (NotExist &res) { return FAILURE; }
    catch (std::bad_alloc &e) { return ALLOCATION_ERROR; }
    return SUCCESS;
}

StatusType GetGroupsHighestLevel(void *DS, int numOfGroups, int **Players) {
    if (Players == nullptr || DS == nullptr)
        return INVALID_INPUT;
    try {
        my_vector<int> res = ((Game *) DS)->getGroupsHighestLevel(numOfGroups);
        int size = (int) res.size();
        int *arr = (int *) malloc(sizeof(int) * size);
        for (int i = 0; i < size; ++i) {
            arr[i] = res.at(i);
        }
        *Players = arr;
    }
    catch (NotEnoughGroups &x) {
        return FAILURE;
    }
    catch (InvalidInput &x) {
        return INVALID_INPUT;
    }
    catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

void Quit(void **DS) {
    delete (Game *) *DS;
    *DS = nullptr;
}

