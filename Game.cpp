#include "Game.h"
#include "avl.h"


void Game::AddGroup(int groupID) {
    groupTree.insert_unique(std::unique_ptr<Group>(new Group(groupID)));
}

void Game::AddPlayer(int playerID, int groupID, int level) {
    Group *group = &groupTree.find(Group(groupID));
    Player player(playerID, level, group);
    PlayerById playerById(player);
    try {
        playersTree.find(playerById);
        throw AlreadyExist();
    } catch (NotExist &e) {}
    PlayerLevel playerByLevel(player, &playerById);
    bool thereIsPrev = true;
    if (group->getPlayersByLevel().isEmpty())
        thereIsPrev = false;
    int playerPrevId = thereIsPrev ? group->getPlayersByLevel().getMax().getId() : (-1);
    group->addPlayer(playerByLevel);
    playersTree.insert(playerById);
    levelsTree.insert(playerByLevel);
    PlayerLevel currMax = group->getPlayersByLevel().getMax();
    if (thereIsPrev && playerPrevId == currMax.getId())
        return;
    if (thereIsPrev)
        bestPlayersPerGroup.remove(BestPlayerByGroup(playerPrevId, groupID));
    bestPlayersPerGroup.insert(BestPlayerByGroup(currMax.getId(), groupID));
}

void Game::RemovePlayer(int playerId) {
    Group tmpGroup(1);
    Player tmpPlayer(playerId, 0, &tmpGroup);
    PlayerById tmpPlayerById(tmpPlayer);
    PlayerById playerById = playersTree.find(tmpPlayerById);
    PlayerLevel tmpLevel(tmpPlayerById.getPlayer());
    PlayerLevel playerByLevel = levelsTree.find(tmpLevel);
    Group *group = playerByLevel.getGroup();
    assert(!group->getPlayersByLevel().isEmpty());
    PlayerLevel prevMax = group->getPlayersByLevel().getMax();
    group->removePlayer(playerByLevel);
    playersTree.remove(playerById);
    levelsTree.remove(playerByLevel);
    if (group->getPlayersByLevel().isEmpty()) {
        bestPlayersPerGroup.remove(BestPlayerByGroup(prevMax.getId(), group->getId()));
        return;
    }
    PlayerLevel currMax = group->getPlayersByLevel().getMax();
    if (prevMax == currMax)
        return;
    bestPlayersPerGroup.remove(BestPlayerByGroup(prevMax.getId(), group->getId()));
    bestPlayersPerGroup.insert(BestPlayerByGroup(currMax.getId(), group->getId()));
}

void Game::ReplaceGroup(int groupID, int replacementID) {
    Group groupTmp(groupID);
    const Group &srcGroup = groupTree.find(groupTmp);
    Group groupTmp2(replacementID);
    Group &repGroup = groupTree.find(groupTmp2);
    std::vector<PlayerLevel *> srcVec = srcGroup.getInorderLevel();
    std::vector<PlayerLevel *> repVec = repGroup.getInorderLevel();
    std::vector<PlayerLevel> merged = merge(srcVec, repVec);
    BestPlayerByGroup srcBest(srcGroup.getPlayersByLevel().getMax().getId(), groupID);
    groupTree.remove(srcGroup);
    bestPlayersPerGroup.remove(srcBest);
    BestPlayerByGroup repBest(repGroup.getPlayersByLevel().getMax().getId(), replacementID);
    groupTree.remove(repGroup);
    bestPlayersPerGroup.remove(repBest);
    groupTree.insert_unique(std::unique_ptr<Group>(new Group(replacementID, merged)));
    Group &newGroup = groupTree.find(Group(replacementID));
    BestPlayerByGroup newBest(groupTree.find(groupTmp2).getPlayersByLevel().getMax().getId(), replacementID);
    bestPlayersPerGroup.insert(newBest);
    std::vector<PlayerLevel *> newPlayers = newGroup.getInorderLevel();
    for (PlayerLevel* p: newPlayers) {
        p->getPlayerById()->setGroup(&newGroup);
    }
    //todo: update players by id to new group
}

void Game::IncreaseLevel(int playerID, int levelIncrease) {
    Group groupTmp(3);
    Player playerTmp(playerID, -1, &groupTmp);
    PlayerById tmpById(playerTmp);
    PlayerById playerById = playersTree.find(tmpById);
    BestPlayerByGroup prevMax(*playerById.getGroup());
    RemovePlayer(playerID);
    AddPlayer(playerID, playerById.getGroup()->getId(), playerById.getLevel() + levelIncrease);
    BestPlayerByGroup newMax(*playerById.getGroup());
    if (prevMax == newMax)
        return;
    bestPlayersPerGroup.remove(prevMax);
    bestPlayersPerGroup.insert(newMax);
}

int Game::getHighestLevel(int groupID) {
    if (groupID < 0) {
        PlayerLevel player = levelsTree.getMax();
        return player.getId();
    }
    Group &group = groupTree.find(groupID);
    PlayerLevel groupPlayer = group.getMax();
    return groupPlayer.getId();
}

std::vector<int> Game::GetAllPlayersByLevel(int groupID) {
    if (groupID < 0) {
        std::vector<PlayerLevel *> vec = levelsTree.inOrder();
        std::vector<int> res = {};
        for (PlayerLevel *i : vec) {
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
    std::vector<BestPlayerByGroup> tmp = bestPlayersPerGroup.getNLowest(numOfGroups);
    if (tmp.size() != numOfGroups)
        throw NotEnoughGroups();
    std::vector<int> res;
    for (BestPlayerByGroup p: tmp) {
        res.push_back(p.getId());
    }
    return res;
}

std::vector<PlayerLevel> Game::merge(std::vector<PlayerLevel *> v1, std::vector<PlayerLevel *> v2) {
    std::vector<PlayerLevel> res;
    auto p1 = v1.begin();
    auto p2 = v2.begin();
    while (p1 != v1.end() || p2 != v2.end()) {
        if (p1 != v1.end() && (p2 == v2.end() || **p1 < **p2)) {
            res.push_back(**p1);
            p1++;
            continue;
        }
        res.push_back(**p2);
        p2++;
    }
    return res;
}





