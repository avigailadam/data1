#ifndef HW1_GROUP_H
#define HW1_GROUP_H

#include <vector>
#include <cassert>
#include "avl.h"
#include "PlayerLevel.h"

class Group {
    int id;
    AvlTree<PlayerLevel> *playersByLevel;
public:
    Group(const Group &g) = delete;

//    Group clone() {
//        return Group(id, playersByLevel)
//    }
    Group(int id) : id(id) {}

    Group(std::unique_ptr<Group> other) : id(other->id) {
        assert(other.get() != nullptr);
        playersByLevel = other->playersByLevel;
        other->playersByLevel = nullptr;
    }

    Group(int id, AvlTree<PlayerLevel> *playersByLevel) : id(id), playersByLevel(playersByLevel) {}

    virtual ~Group() {
        delete this->playersByLevel;
    }

    std::vector<PlayerLevel *> getInorderLevel() const {
        return playersByLevel->inOrder();
    }

    int getId() const {
        return id;
    }

    bool operator>(const Group &other) const {
        return id > other.id;
    }

    bool operator==(const Group &other) const {
        return id == other.id;
    }

    bool operator<(const Group &other) const {
        return id < other.id;
    }

    const AvlTree<PlayerLevel> &getPlayersByLevel() const {
        return *playersByLevel;
    }


    void addPlayer(const PlayerLevel &player) {
        playersByLevel->insert(player);
    }

    void removePlayer(const PlayerLevel &player) {
        playersByLevel->remove(player);
    }

    PlayerLevel getMax() {
        if (playersByLevel == nullptr)
            throw NotExist();
        return playersByLevel->getMax();
    }
};

#endif //HW1_GROUP_H
