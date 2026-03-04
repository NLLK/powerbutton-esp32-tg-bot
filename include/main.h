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

#include "env.h"
#include "const/commands.h"
#include "const/config_keys.h"
#include "const/translate.h"

// local defines
#define OUTPUT_PIN GPIO_NUM_0
#define LONG_PRESS_TIME_DEFAULT 10000U
#define SHORT_PRESS_TIME_DEFAULT 500U
#define LANG_DEFAULT TRANSLATE_LANG_EN
#define MAX_USERS_ALLOWED_TO_CONTROL 5
#define MAX_USERS_IN_WAIT_LIST 5

//macros
#define HASH32(str) Text(str).hash32()

// Global variables
extern FastBot2 bot;
extern Preferences preferences;

extern std::map<uint32_t, uint32_t> userMenuContext;
extern std::set<uint32_t> allowedUsers;
extern std::set<uint32_t> usersWaitingToGetAccessList;

//hardware
void init_hardware();
void press_button(fb::Update& u);
void long_press_button(fb::Update& u);

//config
void save_allowed_users();
void init_config();
void erase_allowed_users();
void append_allowed_user(uint32_t user);
void remove_allowed_user(uint32_t user);

void send_revoke_access_resp_handler(fb::Update& u);

//settings
void set_language(fb::Update& u);
void set_short_press_time(fb::Update& u);
void set_long_press_time(fb::Update& u);

//ui
void set_settings(fb::Update& u);
void send_config(fb::Update& u);
void send_menu(fb::Update& u);
void send_pong(fb::Update& u);
void not_allowed_users_menu(fb::Update& u);
void manage_users(fb::Update& u);
void send_gain_access_resp_handler(fb::Update& u);

//bot
void parse_context_vise(fb::Update& u);
void updateh(fb::Update& u);
void set_commands_list();

// securiry
bool security_filter_is_allowed(uint32_t userIdHash);

//common
void _set_context(fb::Update& u, const char* menu);
