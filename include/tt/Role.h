#include <string>

#pragma once

#ifndef ROLE_H
#define ROLE_H

namespace tt {
    /**
     * Represents the two roles in a collaborative storytelling session: 
     * the Player, who typically controls a single character, and the
     * Game Master, who controls all other characters and the environment.
     * 
     * @author Gage Birchmeier
     */
    enum Role {
        /**
         * The game master role, who controls all characters except the player
         * character and the environment
         */
        GAME_MASTER,
        
        /**
         * The player role, who controls one character, often the main character
         * of the story
         */
        PLAYER,
        
        /**
         * No role, used as a "null" value
         */
        NONE
    };

    /**
     * Returns the partner of this role. If this role is the game master, this
     * method returns the player. If this role is the player, this method
     * return the game master.
     * 
     * @return the partner of this role
     */
    Role getPartner(Role role);

    /**
     * Returns the correct string value needed for the Tandem Tales Protocol
     * for a given role value.
     * 
     * @param role a role enum
     * @return a string holding the value of the role
     */
    std::string rtos(Role role);

    /**
     * Returns the Role enum corresponding to the string value. This is based on
     * the Tandem Tales Protocol.
     * 
     * @param s a string holding a role value
     * @return the corresponding role enum value
     */
    Role stor(std::string s);

    /**
     * Override stream insertion operator to allow printing a role enum value
     * 
     * @param os an output stream to insert into
     * @param a a role enum
     * @returns the output stream referenced
     */ 
    std::ostream& operator<<(std::ostream& os, const Role& a);
}

#endif