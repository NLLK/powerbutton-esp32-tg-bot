#include "main.h"

void erase_allowed_users(){
    allowedUsers.clear();

    save_allowed_users(); //clear the properties

    append_allowed_user(HASH32(ADMIN_CHAT_ID));
}

void remove_allowed_user(uint32_t user){
    allowedUsers.erase(user);

    save_allowed_users();
}

void append_allowed_user(uint32_t user){
    if (allowedUsers.size() >= MAX_USERS_ALLOWED_TO_CONTROL)
        return;

    allowedUsers.insert(user);

    save_allowed_users();
}

void save_allowed_users(){
    uint32_t allowedUsersArray[MAX_USERS_ALLOWED_TO_CONTROL] = {0,};

    int i = 0;
    for (uint32_t userHash : allowedUsers){
        allowedUsersArray[i] = userHash;
        i++;
    }

    preferences.putBytes(CONFIG_KEY_ALLOWED_USERS, (byte*)&allowedUsersArray, sizeof(allowedUsersArray));
}

void load_allowed_users(){
    uint32_t allowedUsersArray[MAX_USERS_ALLOWED_TO_CONTROL] = {0,};
    preferences.getBytes(CONFIG_KEY_ALLOWED_USERS, (byte*)&allowedUsersArray, sizeof(allowedUsersArray));

    allowedUsers.clear();
    for (uint32_t userHash : allowedUsersArray){
        allowedUsers.insert(userHash);
    }
}

void init_config(){
    bool isConfigured = preferences.getBool(CONFIG_KEY_IS_CONFIGURED);

    if (!isConfigured){
        preferences.putUInt(CONFIG_KEY_SHORT_PRESS_TIME, SHORT_PRESS_TIME_DEFAULT);
        preferences.putUInt(CONFIG_KEY_LONG_PRESS_TIME, LONG_PRESS_TIME_DEFAULT);
        preferences.putUInt(CONFIG_KEY_LANGUAGE, LANG_DEFAULT);
        append_allowed_user(HASH32(ADMIN_CHAT_ID));

        preferences.putBool(CONFIG_KEY_IS_CONFIGURED, true);
    }

    load_allowed_users();
}
