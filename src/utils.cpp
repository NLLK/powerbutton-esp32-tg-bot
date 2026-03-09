#include "utils.h"

String utils_formatString(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    int len = vsnprintf(nullptr, 0, format, args);
    va_end(args);
    
    if (len < 0) return String();

    char buffer[len + 1]; 
    va_start(args, format);
    vsnprintf(buffer, len + 1, format, args);
    va_end(args);

    return String(buffer);
}

String utils_getCodeFromUsersID(uint32_t id) {
    String str = String(id, 16);
    str.toUpperCase();

    return str;
}

String utils_getCodeFromUsersID(String id) {
    uint32_t hash = HASH32(id);
    return utils_getCodeFromUsersID(hash);
}

