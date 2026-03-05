#include "global_context.h"

// Global variables
FastBot2 bot;
Preferences preferences;

std::map<uint32_t, uint32_t> userMenuContext;
std::set<uint32_t> allowedUsers;
std::set<uint32_t> usersWaitingToGetAccessList;

void _set_context(fb::Update& u, const char* menu){
    userMenuContext[u.message().from().id().hash32()] = HASH32(menu);
}