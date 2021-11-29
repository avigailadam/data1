//
// Created by avi on 28/11/2021.
//

#ifndef HW1_PLAYERLEVEL_H
#define HW1_PLAYERLEVEL_H


#include <memory>
#include "player_by_id.h"

class PlayerLevel {
    int level;
    int id;
public:
    PlayerLevel(int level, int id) : level(level), id(id) {}

    bool operator>(const PlayerLevel& other) const{
        return level == other.level
               ? id < other.id
               : level > other.level;
    }

    bool operator==(const PlayerLevel& other)  const{
        return id == other.id;
    }

    bool operator<(const PlayerLevel& other) const{
        return !(*this > other) && !(*this == other);
    }
};


#endif //HW1_PLAYERLEVEL_H
