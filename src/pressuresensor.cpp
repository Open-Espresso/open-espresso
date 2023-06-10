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
  // Reset the hw i2c to try and recover comms
  // on fail to do so throw error
  i2cResetState();

  // Throw error code on ADS malfunction/miswiring
  // Invalid Voltage error code: -100
  // Invalid gain error code: 255
  // Invalid mode error code: 254
  volatile short result = ads.getError();
  if (result == 0) return;
  char tmp[25];
  unsigned int check = snprintf(tmp, sizeof(tmp), "ADS error code: %i", result);
  if (check > 0 && check <= sizeof(tmp)) {
    displayMessage(tmp, 2000);
  }
}


void i2cResetState() {
  if(digitalRead(hw_SCL) != HIGH || digitalRead(hw_SDA) != HIGH || !ads.isConnected()) {
    //LOG_INFO("Reset I2C pins");
    volatile short result = I2C_ClearBus(hw_SDA, hw_SCL);
    char tmp[25];
    unsigned int check = snprintf(tmp, sizeof(tmp), "I2C error code: %i", result);
    if (check > 0 && check <= sizeof(tmp)) {
      result == 0 ? pressureInit() : displayMessage(tmp, 2000);
    }
    delay(50);
  }
}
