#ifndef HW1_PLAYER_BY_ID_H
#define HW1_PLAYER_BY_ID_H

#include <memory>
#include "avl.h"
#include "group.h"
#include <assert.h>


class PlayerById {
    Group &group;
    int level;
    int id;
public:
    PlayerById(Group &group, int level, int id) : group(group), level(level), id(id) {}

    void operator=(const PlayerById& other) {
      this->level = other.level;
      this->id = other.id;
      this->group = other.group;
    }

    PlayerById(const PlayerById& other): group(other.group), level(other.level), id(other.id){
    }

    Group &getGroup() const {
        return group;
    }

    int getLevel() const {
        return level;
    }

    int getId() const {
        return id;
    }

    bool operator>(const PlayerById &other) const {
        return id > other.id;
    }

    bool operator==(const PlayerById &other) const {
        return id == other.id;
    }

    bool operator!=(const PlayerById &other) const {
        return id != other.id;
    }

    bool operator<(const PlayerById &other) const {
        return id < other.id;
    }

    void setGroup(Group &newGroup) {
        group = newGroup;
    }

};

#endif //HW1_PLAYER_BY_ID_H
