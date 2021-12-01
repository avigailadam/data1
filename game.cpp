#include "game.h"
#include "avl.h"


void Game::AddGroup(int groupID) {
    groupTree.insert_unique(std::unique_ptr<Group>(new Group(groupID)));
}

void Game::AddPlayer(int playerID, int groupID, int level) {
    PlayerLevel playerByLevel(level, playerID);
    Group &group = groupTree.find(Group(groupID));
    group.addPlayer(playerByLevel);
    PlayerById playerById(group, level, playerID);
    playersTree.insert(playerById);
    levelsTree.insert(playerByLevel);
}

void Game::RemovePlayer(int PlayerID) {
    Group tmp(1);
    const PlayerById playerTemp(tmp, 0, PlayerID);
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
    const Group &srcGroup = groupTree.find(groupTmp);
    groupTmp = Group(replacementID);
    Group &repGroup = groupTree.find(groupTmp);
    std::vector<PlayerLevel> merged = merge(srcGroup.getInorderLevel(), repGroup.getInorderLevel());
    AvlTree<PlayerLevel> newGroupLevels;
    newGroupLevels.recursiveAvl(merged);
    groupTree.remove(srcGroup);
    groupTree.remove(repGroup);
    groupTree.insert_unique(std::unique_ptr<Group>(new Group(replacementID, &newGroupLevels)));
}

void Game::IncreaseLevel(int playerID, int levelIncrease) {
    Group group(0);
    PlayerById playerTemp(group, playerID, playerID);
    PlayerById playerById = playersTree.find(playerTemp);
    RemovePlayer(playerID);
    AddPlayer(playerID, playerById.getGroup().getId(), playerById.getLevel() + levelIncrease);
}

int Game::getHighestLevel(int groupID) {
    if (groupID < 0) {
        PlayerLevel player = levelsTree.getMax();
        return player.getLevel();
    }
    Group &group = groupTree.find(groupID);
    PlayerLevel groupPlayer = group.getMax();
    return groupPlayer.getLevel();
}

std::vector<int> Game::GetAllPlayersByLevel(int groupID) {
    if (groupID < 0) {
        std::vector<PlayerLevel *> vec = levelsTree.inOrder();
        std::vector<int> res = {};
        for (PlayerLevel* i : vec) {
            res.push_back(i->getId());
        }
        return res;
    }
    Group &group = groupTree.find(Group(groupID));
    std::vector<PlayerLevel *> vec = group.getInorderLevel();
    std::vector<int> res = {};
    for (PlayerLevel *i : vec) {
        res.push_back(i->getId());
    }
    return res;
}

std::vector<int> Game::getGroupsHighestLevel(int numOfGroups) {
    if (numOfGroups < 1)
        throw InvalidInput();
    int counter = 0;
    std::vector<int> res;
    std::vector<Group *> groups = groupTree.inOrder();
    for (Group *g: groups) {
        assert(g != nullptr);
        if (!(g->getPlayersByLevel().isEmpty())) {
            counter++;
            continue;
        }
        res.push_back(g->getPlayersByLevel().getMax().getId());
    }
    if (counter < numOfGroups)
        throw NotEnoughGroups();
    return res;
}

std::vector<PlayerLevel> Game::merge(std::vector<PlayerLevel *> v1, std::vector<PlayerLevel *> v2) {
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





