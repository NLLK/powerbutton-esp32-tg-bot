#pragma once

// platform includes
#include <Arduino.h>
#include <Preferences.h>

// std includes
#include <map>

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

//macros
#define HASH32(str) Text(str).hash32()

// Global variables
extern FastBot2 bot;
extern Preferences preferences;

extern std::map<uint32_t, uint32_t> userMenuContext;
extern uint32_t allowedUsers[MAX_USERS_ALLOWED_TO_CONTROL];

//hardware
void init_hardware();
void press_button(fb::Update& u);
void long_press_button(fb::Update& u);

//config
void init_config();

//settings
void set_language(fb::Update& u);
void set_short_press_time(fb::Update& u);
void set_long_press_time(fb::Update& u);

//ui
void set_settings(fb::Update& u);
void send_config(fb::Update& u);
void send_menu(fb::Update& u);
void send_pong(fb::Update& u);

//bot
void parse_context_vise(fb::Update& u);
void updateh(fb::Update& u);
void set_commands_list();

//common
void _set_context(fb::Update& u, const char* menu);
