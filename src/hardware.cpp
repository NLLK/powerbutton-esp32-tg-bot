#include "main.h"

void init_hardware(){
    pinMode(OUTPUT_PIN, OUTPUT);
    digitalWrite(OUTPUT_PIN, LOW);
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
