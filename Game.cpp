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
    my_vector<PlayerLevel *> srcVec = srcGroup.getInorderLevel();
    my_vector<PlayerLevel *> repVec = repGroup.getInorderLevel();
    my_vector<PlayerLevel> merged = merge(srcVec, repVec);
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
    my_vector<PlayerLevel *> newPlayers = newGroup.getInorderLevel();

    for (int i = 0; i < newPlayers.size(); ++i) {
        auto p = newPlayers.at(i);
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
        if (levelsTree.isEmpty())
            return -1;
        PlayerLevel player = levelsTree.getMax();
        return player.getId();
    }
    Group &group = groupTree.find(groupID);
    if (group.isEmpty())
        return -1;
    PlayerLevel groupPlayer = group.getMax();
    return groupPlayer.getId();
}

my_vector<int> Game::GetAllPlayersByLevel(int groupID) {
    if (groupID < 0) {
        my_vector<PlayerLevel *> vec = levelsTree.inOrder();
        my_vector<int> res = {};
        for (int j = 0; j < vec.size(); ++j) {
            auto i = vec.at(j);
            res.push_back(i->getId());
        }
        return res;
    }
    Group &group = groupTree.find(Group(groupID));
    my_vector<PlayerLevel *> vec = group.getInorderLevel();
    my_vector<int> res = {};
    for (int j = 0; j < vec.size(); ++j) {
        auto i = vec.at(j);
        res.push_back(i->getId());
    }
    return res;
}

my_vector<int> Game::getGroupsHighestLevel(int numOfGroups) {
    if (numOfGroups < 1)
        throw InvalidInput();
    my_vector<BestPlayerByGroup> tmp = bestPlayersPerGroup.getNLowest(numOfGroups);
    if (tmp.size() != numOfGroups)
        throw NotEnoughGroups();
    my_vector<int> res;
    for (int i = 0; i < tmp.size(); ++i) {
        res.push_back(tmp.at(i).getId());
    }
    return res;
}

my_vector<PlayerLevel> Game::merge(my_vector<PlayerLevel *> v1, my_vector<PlayerLevel *> v2) {
    assert(v1.is_sorted_ptr());
    assert(v2.is_sorted_ptr());
    my_vector<PlayerLevel> res;
    auto i1 = 0;
    auto i2 = 0;
    while (i1 < v1.size() || i2 < v2.size()) {
        if (i1 != v1.size() && (i2 == v2.size() || *v1.at(i1) < *v2.at(i2))) {
            PlayerLevel *pLevel = v1.at(i1);
            assert(pLevel != nullptr);
            res.push_back(*pLevel);
            i1++;
            continue;
        }
        assert(i2 < v2.size());
        PlayerLevel *pLevel = v2.at(i2);
        assert(pLevel != nullptr);
            res.push_back(*pLevel);
        i2++;
    }
    assert(res.is_sorted());
    return res;
}





