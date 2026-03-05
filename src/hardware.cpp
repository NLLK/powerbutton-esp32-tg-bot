#include "hardware.h"

void init_hardware(){
    pinMode(OUTPUT_PIN, OUTPUT);
    digitalWrite(OUTPUT_PIN, LOW);
}

void press_button(){
    digitalWrite(OUTPUT_PIN, HIGH);
    delay(preferences.getUInt(CONFIG_KEY_SHORT_PRESS_TIME));
    digitalWrite(OUTPUT_PIN, LOW);
}

void long_press_button(){
    digitalWrite(OUTPUT_PIN, HIGH);
    delay(preferences.getUInt(CONFIG_KEY_LONG_PRESS_TIME));
    digitalWrite(OUTPUT_PIN, LOW);
}
