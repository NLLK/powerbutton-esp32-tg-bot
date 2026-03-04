#include "main.h"

bool security_filter_is_allowed(uint32_t userIdHash){
    for (uint8_t i = 0; i < MAX_USERS_ALLOWED_TO_CONTROL; i++){
        if (userIdHash == allowedUsers[i]){
            return true;
        }
    }

    return false;
}