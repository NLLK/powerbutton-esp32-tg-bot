#pragma once

#include "global_context.h"

void save_allowed_users();
void init_config();
void erase_allowed_users();
void append_allowed_user(uint32_t user);
void remove_allowed_user(uint32_t user);