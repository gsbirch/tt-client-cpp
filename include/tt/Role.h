#include <string>

#pragma once

#ifndef ROLE_H
#define ROLE_H

namespace tt {
    enum Role {
        /**
         * The game master role, who controls all characters except the player
         * character and the environment
         */
        GAME_MASTER,
        
        /**
         * The player role, who controls one characters, often the main character
         * of the story
         */
        PLAYER,
        
        /**
         * No role, used as a "null" value
         */
        NONE
    };

    Role getPartner(Role role);
    std::string rtos(Role role);
    Role stor(std::string s);
    std::ostream& operator<<(std::ostream& os, const Role& a);
}

#endif