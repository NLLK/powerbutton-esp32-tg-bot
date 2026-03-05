#include "commands_handler.h"

void handle_send_menu_req(fb::Update& u){
    _set_context(u, COMMANDS_START);

    fb::Message msg("Menu", u.message().from().id());
    fb::Menu menu;
    menu.addButton(COMMANDS_TURN_ON).addButton(COMMANDS_LONG_PRESS).newRow();
    menu.addButton(COMMANDS_START).addButton(COMMANDS_TEST_PING).newRow();
    menu.addButton(COMMANDS_SET_SETTINGS).addButton(COMMANDS_GET_CONFIG).addButton(COMMANDS_MANAGE_USERS);
    msg.setMenu(menu);

    bot.sendMessage(msg);
}

void handle_send_pong_req(fb::Update& u){
    bot.sendMessage(fb::Message("Pong", u.message().from().id()));
}

//
// hardware
//
void handle_press_button_req(fb::Update& u){
    press_button();
    bot.sendMessage(fb::Message("Кнопка нажата кратко", u.message().from().id()));
}

void handle_long_press_button_req(fb::Update& u){
    long_press_button();
    bot.sendMessage(fb::Message("Кнопка нажата длительно", u.message().from().id()));
}

//
// config
//
void handle_set_settings_req(fb::Update& u){
    fb::Message msg("Settings", u.message().from().id());

    fb::Menu menu;
    menu.addButton(COMMANDS_SET_LANGUAGE).addButton(COMMANDS_SET_SHORT_PRESS_TIME)
        .addButton(COMMANDS_SET_LONG_PRESS_TIME).addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);

    bot.sendMessage(msg);
}

void handle_send_config_req(fb::Update& u){
    String str = "";

    str += "Is Configured: " + String(preferences.getBool(CONFIG_KEY_IS_CONFIGURED) ? "true" : "false") + "\n";
    str += "Short press time: " + String(preferences.getUInt(CONFIG_KEY_SHORT_PRESS_TIME)) + "\n";
    str += "Long press time: " + String(preferences.getUInt(CONFIG_KEY_LONG_PRESS_TIME)) + "\n";
    str += "Lang: " + String(preferences.getUInt(CONFIG_KEY_LANGUAGE) == TRANSLATE_LANG_EN ? "EN" : "RU") + "\n";

    str += "Allowed users: \n";
    for (uint32_t userHash : allowedUsers){
        str += String(userHash) + "\n";
    }
    str += "You are: " + String(u.message().from().id().hash32());

    fb::Message msg(str, u.message().from().id());
    bot.sendMessage(msg);
}

void handle_set_language_req(fb::Update& u){
    _set_context(u, COMMANDS_SET_LANGUAGE);

    fb::Message msg("Choose language", u.message().from().id());

    fb::Menu menu;
    menu.addButton("EN").addButton("RU").addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);

    bot.sendMessage(msg);
}

void handle_set_language_resp(fb::Update& u){
    uint32_t message_hash = u.message().text().hash32();

    if (message_hash == HASH32("EN")){
        preferences.putUInt(CONFIG_KEY_LANGUAGE, TRANSLATE_LANG_EN);

        bot.sendMessage(fb::Message("Done", u.message().from().id()));
        handle_send_menu_req(u);
    }else if (message_hash == HASH32("RU")){
        preferences.putUInt(CONFIG_KEY_LANGUAGE, TRANSLATE_LANG_RU);

        bot.sendMessage(fb::Message("Done", u.message().from().id()));
        handle_send_menu_req(u);
    } else {
        bot.sendMessage(fb::Message("Wrong value", u.message().from().id()));
    }
}

void handle_set_short_press_time_req(fb::Update& u){
    _set_context(u, COMMANDS_SET_SHORT_PRESS_TIME);

    fb::Message msg("Send value from 100 to 5000 (in ms)", u.message().from().id());
    fb::Menu menu;
    menu.addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);
    bot.sendMessage(msg);
}

void handle_set_short_press_time_resp(fb::Update& u){
    uint32_t value = u.message().text().toInt32();

    if (value > 100 && value < 5000){
        preferences.putUInt(CONFIG_KEY_SHORT_PRESS_TIME, value);

        bot.sendMessage(fb::Message("Done", u.message().from().id()));

        handle_send_menu_req(u);
    } else {
        bot.sendMessage(fb::Message("Wrong value", u.message().from().id()));
    }
}

void handle_set_long_press_time_req(fb::Update& u){
    _set_context(u, COMMANDS_SET_LONG_PRESS_TIME);

    fb::Message msg("Send value from 100 to 20000 (in ms)", u.message().from().id());
    fb::Menu menu;
    menu.addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);
    bot.sendMessage(msg);
}

void handle_set_long_press_time_resp(fb::Update& u){
    uint32_t value = u.message().text().toInt32();

    if (value > 100 && value < 20000){
        preferences.putUInt(CONFIG_KEY_LONG_PRESS_TIME, value);

        bot.sendMessage(fb::Message("Done", u.message().from().id()));

        handle_send_menu_req(u);
    } else {
        bot.sendMessage(fb::Message("Wrong value", u.message().from().id()));
    }  
}

//
// user management
//

void handle_manage_users_req(fb::Update& u){
    _set_context(u, COMMANDS_MANAGE_USERS);

    fb::Message msg("User management", u.message().from().id());

    fb::Menu menu;
    menu.addButton(COMMANDS_GIVE_ACCESS).addButton(COMMANDS_CLEAR_WAIT_LIST).addButton(COMMANDS_REVOKE_ACCESS).addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);

    bot.sendMessage(msg);
}

void handle_give_access_req(fb::Update& u){
    _set_context(u, COMMANDS_GIVE_ACCESS);

    fb::Message msg("Give access to a user by their code.\nEnter user's code or select from menu", u.message().from().id());
    fb::Menu menu;

    for (uint32_t code : usersWaitingToGetAccessList){
        if (code == 0)
            continue;
        menu.addButton(String(code));
    }

    menu.addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);
    bot.sendMessage(msg);
}

void handle_give_access_resp(fb::Update& u){
    uint32_t value = u.message().text().toInt32();

    if (auto search = usersWaitingToGetAccessList.find(value);
        search != usersWaitingToGetAccessList.end()){
        usersWaitingToGetAccessList.erase(value);
        append_allowed_user(value);

        bot.sendMessage(fb::Message("Done", u.message().from().id()));
        handle_send_menu_req(u);
    }else{
        bot.sendMessage(fb::Message("This user never requested the access", u.message().from().id()));
    }
}

void handle_revoke_access_req(fb::Update& u){
    _set_context(u, COMMANDS_REVOKE_ACCESS);
    fb::Message msg("Revoke user's permission to use this app.\nEnter user's code or select from menu", u.message().from().id());
    fb::Menu menu;

    for (uint32_t code : allowedUsers){

        if (code == HASH32(ADMIN_CHAT_ID))
            continue; // one would never like to remove admin's access

        menu.addButton(String(code));
    }

    menu.addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);
    bot.sendMessage(msg);
}

void handle_revoke_access_resp(fb::Update& u){
    uint32_t value = u.message().text().toInt32();

    if (auto search = allowedUsers.find(value);
        search != allowedUsers.end()){
        remove_allowed_user(value);

        bot.sendMessage(fb::Message("Done", u.message().from().id()));
        handle_send_menu_req(u);
    }else{
        bot.sendMessage(fb::Message("This user never had access to this application", u.message().from().id()));
    }
}

void handle_clear_wait_list_req(fb::Update& u){
    usersWaitingToGetAccessList.clear();

    bot.sendMessage(fb::Message("Done", u.message().from().id()));
}
