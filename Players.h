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
    Player player;
    PlayerById *playerById;
public:
    explicit PlayerLevel(Player &player) : player(player), playerById(nullptr) {}

    PlayerLevel(Player &player, PlayerById *playerById) : player(player), playerById(playerById) {}

    PlayerLevel(const PlayerLevel &other) = default;

    bool operator>(const PlayerLevel &other) const {
        if (player.getId() == other.player.getId())
            return false;
        return player.getLevel() == other.player.getLevel()
               ? player.getId() < other.player.getId()
               : player.getLevel() > other.player.getLevel();
    }

    bool operator==(const PlayerLevel &other) const {
        return player.getId() == other.player.getId();
    }

    bool operator<(const PlayerLevel &other) const {
        if (player.getId() == other.player.getId())
            return false;
        return player.getLevel() == other.player.getLevel()
               ? player.getId() > other.player.getId()
               : player.getLevel() < other.player.getLevel();
    }

    bool operator!=(const PlayerLevel &other) const {
        return player.getId() != other.player.getId();
    }

    int getLevel() const {
        return player.getLevel();
    }

    int getId() const {
        return player.getId();
    }

    Group *getGroup() const {
        return player.getGroup();
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
