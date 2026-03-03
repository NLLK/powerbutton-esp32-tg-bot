#include <Arduino.h>

#include "env.h"

#define OUTPUT_PIN GPIO_NUM_0

#include <FastBot2.h>
FastBot2 bot;

void press_button(fb::Update& u){
    digitalWrite(OUTPUT_PIN, HIGH);
    delay(3000);
    digitalWrite(OUTPUT_PIN, LOW);
    bot.sendMessage(fb::Message("Нажата кнопка", u.message().from().id()));
}

void send_menu(fb::Update& u){
    fb::Message msg("Меню", u.message().from().id());
    fb::InlineMenu menu("Нажать кнопку;Пинг;", "/turn_on;/test_ping");
    msg.setInlineMenu(menu);
    bot.sendMessage(msg);
}

void updateh(fb::Update& u) {
    Serial.println("NEW MESSAGE");
    Serial.println(u.message().from().username());
    Serial.println(u.message().text());

    if (u.isQuery()) {
        Serial.println("NEW QUERY");
        Serial.println(u.query().data());

        // ответ на query
        // bot.answerCallbackQuery(u.query().id());
        bot.answerCallbackQuery(u.query().id(), u.query().data());
        // bot.answerCallbackQuery(u.query().id(), u.query().data(), true);

        if (u.query().data().compare("/test_ping")){
            // press_button(u);
            bot.sendMessage(fb::Message("Pong", u.message().from().id()));
        }else if (u.query().data().compare("/turn_on")){
            press_button(u);
        }
        send_menu(u);
    }else{
        if (u.message().text() == "/start"){
            send_menu(u);
        }
    }
}

void setup() {

    pinMode(OUTPUT_PIN, OUTPUT);
    digitalWrite(OUTPUT_PIN, LOW);

    Serial.begin(115200);
    Serial.println();

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected");

    // ============
    bot.attachUpdate(updateh);   // подключить обработчик обновлений
    bot.setToken(F(BOT_TOKEN));  // установить токен

    // режим опроса обновлений. Самый быстрый - Long
    // особенности читай тут в самом низу
    // https://github.com/GyverLibs/FastBot2/blob/main/docs/3.start.md

    // bot.setPollMode(fb::Poll::Sync, 4000);  // умолч
    // bot.setPollMode(fb::Poll::Async, 4000);
    bot.setPollMode(fb::Poll::Long, 20000);

    // поприветствуем админа
    bot.sendMessage(fb::Message("ESP32 Started", CHAT_ID));
}

void loop() {
    // вызывать тикер в loop
    bot.tick();
}