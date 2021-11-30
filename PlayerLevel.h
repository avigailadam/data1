//
// Created by avi on 28/11/2021.
//

#ifndef HW1_PLAYERLEVEL_H
#define HW1_PLAYERLEVEL_H


#include <memory>
#include "PlayerById.h"

class PlayerLevel {
    int level;
    int id;
public:
    PlayerLevel(int level, int id) : level(level), id(id) {}
    PlayerLevel(const PlayerLevel& other){
        level = other.level;
        id = other.id;
    }

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
    int getLevel(){
        return level;
    }
    int getId(){
        return id;
    }
};


#endif //HW1_PLAYERLEVEL_H
