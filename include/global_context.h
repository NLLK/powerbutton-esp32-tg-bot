#pragma once

// platform includes
#include <Arduino.h>
#include <Preferences.h>

// std includes
#include <map>
#include <set>

// library includes
#include <FastBot2.h>

// local includes
#include "utils.h"

// Global variables
extern FastBot2 bot;
extern Preferences preferences;

extern std::map<uint32_t, uint32_t> userMenuContext;
extern std::set<uint32_t> allowedUsers;
extern std::set<uint32_t> usersWaitingToGetAccessList;

void _set_context(fb::Update& u, const char* menu);
