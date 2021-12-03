#ifndef HW1_BESTPLAYERBYGROUP_H
#define HW1_BESTPLAYERBYGROUP_H

#include "Players.h"

class BestPlayerByGroup{
    int id;
    int groupID;
public:
    BestPlayerByGroup(int id, int groupId) : id(id), groupID(groupId) {}

    explicit BestPlayerByGroup(const Group& group): id(group.getPlayersByLevel().getMax().getId()), groupID(group.getId()) {}

    int getId() const {
        return id;
    }

    int getGroupId() const {
        return groupID;
    }

    bool operator>(const BestPlayerByGroup &other) const {
        return groupID > other.groupID;
    }

    bool operator==(const BestPlayerByGroup &other) const {
        return groupID == other.groupID;
    }

    bool operator!=(const BestPlayerByGroup &other) const {
        return groupID != other.groupID;
    }

    bool operator<(const BestPlayerByGroup &other) const {
        return groupID < other.groupID;
    }
};





#endif //HW1_BESTPLAYERBYGROUP_H
