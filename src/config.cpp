#include "main.h"

void init_config(){
    bool isConfigured = preferences.getBool(CONFIG_KEY_IS_CONFIGURED);

    if (!isConfigured){
        preferences.putUInt(CONFIG_KEY_SHORT_PRESS_TIME, SHORT_PRESS_TIME_DEFAULT);
        preferences.putUInt(CONFIG_KEY_LONG_PRESS_TIME, LONG_PRESS_TIME_DEFAULT);
        preferences.putUInt(CONFIG_KEY_LANGUAGE, LANG_DEFAULT);
        allowedUsers[0] = HASH32(ADMIN_CHAT_ID);
        preferences.putBytes(CONFIG_KEY_ALLOWED_USERS, (byte*)&allowedUsers, sizeof(allowedUsers));
    }

    preferences.getBytes(CONFIG_KEY_ALLOWED_USERS, (byte*)&allowedUsers, sizeof(allowedUsers));
}
