#include "main.h"

bool security_filter_is_allowed(uint32_t userIdHash){

    if (userIdHash == HASH32(ADMIN_CHAT_ID)) return true; // To ensure admin always have the full access

     if (auto search = allowedUsers.find(userIdHash);
            search != allowedUsers.end()){
        return true;
    }

    return false;
}