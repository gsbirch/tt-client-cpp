#include <tt/Role.h>

namespace tt {
    /**
     * Returns the partner of this role. If this role is the game master, this
     * method returns the player. If this role is the player, this method
     * return the game master.
     * 
     * @return the partner of this role
     */
    Role getPartner(Role role) {
        if(role == Role::GAME_MASTER)
            return Role::PLAYER;
        else
            return Role::GAME_MASTER;
    }

    std::string rtos(Role role) {
        switch (role) {
            case Role::GAME_MASTER:
                return "GAME_MASTER";
            case Role::PLAYER:
                return "PLAYER";
            default:
                return "NONE";
        }
    }
    Role stor(std::string s)
    {
        if (s == "GAME_MASTER") return Role::GAME_MASTER;
        if (s == "PLAYER") return Role::PLAYER;
        return Role::NONE;
    }
}
