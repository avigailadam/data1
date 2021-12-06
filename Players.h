#ifndef HW1_PLAYERS_H
#define HW1_PLAYERS_H

#include <ostream>

class Group;

class Player {
    int id;
    int level;
    Group *group;
public:
    Player(int id, int level, Group *group) : id(id), level(level), group(group) {}
    Player() : id(0), level(0), group(nullptr) {}

    friend std::ostream &operator<<(std::ostream &os, const Player &player) {
        os << "id: " << player.id << " level: " << player.level;
        return os;
    }

    Player(const Player &other) = default;

    int getId() const {
        return id;
    }

    int getLevel() const {
        return level;
    }

    Group *getGroup() const {
        return group;
    }

    void setGroup(Group *g) {
        group = g;
    }

    void setId(int id) {
        id = id;
    }

    void setLevel(int level) {
        level = level;
    }
};


class PlayerById {
    Player player;
public:
    PlayerById() : player(Player()) {}
    explicit PlayerById(Player player) : player(player) {}

    friend std::ostream &operator<<(std::ostream &os, const PlayerById &id) {
        os << "player: " << id.player;
        return os;
    }

    PlayerById &operator=(const PlayerById &other) {
        this->player.setLevel(other.player.getLevel());
        this->player.setId(other.player.getId());
        return  *this;
    }

    Player &getPlayer() {
        return player;
    }

    Group *getGroup() const {
        return player.getGroup();
    }

    int getLevel() const {
        return player.getLevel();
    }

    int getId() const {
        return player.getId();
    }

    bool operator>(const PlayerById &other) const {
        return player.getId() > other.player.getId();
    }

    bool operator==(const PlayerById &other) const {
        return player.getId() == other.player.getId();
    }

    bool operator!=(const PlayerById &other) const {
        return player.getId() != other.player.getId();
    }

    bool operator<(const PlayerById &other) const {
        return player.getId() < other.player.getId();
    }

    void setGroup(Group *group) {
        player.setGroup(group);
    }
};


class PlayerLevel {
    PlayerById *playerById;
public:
    friend std::ostream &operator<<(std::ostream &os, const PlayerLevel &level) {
        os << "playerById: " << *level.playerById;
        return os;
    }

    PlayerLevel() : playerById(nullptr) {

    }

    PlayerLevel(PlayerById *playerById) : playerById(playerById) {}

    PlayerLevel(const PlayerLevel &other): playerById(other.playerById) {}

    bool operator>(const PlayerLevel &other) const {
        if (playerById->getId() == other.getId())
            return false;
        return playerById->getLevel() == other.playerById->getLevel()
               ? playerById->getId() < other.playerById->getId()
               : playerById->getLevel() > other.playerById->getLevel();
    }

    bool operator==(const PlayerLevel &other) const {
        return playerById->getId() == other.playerById->getId();
    }

    bool operator<(const PlayerLevel &other) const {
        if (playerById->getId() == other.playerById->getId())
            return false;
        return playerById->getLevel() == other.playerById->getLevel()
               ? playerById->getId() > other.playerById->getId()
               : playerById->getLevel() < other.playerById->getLevel();
    }

    bool operator!=(const PlayerLevel &other) const {
        return playerById->getId() != other.playerById->getId();
    }

    int getLevel() const {
        return playerById->getLevel();
    }

    int getId() const {
        return playerById->getId();
    }

    Group *getGroup() const {
        return playerById->getGroup();
    }


    PlayerById *getPlayerById() {
        return playerById;
    }

};


class Group {
    int id;
    AvlTree<PlayerLevel> *playersByLevel;
public:
    Group(const Group &g) = delete;

    friend std::ostream &operator<<(std::ostream &os, const Group &group) {
        os << "id: " << group.id;
        return os;
    }

    Group(int id) : id(id), playersByLevel(new AvlTree<PlayerLevel>()) {}

    Group(int id,  my_vector<PlayerLevel> vec) : id(id), playersByLevel(new AvlTree<PlayerLevel>()) {
        playersByLevel->recursiveAvl(vec);
    }

    Group(std::unique_ptr<Group> other) : id(other->id) {
        playersByLevel = other->playersByLevel;
        other->playersByLevel = nullptr;
    }

    Group(int id, AvlTree<PlayerLevel> *playersByLevel) : id(id), playersByLevel(playersByLevel) {}

    virtual ~Group() {
        delete this->playersByLevel;
    }

    my_vector<PlayerLevel *> getInorderLevel() const {
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

    bool operator!=(const Group &other) const {
        return id != other.id;
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

    bool isEmpty() const {
        return playersByLevel->isEmpty();
    }
};

#endif //HW1_PLAYERS_H
