#ifndef HW1_GROUP_H
#define HW1_GROUP_H

#include "avl.h"
#include "player.h"

class Group{
    int id;
    AvlTree<std::shared_ptr<Player>> playersByLevel;
public:
    Group(int id, const AvlTree<std::shared_ptr<Player>> &playersByLevel) : id(id), playersByLevel(playersByLevel) {}

    virtual ~Group() {

    }

    int getId() const {
        return id;
    }

    const AvlTree<std::shared_ptr<Player>> &getPlayersByLevel() const {
        return playersByLevel;
    }
};

#endif //HW1_GROUP_H
