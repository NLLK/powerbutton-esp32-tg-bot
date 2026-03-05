#pragma once

// platform includes
#include <Arduino.h>
#include <Preferences.h>

// local includes
#include "env.h"
#include "global_context.h"
#include "utils.h"
#include "const/commands.h"
#include "const/config.h"
#include "const/translate.h"
#include "config.h"

// include modules
#include "message_handler.h"

//hardware
#include "hardware.h"

//settings
void set_language(fb::Update& u);
void set_short_press_time(fb::Update& u);
void set_long_press_time(fb::Update& u);

//ui
void set_settings(fb::Update& u);
void send_config(fb::Update& u);
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
