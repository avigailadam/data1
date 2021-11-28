#ifndef HW1_PLAYER_H
#define HW1_PLAYER_H

#include <memory>
#include "avl.h"
#include "group.h"
#include <assert>


class Player {
    Group& group;
    int level;
    const int id;
public:
    Player(Group &group, int level, int id) : group(group), level(level), id(id) {}


     Group& getGroup() const {
        return teamPointer;
    }

    int getLevel() const {
        return level;
    }

    int getId() const {
        return id;
    }

    bool operator>(const Player &other) const {
        return id > other.id;
    }

    bool operator==(const Player &other) const {
        return id == other.id;
    }

    bool operator<(const Player &other) const {
        return id < other.id;
    }

};

#endif //HW1_PLAYER_H
