#pragma once

#include <Arduino.h>
#include <stdarg.h>
#include <StringUtils.h>

#define HASH32(str) Text(str).hash32()

String utils_formatString(const char* format, ...);
String utils_getCodeFromUsersID(uint32_t id);
String utils_getCodeFromUsersID(String id);

