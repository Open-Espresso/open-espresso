#include "OpenEspresso.h"
#include <Wire.h>
#include "ADS1X15.h"
#include "i2c-bus.h"

#define hw_SCL        PB6
#define hw_SDA        PB7


void getAdsError(void);
void pressureInit(void);
void i2cResetState(void);
float getPressure(void);


#if defined PCB
  ADS1015 ads(0x48);
#else
  ADS1115 ads(0x48);
#endif

void pressureInit() {   

  ads.begin();
  ads.setGain(0);      // 6.144 volt
  ads.setDataRate(4);  // fast
  ads.setMode(0);      // continuous mode
  ads.readADC(0);      // first read to trigger

}

float getPressure() {  
    float pressure;
    volatile int16_t rawValue;


    //getAdsError();

    rawValue = ads.getValue();

    #if defined PCB
        pressure = (rawValue - 166.0) / 111.11f; // 12-bit
    #else
        pressure = (rawValue - 2666.0) / 1777.8f; // 16-bit
    #endif

    return pressure;

}


void getAdsError() {

}


void i2cResetState() {

}
