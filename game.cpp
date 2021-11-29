#include "game.h"
#include "avl.h"

void Game::AddGroup(int groupID) {
    Group *group = new Group(groupID);
    groupsTree.insert(group);
}

void Game::AddPlayer(int playerID, int groupID, int level) {
    if (level < 0 || groupID <= 0 || playerID <= 0)
        throw InvalidInput();
    PlayerLevel *player_by_level = new PlayerLevel(level, playerID);
    Group *group1 = new Group(groupID);
    Group *group = groupsTree.find(group1);
    group->addPlayer(player_by_level);
    PlayerById* player_by_id= new PlayerById(*group,level,playerID);
    playersTree.insert(player_by_id);
    levelsTree.insert(player_by_level);
    delete group1;
}

void Game::RemovePlayer(int PlayerID) {
    PlayerById* player_temp= new PlayerById(PlayerID);
    PlayerById* player_by_id= playersTree.find(player_temp);
    delete player_temp;
    Group& group= player_by_id->getGroup();
    PlayerLevel* temp= new PlayerLevel( player_by_id->getLevel(),player_by_id->getId());
    PlayerLevel* player_by_level= levelsTree.find(temp);
    delete temp;
    group.removePlayer(player_by_level);
    levelsTree.remove(player_by_level);
    playersTree.remove(player_by_id);
}

void Game::ReplaceGroup(int groupID, int replacementID) {

}

void Game::IncreaseLevel(int playerID, int levelIncrease) {


}

int Game::getHighestLevel(int groupID) {
    return 0;
}

std::vector<std::shared_ptr<PlayerById>> Game::GetAllPlayersByLevel(int groupID) {
    return std::vector<std::shared_ptr<PlayerById>>();
}

std::vector<std::shared_ptr<PlayerById>> Game::getGroupsHighestLevel(int numOfGroups) {
    return std::vector<std::shared_ptr<PlayerById>>();
}

