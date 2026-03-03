// platform includes
#include <Arduino.h>
#include <Preferences.h>

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

// Global variables
FastBot2 bot;
Preferences preferences;

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

void send_config(fb::Update& u){
    String str = "";
    str += "Is Configured: " + String(preferences.getBool(CONFIG_KEY_IS_CONFIGURED) ? "true" : "false") + "\n";
    str += "Short press time: " + String(preferences.getUInt(CONFIG_KEY_SHORT_PRESS_TIME)) + "\n";
    str += "Long press time: " + String(preferences.getUInt(CONFIG_KEY_LONG_PRESS_TIME)) + "\n";
    str += "Lang: " + String(preferences.getUInt(CONFIG_KEY_LANGUAGE) == 0 ? "EN" : "RU") + "\n";

    fb::Message msg(str, u.message().from().id());
    bot.sendMessage(msg);
}

void send_menu(fb::Update& u){

    fb::Message msg("Menu", u.message().from().id());
    fb::Menu menu;
    menu.addButton(COMMANDS_TURN_ON).addButton(COMMANDS_LONG_PRESS).newRow();
    menu.addButton(COMMANDS_START).addButton(COMMANDS_TEST_PING).addButton(COMMANDS_GET_CONFIG);
    
    msg.setMenu(menu);

    bot.sendMessage(msg);
}

void send_pong(fb::Update& u){
    bot.sendMessage(fb::Message("Pong", u.message().from().id()));
}

void updateh(fb::Update& u) {
    Serial.println("NEW MESSAGE");
    Serial.println(u.message().from().username());
    Serial.println(u.message().text());

    if (u.isMessage()){
        if (u.message().text() == COMMANDS_START){
            send_menu(u);
        } else if (u.message().text() == COMMANDS_TEST_PING){
            send_pong(u);
        } else if (u.message().text() == COMMANDS_TURN_ON){
            press_button(u);
        } else if (u.message().text() == COMMANDS_LONG_PRESS){
            long_press_button(u);
        } else if (u.message().text() == COMMANDS_GET_CONFIG){
            send_config(u);
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

    // send admin message about device woke up
    bot.sendMessage(fb::Message("ESP32 Started", ADMIN_CHAT_ID));
}

void loop() {
    bot.tick();
}