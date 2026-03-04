#include "main.h"

void set_commands_list(){
    fb::MyCommands commands;
    commands.addCommand(COMMANDS_START, "Меню");
    bot.setMyCommands(commands);
}

void set_settings(fb::Update& u){
    fb::Message msg("Settings", u.message().from().id());

    fb::Menu menu;
    menu.addButton(COMMANDS_SET_LANGUAGE).addButton(COMMANDS_SET_SHORT_PRESS_TIME)
        .addButton(COMMANDS_SET_LONG_PRESS_TIME).addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);

    bot.sendMessage(msg);
}

void send_config(fb::Update& u){
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

void send_menu(fb::Update& u){
    _set_context(u, COMMANDS_START);

    fb::Message msg("Menu", u.message().from().id());
    fb::Menu menu;
    menu.addButton(COMMANDS_TURN_ON).addButton(COMMANDS_LONG_PRESS).newRow();
    menu.addButton(COMMANDS_START).addButton(COMMANDS_TEST_PING).newRow();
    menu.addButton(COMMANDS_SET_SETTINGS).addButton(COMMANDS_GET_CONFIG).addButton(COMMANDS_MANAGE_USERS);
    msg.setMenu(menu);

    bot.sendMessage(msg);
}

void send_pong(fb::Update& u){
    bot.sendMessage(fb::Message("Pong", u.message().from().id()));
}

void parse_context_vise(fb::Update& u){
    uint32_t context_hash = userMenuContext[u.message().from().id().hash32()];

    if (context_hash == HASH32(COMMANDS_SET_LANGUAGE)){
        uint32_t message_hash = u.message().text().hash32();

        if (message_hash == HASH32("EN")){
            preferences.putUInt(CONFIG_KEY_LANGUAGE, TRANSLATE_LANG_EN);

            bot.sendMessage(fb::Message("Done", u.message().from().id()));
            send_menu(u);
        }else if (message_hash == HASH32("RU")){
            preferences.putUInt(CONFIG_KEY_LANGUAGE, TRANSLATE_LANG_RU);

            bot.sendMessage(fb::Message("Done", u.message().from().id()));
            send_menu(u);
        } else {
            bot.sendMessage(fb::Message("Wrong value", u.message().from().id()));
        }
    }else if (context_hash == HASH32(COMMANDS_SET_SHORT_PRESS_TIME)){
        uint32_t value = u.message().text().toInt32();

        if (value > 100 && value < 5000){
            preferences.putUInt(CONFIG_KEY_SHORT_PRESS_TIME, value);

            bot.sendMessage(fb::Message("Done", u.message().from().id()));

            send_menu(u);
        } else {
            bot.sendMessage(fb::Message("Wrong value", u.message().from().id()));
        }
    }else if (context_hash == HASH32(COMMANDS_SET_LONG_PRESS_TIME)){
        uint32_t value = u.message().text().toInt32();

        if (value > 100 && value < 20000){
            preferences.putUInt(CONFIG_KEY_LONG_PRESS_TIME, value);

            bot.sendMessage(fb::Message("Done", u.message().from().id()));

            send_menu(u);
        } else {
            bot.sendMessage(fb::Message("Wrong value", u.message().from().id()));
        }   
    }else if (context_hash == HASH32(COMMANDS_GAIN_ACCESS)){
        send_gain_access_resp_handler(u);
    }else if (context_hash == HASH32(COMMANDS_REVOKE_ACCESS)){
        send_revoke_access_resp_handler(u);
    }
}

void not_allowed_users_menu(fb::Update& u){
    if (u.isMessage()){

        if (usersWaitingToGetAccessList.size() > MAX_USERS_IN_WAIT_LIST){
            bot.sendMessage(fb::Message("Too many users trying to get access. Ask administrator to clear waiting list", u.message().from().id()));
            return;
        }

        if (u.message().text().hash32() == su::Text(COMMANDS_START).hash32()){
            fb::Message msg("Menu", u.message().from().id());
            fb::Menu menu;
            menu.addButton(COMMANDS_ASK_TO_GAIN_ACCESS);
            msg.setMenu(menu);

            bot.sendMessage(msg);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_ASK_TO_GAIN_ACCESS).hash32()){
            uint32_t user_hash = u.message().from().id().hash32();

            if (auto search = usersWaitingToGetAccessList.find(user_hash);
                 search != usersWaitingToGetAccessList.end()){
                return;
            }else{
                usersWaitingToGetAccessList.insert(user_hash);
            }

            bot.sendMessage(fb::Message(Text(String("Access requested. Wait for admin to give you an access. Tell them this code: ")
                + String(user_hash)), u.message().from().id()));

            bot.sendMessage(fb::Message(
                Text(String("Some user required access to this app. They should tell you this this code: ") + String(user_hash) 
                + String(".\nIn case you want to authorize this user, use this command ") + String(COMMANDS_GAIN_ACCESS)),
                ADMIN_CHAT_ID));
        }
    }
}


void manage_users(fb::Update& u){
    _set_context(u, COMMANDS_MANAGE_USERS);

    fb::Message msg("User management", u.message().from().id());

    fb::Menu menu;
    menu.addButton(COMMANDS_GAIN_ACCESS).addButton(COMMANDS_CLEAR_WAIT_LIST).addButton(COMMANDS_REVOKE_ACCESS).addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);

    bot.sendMessage(msg);
}

void send_clear_wait_list(fb::Update& u){
    usersWaitingToGetAccessList.clear();

    bot.sendMessage(fb::Message("Done", u.message().from().id()));
}

void send_revoke_access(fb::Update& u){
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

void send_revoke_access_resp_handler(fb::Update& u){
    uint32_t value = u.message().text().toInt32();

    if (auto search = allowedUsers.find(value);
        search != allowedUsers.end()){
        remove_allowed_user(value);

        bot.sendMessage(fb::Message("Done", u.message().from().id()));
        send_menu(u);
    }else{
        bot.sendMessage(fb::Message("This user never had access to this application", u.message().from().id()));
    }
}

void send_gain_access(fb::Update& u){
    _set_context(u, COMMANDS_GAIN_ACCESS);

    fb::Message msg("Give access to a user by their code.\nEnter user's code or select from menu", u.message().from().id());
    fb::Menu menu;

    for (uint32_t code : usersWaitingToGetAccessList){
        menu.addButton(String(code));
    }

    menu.addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);
    bot.sendMessage(msg);
}

void send_gain_access_resp_handler(fb::Update& u){
    uint32_t value = u.message().text().toInt32();

    if (auto search = usersWaitingToGetAccessList.find(value);
        search != usersWaitingToGetAccessList.end()){
        usersWaitingToGetAccessList.erase(value);
        append_allowed_user(value);

        bot.sendMessage(fb::Message("Done", u.message().from().id()));
        send_menu(u);
    }else{
        bot.sendMessage(fb::Message("This user never requested the access", u.message().from().id()));
    }
}

void updateh(fb::Update& u){
    Serial.println("NEW MESSAGE");
    Serial.println(u.message().from().username());
    Serial.println(u.message().text());

    if (!security_filter_is_allowed(u.message().from().id().hash32())){
        not_allowed_users_menu(u);
        return;
    }

    if (u.isMessage()){
        if (u.message().text().hash32() == su::Text(COMMANDS_START).hash32() 
            || u.message().text().hash32() == su::Text(COMMANDS_START_BACK).hash32()){
            send_menu(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_TEST_PING).hash32()){
            send_pong(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_TURN_ON).hash32()){
            press_button(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_LONG_PRESS).hash32()){
            long_press_button(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_GET_CONFIG).hash32()){
            send_config(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_SET_SETTINGS).hash32()){
            set_settings(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_SET_LANGUAGE).hash32()){
            set_language(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_SET_SHORT_PRESS_TIME).hash32()){
            set_short_press_time(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_SET_LONG_PRESS_TIME).hash32()){
            set_long_press_time(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_MANAGE_USERS).hash32()){
            manage_users(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_GAIN_ACCESS).hash32()){
            send_gain_access(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_CLEAR_WAIT_LIST).hash32()){
            send_clear_wait_list(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_REVOKE_ACCESS).hash32()){
            send_revoke_access(u);
        } else {
            parse_context_vise(u);
        }
    }
}