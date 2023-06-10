
#include "OpenEspresso.h"


// Blip the pump for 1/60 of a second

// This get called at the cycles per seconds of the zero crossing if we are supposed to be shooting espresso.
void runPump(void) {
    static int skip = 0;

    if (skip % 9 == 0) {
        analogWrite(PUMP_DIMMER, 255);
    } else {
        analogWrite(PUMP_DIMMER, 0);
    }   

    skip++;
}
