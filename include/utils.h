#pragma once

#include <Arduino.h>
#include <stdarg.h>

#define HASH32(str) Text(str).hash32()

String utils_formatString(const char* format, ...);