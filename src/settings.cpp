
#include "main.h"

void set_language(fb::Update& u){
    _set_context(u, COMMANDS_SET_LANGUAGE);

    fb::Message msg("Choose language", u.message().from().id());

    fb::Menu menu;
    menu.addButton("EN").addButton("RU").addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);

    bot.sendMessage(msg);
}

void set_short_press_time(fb::Update& u){
    _set_context(u, COMMANDS_SET_SHORT_PRESS_TIME);

    fb::Message msg("Send value from 100 to 5000 (in ms)", u.message().from().id());
    fb::Menu menu;
    menu.addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);
    bot.sendMessage(msg);
}

void set_long_press_time(fb::Update& u){
    _set_context(u, COMMANDS_SET_LONG_PRESS_TIME);

    fb::Message msg("Send value from 100 to 20000 (in ms)", u.message().from().id());
    fb::Menu menu;
    menu.addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);
    bot.sendMessage(msg);
}
