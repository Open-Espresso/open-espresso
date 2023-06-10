#ifndef OPENESPRESSO_H
#define OPENESPRESSO_H

#include <Arduino.h>
#include "EasyNextionLibrary.h"

#define PCB

#if defined PCB
#include <Wire.h>
#include <Adafruit_MAX31855.h>
#else
#include <max6675.h>
#endif



extern int steam_mode;
extern int shooting;
extern int flow_priority;
extern int yield_priority;
extern EasyNex myNex;



void trigger1();
void trigger2();
void trigger3();
void trigger4();
void trigger5();
void trigger6();
void trigger7();
void trigger8();
void trigger9();
void trigger10();

void displayMessage(String message, int min_duration);

#endif // OPENESPRESSO_H

#ifdef LEGO
#define SO_MAIN         PB4
#define CS_MAIN         PA6
#define CLK_MAIN        PA5
#define BOILER_SSR      PA15
#define PUMP_DIMMER     PA1
#define SOLONOID        PC13
#define ZC_PIN          PA0
#endif

#ifdef PCB
#define SO_MAIN         PB4
#define CS_MAIN         PA6
#define CLK_MAIN        PA5
#define BOILER_SSR      PA15
#define PUMP_DIMMER     PA1
#define SOLONOID        PC13
#define ZC_PIN          PA0
#endif

// Pressure sensor
extern void pressureInit(void);
extern float getPressure(void);

// Run pump
extern void runPump(void);

