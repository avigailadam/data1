//
// Created by avi on 28/11/2021.
//

#ifndef HW1_PLAYERLEVEL_H
#define HW1_PLAYERLEVEL_H


#include <memory>

class PlayerLevel {
    int level;
    int id;
public:
    PlayerLevel(int level, int id) : level(level), id(id) {}

    PlayerLevel(const PlayerLevel &other) {
        level = other.level;
        id = other.id;
    }

    bool operator>(const PlayerLevel &other) const {
        if (id == other.id)
            return false;
        return level == other.level
               ? id < other.id
               : level > other.level;
    }

    bool operator==(const PlayerLevel &other) const {
        return id == other.id;
    }

    bool operator<(const PlayerLevel &other) const {
        if (id == other.id)
            return false;
        return level == other.level
               ? id > other.id
               : level < other.level;
    }

    bool operator!=(const PlayerLevel &other) const {
        return id != other.id;
    }

    int getLevel() const {
        return level;
    }

    int getId() const {
        return id;
    }
};


#endif //HW1_PLAYERLEVEL_H
