#ifndef HW1_PLAYERS_H
#define HW1_PLAYERS_H

class Group;

class Player {
    int id;
    int level;
    Group *group;
public:
    Player(int id, int level, Group *group) : id(id), level(level), group(group) {}

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
        assert(g != nullptr);
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
    explicit PlayerById(Player player) : player(player) {}

    PlayerById &operator=(const PlayerById &other) {
        this->player.setLevel(other.player.getLevel());
        this->player.setId(other.player.getId());
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

    PlayerLevel(PlayerById *playerById) : playerById(playerById) {}

    PlayerLevel(const PlayerLevel &other) = default;

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

    Group(int id) : id(id), playersByLevel(new AvlTree<PlayerLevel>()) {}

    Group(int id, std::vector<PlayerLevel> vec) : id(id), playersByLevel(new AvlTree<PlayerLevel>()) {
        playersByLevel->recursiveAvl(vec);
    }

    Group(std::unique_ptr<Group> other) : id(other->id) {
        assert(other.get() != nullptr);
        playersByLevel = other->playersByLevel;
        other->playersByLevel = nullptr;
    }

    Group(int id, AvlTree<PlayerLevel> *playersByLevel) : id(id), playersByLevel(playersByLevel) {}

    virtual ~Group() {
        delete this->playersByLevel;
    }

    std::vector<PlayerLevel *> getInorderLevel() const {
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
        assert(playersByLevel != nullptr);
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
};

#endif //HW1_PLAYERS_H
