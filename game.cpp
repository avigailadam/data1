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
    PlayerById *player_by_id = new PlayerById(*group, level, playerID);
    playersTree.insert(player_by_id);
    levelsTree.insert(player_by_level);
    delete group1;
}

void Game::RemovePlayer(int PlayerID) {
    PlayerById *player_temp = new PlayerById(PlayerID);
    PlayerById *player_by_id = playersTree.find(player_temp);
    delete player_temp;
    Group &group = player_by_id->getGroup();
    PlayerLevel *temp = new PlayerLevel(player_by_id->getLevel(), player_by_id->getId());
    PlayerLevel *player_by_level = levelsTree.find(temp);
    delete temp;
    group.removePlayer(player_by_level);
    levelsTree.remove(player_by_level);
    playersTree.remove(player_by_id);
}

void Game::ReplaceGroup(int groupID, int replacementID) {
    Group *groupTmp = new Group(groupID);
    Group *srcGroup = groupsTree.find(groupTmp);
    delete groupTmp;
    groupTmp = new Group(replacementID);
    Group *repGroup = groupsTree.find(groupTmp);
    delete groupTmp;
    std::vector<PlayerLevel> merged = merge(srcGroup->getInorderLevel(), repGroup->getInorderLevel());
    AvlTree<PlayerLevel> *newGroupLevels = recursiveAvl(merged, nullptr);
    Group *newGroup = new Group(replacementID, newGroupLevels);
    groupsTree.remove(srcGroup);
    groupsTree.remove(repGroup);
    groupsTree.insert(newGroup);
    delete srcGroup;
    delete repGroup;
}

void Game::IncreaseLevel(int playerID, int levelIncrease) {
    if (levelIncrease < 0 || playerID <= 0)
        throw InvalidInput();
    PlayerById *player_temp = new PlayerById(playerID);
    PlayerById *player_by_id = playersTree.find(player_temp);
    delete player_temp;
    RemovePlayer(playerID);
    AddPlayer(playerID, player_by_id->getGroup().getId(), player_by_id->getLevel() + levelIncrease);
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

std::vector<PlayerLevel> Game::merge(std::vector<PlayerLevel> v1, std::vector<PlayerLevel> v2) {
    int size = v1.size() + v2.size();
    std::vector<PlayerLevel> res;
    std::vector<PlayerLevel>::iterator p1 = v1.begin();
    std::vector<PlayerLevel>::iterator p2 = v2.begin();
    while (p1 != v1.end() || p2 != v2.end()) {
        if (p1 != v1.end() && (p2 == v2.end() || *p1 < *p2)) {
            res.push_back(*p1);
            p1++;
            continue;
        }
        res.push_back(*p2);
        p2++;
    }
    return res;
}

AvlTree<PlayerLevel> *Game::recursiveAvl(std::vector<PlayerLevel> vector, AvlTree<PlayerLevel> *father) {
    int size = vector.size();
    if (size <= 0) {
        return nullptr;
    }
    AvlTree<PlayerLevel> *res = new AvlTree<PlayerLevel>;
    res->setData(&vector[size / 2]);
    res->setFather(father);
    res->setRightSon(recursiveAvl(sliceVec(vector, (size / 2) + 1, size - 1), res));
    res->setLeftSon(recursiveAvl(sliceVec(vector, 0, (size / 2) - 1), res));
    return res;
}

std::vector<PlayerLevel> Game::sliceVec(std::vector<PlayerLevel> vector, int start, int end) {
    std::vector<PlayerLevel>::const_iterator first = vector.begin() + 100000;
    std::vector<PlayerLevel>::const_iterator last = vector.begin() + 101000;
    std::vector<PlayerLevel> newVec(first, last);
    return std::vector<PlayerLevel>();
}

