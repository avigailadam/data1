#include "game.h"
#include "avl.h"

void Game::AddGroup(int groupID) {
    Group group(groupID);
    groupTree.insert(group);
}

void Game::AddPlayer(int playerID, int groupID, int level) {
    if (level < 0 || groupID <= 0 || playerID <= 0)
        throw InvalidInput();
    PlayerLevel playerByLevel(level, playerID);
    Group group1(groupID);
    Group& group = groupTree.find(group1);
    group.addPlayer(playerByLevel);
    PlayerById playerById(group, level, playerID);
    playersTree.insert(playerById);
    levelsTree.insert(playerByLevel);
}

void Game::RemovePlayer(int PlayerID) {
    Group tmp(1);
    PlayerById playerTemp(tmp, 0, PlayerID);
    PlayerById playerById = playersTree.find(playerTemp);
    Group &group = playerById.getGroup();
    PlayerLevel temp(playerById.getLevel(), playerById.getId());
    PlayerLevel playerByLevel = levelsTree.find(temp);
    group.removePlayer(playerByLevel);
    levelsTree.remove(playerByLevel);
    playersTree.remove(playerById);
}

void Game::ReplaceGroup(int groupID, int replacementID) {
    Group groupTmp(groupID);
    Group &srcGroup = groupTree.find(groupTmp);
    groupTmp = Group(replacementID);
    Group &repGroup = groupTree.find(groupTmp);
    std::vector<PlayerLevel> merged = merge(srcGroup.getInorderLevel(), repGroup.getInorderLevel());
    AvlTree<PlayerLevel> *newGroupLevels = recursiveAvl(merged, nullptr);
    Group newGroup(replacementID, newGroupLevels);
    groupTree.remove(srcGroup);
    groupTree.remove(repGroup);
    groupTree.insert(newGroup);
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

std::vector<PlayerLevel> Game::merge(std::vector<PlayerLevel*> v1, std::vector<PlayerLevel*> v2) {
    int size = v1.size() + v2.size();
    std::vector<PlayerLevel> res;
    auto p1 = v1.begin();
    auto p2 = v2.begin();
    while (p1 != v1.end() || p2 != v2.end()) {
        if (p1 != v1.end() && (p2 == v2.end() || *p1 < *p2)) {
            res.push_back(**p1);
            p1++;
            continue;
        }
        res.push_back(**p2);
        p2++;
    }
    return res;
}
//todo: move to avl.h
AvlTree<PlayerLevel> *Game::recursiveAvl(std::vector<PlayerLevel> vector, AvlTree<PlayerLevel> *father) {
    int size = vector.size();
    if (size <= 0) {
        return nullptr;
    }
    AvlTree<PlayerLevel> *res = new AvlTree<PlayerLevel>;
    res->setData(&vector.at(size / 2));
    res->setFather(father);
    res->setRightSon(recursiveAvl(sliceVec(vector, (size / 2) + 1, size - 1), res));
    res->setLeftSon(recursiveAvl(sliceVec(vector, 0, (size / 2) - 1), res));
    return res;
}

std::vector<PlayerLevel> Game::sliceVec(std::vector<PlayerLevel> vector, int start, int end) {
    std::vector<PlayerLevel>::const_iterator first = vector.begin() + start;
    std::vector<PlayerLevel>::const_iterator last = vector.begin() + end + 1;
    std::vector<PlayerLevel> newVec(first, last);
    return std::vector<PlayerLevel>();
}

