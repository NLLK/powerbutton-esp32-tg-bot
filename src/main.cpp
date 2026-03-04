// platform includes
#include <Arduino.h>
#include <Preferences.h>

// std includes
#include <map>

// library includes
#include <FastBot2.h>

// local includes
#include "env.h"
#include "commands.h"
#include "config_keys.h"
#include "translate.h"

// local defines
#define OUTPUT_PIN GPIO_NUM_0
#define LONG_PRESS_TIME_DEFAULT 10000U
#define SHORT_PRESS_TIME_DEFAULT 500U
#define LANG_DEFAULT TRANSLATE_LANG_EN

//macros
#define HASH32(str) Text(str).hash32()

// Global variables
FastBot2 bot;
Preferences preferences;

std::map<uint32_t, uint32_t> userMenuContext;

void _set_context(fb::Update& u, const char* menu){
    userMenuContext[u.message().from().id().hash32()] = HASH32(menu);
}

void init_config(){
    bool isConfigured = preferences.getBool(CONFIG_KEY_IS_CONFIGURED);

    if (!isConfigured){
        preferences.putUInt(CONFIG_KEY_SHORT_PRESS_TIME, SHORT_PRESS_TIME_DEFAULT);
        preferences.putUInt(CONFIG_KEY_LONG_PRESS_TIME, LONG_PRESS_TIME_DEFAULT);
        preferences.putUInt(CONFIG_KEY_LANGUAGE, LANG_DEFAULT);
    }
}

void press_button(fb::Update& u){
    digitalWrite(OUTPUT_PIN, HIGH);
    delay(preferences.getUInt(CONFIG_KEY_SHORT_PRESS_TIME));
    digitalWrite(OUTPUT_PIN, LOW);
    bot.sendMessage(fb::Message("Кнопка нажата кратко", u.message().from().id()));
}

void long_press_button(fb::Update& u){
    digitalWrite(OUTPUT_PIN, HIGH);
    delay(preferences.getUInt(CONFIG_KEY_LONG_PRESS_TIME));
    digitalWrite(OUTPUT_PIN, LOW);
    bot.sendMessage(fb::Message("Кнопка нажата длительно", u.message().from().id()));
}

void set_settings(fb::Update& u){
    fb::Message msg("Settings", u.message().from().id());

    fb::Menu menu;
    menu.addButton(COMMANDS_SET_LANGUAGE).addButton(COMMANDS_SET_SHORT_PRESS_TIME)
        .addButton(COMMANDS_SET_LONG_PRESS_TIME).addButton(COMMANDS_START_BACK);
    msg.setMenu(menu);

    bot.sendMessage(msg);
}

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

void send_config(fb::Update& u){
    String str = "";
    str += "Is Configured: " + String(preferences.getBool(CONFIG_KEY_IS_CONFIGURED) ? "true" : "false") + "\n";
    str += "Short press time: " + String(preferences.getUInt(CONFIG_KEY_SHORT_PRESS_TIME)) + "\n";
    str += "Long press time: " + String(preferences.getUInt(CONFIG_KEY_LONG_PRESS_TIME)) + "\n";
    str += "Lang: " + String(preferences.getUInt(CONFIG_KEY_LANGUAGE) == TRANSLATE_LANG_EN ? "EN" : "RU") + "\n";

    fb::Message msg(str, u.message().from().id());
    bot.sendMessage(msg);
}

void send_menu(fb::Update& u){
    _set_context(u, COMMANDS_START);

    fb::Message msg("Menu", u.message().from().id());
    fb::Menu menu;
    menu.addButton(COMMANDS_TURN_ON).addButton(COMMANDS_LONG_PRESS).newRow();
    menu.addButton(COMMANDS_START).addButton(COMMANDS_TEST_PING).newRow();
    menu.addButton(COMMANDS_SET_SETTINGS).addButton(COMMANDS_GET_CONFIG);
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
    }

}

void updateh(fb::Update& u) {
    Serial.println("NEW MESSAGE");
    Serial.println(u.message().from().username());
    Serial.println(u.message().text());

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
        } else {
            parse_context_vise(u);
        }
    }
}

void setup() {

    // setup hardware
    pinMode(OUTPUT_PIN, OUTPUT);
    digitalWrite(OUTPUT_PIN, LOW);

    // setup serial
    Serial.begin(115200);
    Serial.println();

    // setup wi-fi
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected");

    // setup preferences
    preferences.begin("app", false);
    init_config();

    // setup bot
    bot.attachUpdate(updateh); 
    bot.setToken(F(BOT_TOKEN)); 
    bot.setPollMode(fb::Poll::Long, 20000);

    fb::MyCommands commands;
    commands.addCommand(COMMANDS_START, "Меню");
    bot.setMyCommands(commands);

    // send admin message about device woke up
    bot.sendMessage(fb::Message("ESP32 Started", ADMIN_CHAT_ID));
}

void loop() {
    bot.tick();
}