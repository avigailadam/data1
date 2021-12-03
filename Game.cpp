#include "Game.h"
#include "avl.h"


void Game::AddGroup(int groupID) {
    groupTree.insert_unique(std::unique_ptr<Group>(new Group(groupID)));
}

void Game::AddPlayer(int playerID, int groupID, int level) {
    Group *group = &groupTree.find(Group(groupID));
    Player player(playerID, level, group);
    PlayerById playerByIdTemp(player);

    playersTree.insert(playerByIdTemp);
    PlayerById &playerById = playersTree.find(playerByIdTemp);
    PlayerLevel playerByLevel(&playerById);
    bool thereIsPrev = group->getPlayersByLevel().isEmpty() == false;
    int playerPrevId = thereIsPrev ? group->getPlayersByLevel().getMax().getId() : (-1);
    group->addPlayer(playerByLevel);
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
    PlayerById &playerById = playersTree.find(tmpPlayerById);
    PlayerLevel tmpLevel(&playerById);
    PlayerLevel &playerByLevel = levelsTree.find(tmpLevel);
    Group *group = playerByLevel.getGroup();
    assert(!group->getPlayersByLevel().isEmpty());
    PlayerLevel prevMax = group->getPlayersByLevel().getMax();
    group->removePlayer(playerByLevel);
    levelsTree.remove(playerByLevel);
    if (group->getPlayersByLevel().isEmpty()) {
        bestPlayersPerGroup.remove(BestPlayerByGroup(prevMax.getId(), group->getId()));
        playersTree.remove(playerById);
        return;
    }
    PlayerLevel currMax = group->getPlayersByLevel().getMax();
    if (prevMax == currMax) {
        playersTree.remove(playerById);
        return;
    }
    bestPlayersPerGroup.remove(BestPlayerByGroup(prevMax.getId(), group->getId()));
    bestPlayersPerGroup.insert(BestPlayerByGroup(currMax.getId(), group->getId()));
    playersTree.remove(playerById);
}

void Game::ReplaceGroup(int groupID, int replacementID) {
    Group groupTmp(groupID);
    const Group &srcGroup = groupTree.find(groupTmp);
    Group groupTmp2(replacementID);
    Group &repGroup = groupTree.find(groupTmp2);
    if (srcGroup.isEmpty()) {
        groupTree.remove(srcGroup);
        return;
    }
    std::vector<PlayerLevel *> srcVec = srcGroup.getInorderLevel();
    std::vector<PlayerLevel *> repVec = repGroup.getInorderLevel();
    std::vector<PlayerLevel> merged = merge(srcVec, repVec);
    BestPlayerByGroup srcBest(srcGroup.getPlayersByLevel().getMax().getId(), groupID);
    groupTree.remove(srcGroup);
    bestPlayersPerGroup.remove(srcBest);
    if (!repGroup.isEmpty()) {
        BestPlayerByGroup repBest(repGroup.getPlayersByLevel().getMax().getId(), replacementID);
        bestPlayersPerGroup.remove(repBest);
    }
    groupTree.remove(repGroup);
    groupTree.insert_unique(std::unique_ptr<Group>(new Group(replacementID, merged)));
    Group &newGroup = groupTree.find(Group(replacementID));
    assert(!newGroup.isEmpty());
    BestPlayerByGroup newBest(groupTree.find(groupTmp2).getPlayersByLevel().getMax().getId(), replacementID);
    bestPlayersPerGroup.insert(newBest);
    std::vector<PlayerLevel *> newPlayers = newGroup.getInorderLevel();

    for (PlayerLevel *p: newPlayers) {
        assert(p != nullptr);
        PlayerById *pId = p->getPlayerById();
        assert(pId != nullptr);
        pId->setGroup(&newGroup);
    }
    //todo: update players by id to new group
}

void Game::IncreaseLevel(int playerID, int levelIncrease) {
    Group groupTmp(3);
    Player playerTmp(playerID, -1, &groupTmp);
    PlayerById tmpById(playerTmp);
    PlayerById &playerById = playersTree.find(tmpById);
    Group *group = playerById.getGroup();
    assert(group != nullptr);
    BestPlayerByGroup prevMax(*group);
    int level = playerById.getLevel();
    RemovePlayer(playerID);
    AddPlayer(playerID, group->getId(), level + levelIncrease);
    BestPlayerByGroup newMax(*group);
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
    if (group.isEmpty())
        return -1;
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
    if (numOfGroups == 0)
        throw InvalidInput();
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





