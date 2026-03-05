
#include "utils.h"

#include <Arduino.h>

#include <stdarg.h>  // для va_list и друзей

String utils_formatString(const char* format, ...) {
  va_list args;
  va_start(args, format);
  
  int len = vsnprintf(nullptr, 0, format, args);
  va_end(args);
  
  if (len < 0) return String();

  char buffer[len + 1];  // VLA (может не поддерживаться некоторыми компиляторами)
  // Если VLA не поддерживается, используйте new[]:
  // char* buffer = new char[len + 1];
  
  va_start(args, format);
  vsnprintf(buffer, len + 1, format, args);
  va_end(args);
  
  String result(buffer);
  
  // Если использовали new[]:
  // delete[] buffer;
  
  return result;
}