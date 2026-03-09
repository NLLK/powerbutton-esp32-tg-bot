#pragma once

#include "global_context.h"
#include "const/commands.h"
#include "const/config.h"
#include "const/translate.h"
#include "config.h"
#include "utils.h"
#include "env.h"
#include "hardware.h"

void handle_send_menu_req(fb::Update& u);
void handle_send_pong_req(fb::Update& u);
void handle_send_help_info_req(fb::Update& u);

void handle_press_button_req(fb::Update& u);
void handle_long_press_button_req(fb::Update& u);

void handle_set_settings_req(fb::Update& u);
void handle_send_config_req(fb::Update& u);
void handle_set_language_req(fb::Update& u);
void handle_set_language_resp(fb::Update& u);
void handle_set_short_press_time_req(fb::Update& u);
void handle_set_short_press_time_resp(fb::Update& u);
void handle_set_long_press_time_req(fb::Update& u);
void handle_set_long_press_time_resp(fb::Update& u);
void handle_set_msg_admin_power_on_req(fb::Update& u);
void handle_set_msg_adming_power_on_resp(fb::Update& u);

void handle_manage_users_req(fb::Update& u);
void handle_give_access_req(fb::Update& u);
void handle_give_access_resp(fb::Update& u);
void handle_revoke_access_req(fb::Update& u);
void handle_revoke_access_resp(fb::Update& u);
void handle_clear_wait_list_req(fb::Update& u);