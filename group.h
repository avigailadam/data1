#ifndef HW1_GROUP_H
#define HW1_GROUP_H

#include "avl.h"
#include "PlayerLevel.h"

class Group{
    int id;
    AvlTree<PlayerLevel> playersByLevel;
public:
    Group(int id) : id(id){}

    virtual ~Group() {

    }

    int getId() const {
        return id;
    }

    bool operator>(const Group& other) const{
        return id > other.id;
    }

    bool operator==(const Group& other) const{
        return id == other.id;
    }

    bool operator<(const Group& other) const{
        return id < other.id;
    }

    const AvlTree<PlayerLevel> &getPlayersByLevel() const {
        return playersByLevel;
    }

    void addPlayer(PlayerLevel player){
        playersByLevel.insert(player);
    }
};

#endif //HW1_GROUP_H
