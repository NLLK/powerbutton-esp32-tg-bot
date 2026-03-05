#pragma once

#include "global_context.h"
#include "const/config.h"

#define OUTPUT_PIN GPIO_NUM_0

void init_hardware();
void press_button();
void long_press_button();