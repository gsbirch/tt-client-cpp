#include <tt/io/Message.h>

#pragma once 

#ifndef CONNECT_H
#define CONNECT_H

namespace tt {
    /**
     * The connect message is sent from the {edu.uky.cs.nil.tt.Server server} to an
     * {@link edu.uky.cs.nil.tt.Agent agent} as soon as the agent connects to give
     * details about which story worlds and agents are available for play.
     * 
     * @author Gage Birchmeier
     */
    class Connect : public Message {
        
    };
}

#endif