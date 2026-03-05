#include "message_handler.h"

void set_commands_list(){
    fb::MyCommands commands;
    commands.addCommand(COMMANDS_START, "Меню");
    bot.setMyCommands(commands);
}

void parse_context_vise(fb::Update& u){
    uint32_t context_hash = userMenuContext[u.message().from().id().hash32()];

    if (context_hash == HASH32(COMMANDS_SET_LANGUAGE)){
        handle_set_language_resp(u);
    }else if (context_hash == HASH32(COMMANDS_SET_SHORT_PRESS_TIME)){
        handle_set_short_press_time_resp(u);
    }else if (context_hash == HASH32(COMMANDS_SET_LONG_PRESS_TIME)){
        handle_set_long_press_time_resp(u);
    }else if (context_hash == HASH32(COMMANDS_GIVE_ACCESS)){
        handle_give_access_resp(u);
    }else if (context_hash == HASH32(COMMANDS_REVOKE_ACCESS)){
        handle_revoke_access_resp(u);
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
            menu.addButton(COMMANDS_REQUEST_ACCESS);
            msg.setMenu(menu);

            bot.sendMessage(msg);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_REQUEST_ACCESS).hash32()){
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
                + String(".\nIn case you want to authorize this user, use this command ") + String(COMMANDS_GIVE_ACCESS)),
                ADMIN_CHAT_ID));
        }
    }
}

void send_gain_access_resp_handler(fb::Update& u){
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
            handle_send_menu_req(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_TEST_PING).hash32()){
            handle_send_pong_req(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_TURN_ON).hash32()){
            press_button(u);
            bot.sendMessage(fb::Message("Кнопка нажата кратко", u.message().from().id()));
        } else if (u.message().text().hash32() == su::Text(COMMANDS_LONG_PRESS).hash32()){
            long_press_button(u);
            bot.sendMessage(fb::Message("Кнопка нажата длительно", u.message().from().id()));
        } else if (u.message().text().hash32() == su::Text(COMMANDS_GET_CONFIG).hash32()){
            handle_send_config_req(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_SET_SETTINGS).hash32()){
            handle_set_settings_req(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_SET_LANGUAGE).hash32()){
            handle_set_language_req(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_SET_SHORT_PRESS_TIME).hash32()){
            handle_set_short_press_time_req(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_SET_LONG_PRESS_TIME).hash32()){
            handle_set_long_press_time_req(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_MANAGE_USERS).hash32()){
            handle_manage_users_req(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_GIVE_ACCESS).hash32()){
            handle_give_access_req(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_CLEAR_WAIT_LIST).hash32()){
            handle_clear_wait_list_req(u);
        } else if (u.message().text().hash32() == su::Text(COMMANDS_REVOKE_ACCESS).hash32()){
            handle_revoke_access_req(u);
        } else {
            parse_context_vise(u);
        }
    }
}