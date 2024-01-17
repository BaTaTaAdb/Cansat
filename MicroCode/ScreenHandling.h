#ifndef SCREENHANDLING_H
#define SCREENHANDLING_H

#include <Arduino.h>
#define button1_pin 18
#define button2_pin 19

void ISR_button(void);
void ScreenSetup(void);
void HandleScreen();
#endif