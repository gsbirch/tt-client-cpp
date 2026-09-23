#include <tt/io/Message.h>
#include <tt/io/Entry.h>


#pragma once 

#ifndef CONNECT_H
#define CONNECT_H

namespace tt {
    /**
	 * Advertises that a specific story world and agent pair is waiting for a
	 * match.
	 * 
	 * @author Stephen G. Ware
	 */
    class Available {
        public:
            /** The name of the story world the waiting agent wants to play in */
            std::string world;
            /** The name of the agent waiting for a match */
            std::string agent;
            /**
             * Constructs an available world/agent pair from a world name and agent
             * name.
             * 
             * @param world the name of the world the waiting agent wants to play in
             * @param agent the name of the agent waiting for a match
             */
            Available(std::string world, std::string agent):
            world(world), agent(agent) {}

            // default constructor for json deserialization
            Available() {}

            std::string toString() const {
                return "[Available: world=\"" + world + "\"; agent=\"" + agent + "\"]";
            }
            friend std::ostream& operator<<(std::ostream& os, const Available& a);
    };
    /**
     * The connect message is sent from the {edu.uky.cs.nil.tt.Server server} to an
     * {@link edu.uky.cs.nil.tt.Agent agent} as soon as the agent connects to give
     * details about which story worlds and agents are available for play.
     * 
     * @author Gage Birchmeier
     */
    class Connect : public Message {
        public:
            /** The server's software version */
            std::string version;

            /**
             * A list of {@link Entry entries} giving the names and descriptions of the
             * publicly listed worlds available on this server
             */
            std::vector<Entry> worlds;

            /**
             * A list of {@link Entry entries} giving the names and descriptions of the
             * publicly listed agents available on this server
             */
            std::vector<Entry> agents;

            /**
             * A list of {@link Available available} world/agent pairs that lets an
             * agent know whether a specific combination of a story world and agent
             * is immediately available for play
             */
            std::vector<Available> available;

            Connect() {}

            std::string toString() const override;

            void verify() const override;

            std::string type() const override {
                return "Connect";
            }
    };

    void from_json(const nlohmann::json& j, Connect& msg);
    void to_json(nlohmann::json& j, const Connect& msg);
    void from_json(const nlohmann::json& j, Available& obj);
    void to_json(nlohmann::json& j, const Available& obj);
}

#endif