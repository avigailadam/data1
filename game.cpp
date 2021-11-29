#include "game.h"

void Game::AddGroup(int groupID) {
    Group* group = new Group(groupID);
    groupsTree.insert(group);
}

void Game::AddPlayer(int playerID, int groupID, int level) {
    PlayerLevel* player = new PlayerLevel(level, playerID);
    Group* group1 = new Group(groupID);
    Group* group = groupsTree.find(group1);
    group->addPlayer(player);
}

void Game::RemovePlayer(int PlayerID) {

}

void Game::ReplaceGroup(int groupID, int replacementID) {

}

void Game::IncreaseLevel(int playerID, int levelIncrease) {

}

int Game::getHighestLevel(int groupID) {
    return 0;
}

std::vector<std::shared_ptr<Player>> Game::GetAllPlayersByLevel(int groupID) {
    return std::vector<std::shared_ptr<Player>>();
}

std::vector<std::shared_ptr<Player>> Game::getGroupsHighestLevel(int numOfGroups) {
    return std::vector<std::shared_ptr<Player>>();
}

