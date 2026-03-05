#pragma once

#include "global_context.h"
#include "const/translate.h"

void save_allowed_users();
void init_config();
void erase_allowed_users();
void append_allowed_user(uint32_t user);
void remove_allowed_user(uint32_t user);

const char* context_vise_translate_get_msg(Dictionary message);