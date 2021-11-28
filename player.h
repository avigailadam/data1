#ifndef HW1_PLAYER_H
#define HW1_PLAYER_H

#include <memory>
#include "avl.h"

class Player{
    AvlTree<std::shared_ptr<Player>> teamPointer;
    int level;
    int id;
public:
    Player(const AvlTree<std::shared_ptr<Player>> &teamPointer, int level, int id) : teamPointer(teamPointer),
                                                                                     level(level), id(id) {}

    virtual ~Player() {

    }

    const AvlTree<std::shared_ptr<Player>> &getTeamPointer() const {
        return teamPointer;
    }

    int getLevel() const {
        return level;
    }

    int getId() const {
        return id;
    }

};


#endif //HW1_PLAYER_H
