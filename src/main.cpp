
#include "OpenEspresso.h"


EasyNex myNex(Serial);   // Create an object of EasyNex class with the name < myNex >
                        // Set as parameter the Serial you are going to use

                        
uint16_t temperature_graph;  // a variable to store the reading

const int REFRESH_TIME = 100;             // time to refresh the Nextion page every 100 ms
unsigned long refresh_timer = millis();  // timer for refreshing Nextion's page
            
int ktcSO = SO_MAIN; // set up which pins the temp sensor uses for the main boiler.
int ktcCS = CS_MAIN;
int ktcCLK = CLK_MAIN;


MAX6675 main_ktc(ktcCLK, ktcCS, ktcSO);

float getTemp(MAX6675 ktc);
int steam_mode = 0; // 0 means off. 1 means on.
int shooting = 0; // 0 means off. 1 means on.
int flow_priority = 1; // 0 means off. 1 means on.
int yield_priority = 1; // 0 means off. 1 means on.

extern void trigger1();
extern void trigger2();
extern void trigger3();
extern void trigger4();
extern void trigger5();
extern void trigger6();
extern void trigger7();
extern void trigger8();
extern void trigger9();
extern void trigger10();



float maintainBoilerTemp(float current_temp, float target_temp, int power_pin);
void updateGraph(float temperature, float pressure, float flow_rate);
void displayMessage(String message, int min_duration);
void pollLCD();




void setup() {

  // initialize digital pin PC13 as an output.


  myNex.begin(9600); // Begin the LCD display object with a baud rate of 9600

  pinMode(BOILER_SSR, OUTPUT);
  digitalWrite(BOILER_SSR, LOW);
  pinMode(PUMP_DIMMER, OUTPUT);
  digitalWrite(PUMP_DIMMER, LOW);
  pinMode(SOLONOID, OUTPUT);
  digitalWrite(SOLONOID, LOW);

}

// the loop function runs over and over again forever

void loop() {
  

  float temperature = 0.0, pressure=0.0, flow_rate=0.0;



  if((millis()-refresh_timer) < REFRESH_TIME) {

    delay(REFRESH_TIME - (millis()-refresh_timer)); // Wait for the remaining time. Only run the main loop at 10hz.
    
  }

  refresh_timer = millis(); 

  pollLCD();


  temperature = getTemp(main_ktc); 

  displayMessage("Boiler Temp: " + String(temperature) + "C", 0);

  if (steam_mode) {
      maintainBoilerTemp(temperature, 140.0, BOILER_SSR);
  } else {
      maintainBoilerTemp(temperature, 93.0, BOILER_SSR);

  }

  updateGraph(temperature, pressure, flow_rate); // Update the graph on the LCD screen)

}

void updateGraph(float temperature, float pressure, float flow_rate){ // Update the graph on the LCD screen)

  if (temperature < 90.0) myNex.writeNum("maingraph.pco0", 1055); // BLUE
  else if (temperature > 95.0) myNex.writeNum("maingraph.pco0", 63488); // RED
  else myNex.writeNum("maingraph.pco0", 2016); // GREEN

  myNex.writeNum("nGraph.val", (uint16_t)temperature); 

}

// Show a message for a minimum duration in milliseconds. If this function is called again, ignore if that time has not passed.
void displayMessage(String message, int min_duration) {
  static unsigned long original_time = 0;
  static int active_min_duration = -1;

  // Get the current time
  unsigned long current_time = millis();

  if (active_min_duration == -1) {
    active_min_duration = min_duration; // Initialize for first use.
    original_time = current_time;
    myNex.writeStr("messages.txt", message);   // Display the message
    return;
  }

  unsigned long elapsed_time = current_time - original_time; // Calculate the elapsed time since the last valid call

  // Check if the minimum duration has elapsed. If so, you can show the message and reset for the next use.
  if (elapsed_time >= active_min_duration) {  // Return early if the minimum duration has not elapsed.
    active_min_duration = -1; // reset for next use.
    myNex.writeStr("messages.txt", message);   // Display the message
    return;
  }

  // If the minimum duration has not elapsed, then ignore the call. Nothing happens.

}


void pollLCD() {
  myNex.NextionListen(); // Check if there is any touch event on the LCD
}


// Only read the temp sensor no more than 4x a second. If sooner then return previous value. MAX6675 limitation.
float getTemp(MAX6675 ktc) {
  static float cachedTemp = 0.0;
  static unsigned long previousTime = 0;
  unsigned long currentTime;
  float check;

  currentTime = millis();

  if ((currentTime - previousTime) >= 250) {
    check = ktc.readCelsius();
    if (check != 0.0) {
      cachedTemp = check;
    }
    previousTime = currentTime;
  }

  return cachedTemp;

}

float maintainBoilerTemp(float current_temp, float target_temp, int power_pin) {
  // Temperature thresholds
  float temp_below_threshold = target_temp - 10.0;  // 10 degrees below target
  float temp_above_threshold = target_temp + 0.1;   // 0.1 degrees above target

  // Calculate the duty cycle based on temperature
  int duty_cycle = 0;  // Default to 0% duty cycle

  if (current_temp >= temp_below_threshold && current_temp < target_temp) {
    // Scale the duty cycle proportionately within the range
    float range = target_temp - temp_below_threshold;
    float temp_difference = target_temp - current_temp;
    duty_cycle = int((temp_difference / range) * 255);
  } else if (current_temp < temp_below_threshold) {
    duty_cycle = 255;  // 100% duty cycle
  }

  // Write the duty cycle to the power pin
  analogWrite(power_pin, duty_cycle);

  // Return the calculated duty cycle as a percentage
  return (duty_cycle / 255.0) * 100.0;
}


