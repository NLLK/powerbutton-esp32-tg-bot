#pragma once

#include "global_context.h"
#include "const/commands.h"
#include "const/config.h"
#include "const/translate.h"

#include "utils.h"
#include "commands_handler.h"
#include "security.h"

void set_commands_list();
void parse_context_vise(fb::Update& u);
void not_allowed_users_menu(fb::Update& u);
void updateh(fb::Update& u);