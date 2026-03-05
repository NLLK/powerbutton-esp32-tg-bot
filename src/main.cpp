#include "main.h"

void setup() {

    // setup hardware
    init_hardware();

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

    set_commands_list();

    // send admin message about device woke up
    bot.sendMessage(fb::Message("ESP32 Started", ADMIN_CHAT_ID));
}

void loop() {
    bot.tick();
}