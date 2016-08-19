#include "relay.h"
#include "constants.h"

volatile relay* waterfall = new relay();
volatile relay* uv = new relay();
volatile relay* mist = new relay();
volatile relay* r_array[3] = {waterfall, uv, mist}; //right to left on hood, in order of closest -> furthest from user

void pin_setup(){
  for (int ii = 0; ii< 3; ii++){
    volatile relay* r = r_array[ii];
    r->ID = ii;
    r->auto_pin = _auto_pins[ii];
    r->manual_pin = _manual_pins[ii];
    r->relay_pin = _relay_pins[ii];
    r->switch_case = _switch_cases[ii]; 
  }  
  
  for (int ii=0; ii<3; ii++){
    pinMode(r_array[ii]->auto_pin, INPUT);
    pinMode(r_array[ii]->manual_pin, INPUT);
    pinMode(r_array[ii]->relay_pin, OUTPUT);
  
    digitalWrite(r_array[ii]->relay_pin, HIGH);  
  }
  pinMode(lcd_mode, INPUT); //lcd print mode 1 or 2
}

// Checks state of switches
void switch_state(int relay_ID) {
  volatile relay* r = r_array[relay_ID]; 
  if(digitalRead(r->auto_pin)==HIGH){ //automate day night cycle
     //switch_status = "AUTO ON RELAY ";
     r->switch_case = 0;
     auto_mode(relay_ID, r->relay_pin);
  }
  else if(digitalRead(r->manual_pin)==HIGH){ //turn it on
    //switch_status = "MANUAL ON RELAY ";
    r->switch_case = 1;
    digitalWrite(r->relay_pin, LOW); //active low relays
  }
  else if(digitalRead(r->manual_pin)==LOW && digitalRead(r->auto_pin)==LOW){ //turn it off
    //switch_status = "OFF RELAY ";
    r->switch_case = 2;
    digitalWrite(r->relay_pin, HIGH);
  }
  else{
    //switch_status = "ERROR: RELAY SWITCH NOT READ";
    r->switch_case = 3;
    digitalWrite(r->relay_pin, HIGH);
  }

  // check if switch state has changed
  if (r->switch_case != _switch_cases[relay_ID]) {
    Serial.print(switch_status[r->switch_case]);
    Serial.println(switch_label[relay_ID]);
    _switch_cases[relay_ID] = r->switch_case;
  }
}
