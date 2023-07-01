#ifndef __CarlaMessages_H
#define __CarlaMessages_H

#include "../../lib/json.hpp"


struct light_update {
    std::string msg_type = "LIGHT_UPDATE";
    std::string light_curr_state;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(light_update, msg_type, light_curr_state)

struct light_command {
    std::string msg_type = "LIGHT_COMMAND";
    std::string light_next_state;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(light_command, msg_type, light_next_state)

#endif
