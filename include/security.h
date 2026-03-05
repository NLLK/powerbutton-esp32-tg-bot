#pragma once

#include <Arduino.h>
#include "env.h"
#include "global_context.h"

bool security_filter_is_allowed(uint32_t userIdHash);