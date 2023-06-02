#include "OpenEspresso.h"



void trigger1() {

  displayMessage("Change Target Temp", 2000);


}

void trigger2() {

  displayMessage("Change Soak Time", 2000);

  
}

void trigger3() {

  displayMessage("Change Flow Rate", 2000);
 
  
}


void trigger4() {

  displayMessage("Change Pressure", 2000);

  
}

void trigger5() {

  displayMessage("Change Yield", 2000);
  
}


void trigger6() {

  displayMessage("Change Duration", 2000);
  
}


void trigger7() { // Shoot

  if (steam_mode) {
    displayMessage("Cannot shoot while steaming", 2000);
    return;
  }

  if (shooting) {
    shooting = 0;
    
    myNex.writeNum("shoot.bco", 42292);
    myNex.writeStr("shoot.txt", "Shoot");

    analogWrite(PUMP_DIMMER, 0); // Turn off pump
    digitalWrite(SOLONOID, LOW); // Close solonoid

    displayMessage("Shot Stopped", 2000);

  } else {
    shooting = 1;
    
    myNex.writeNum("shoot.bco", 63488);
    myNex.writeStr("shoot.txt", "Shooting");

    analogWrite(PUMP_DIMMER, 255); // Turn on pump
    displayMessage("Shot Started", 2000);
    digitalWrite(SOLONOID, HIGH); // Open solonoid

  }

  
}


void trigger8() { // Steam Mode

  if (shooting) {
    displayMessage("Cannot steam while shooting", 2000);

    return;
  }

  if (steam_mode) {
    steam_mode = 0;
    myNex.writeNum("steam.bco", 42292);
    myNex.writeStr("steam.txt", "Steam");
      displayMessage("Steam Mode off", 2000);

  } else {
    steam_mode = 1;
   
    myNex.writeNum("steam.bco", 63488);
    myNex.writeStr("steam.txt", "Steaming");
    displayMessage("Steam Mode On", 2000);
 
  }
  
}

void trigger9() {

  if (flow_priority) {
    flow_priority = 0;
    
    myNex.writeNum("flowvspressure.bco", 63488);
    myNex.writeStr("flowvspressure.txt", "Pressure Priority");
    displayMessage("Pressure Priority", 2000);
  } else {
    flow_priority = 1;
    
    myNex.writeNum("flowvspressure.bco", 65504);
    myNex.writeStr("flowvspressure.txt", "Flow Priority");
    displayMessage("Flow Priority", 2000);
  }
  
}

void trigger10() {

  if (yield_priority) {
    yield_priority = 0;
    myNex.writeNum("yieldvstime.bco", 63488);
    myNex.writeStr("yieldvstime.txt", "Duration Priority");
    displayMessage("Duration Priority", 2000);
  } else {
    yield_priority = 1;
   
    myNex.writeNum("yieldvstime.bco", 65504);
    myNex.writeStr("yieldvstime.txt", "Yield Priority");
     displayMessage("Yield Priority", 2000);
  }
  
}
