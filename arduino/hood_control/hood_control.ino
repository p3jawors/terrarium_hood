// Relays are active low (set pins to low to turn relay on)

#include <Wire.h>
#include "RTClib.h"
#include "relay.h"
#include "constants.h"
#include "sun.h"
RTC_DS1307 RTC;
//DateTime now = RTC.now();

void rtc_setup();
void print_time();

//void auto_mode(int relay_ID, int relay_pin);

//============ SETUP ================
void setup() {
  Serial.begin(57600);
  //setup pins
  pin_setup();
  Serial.println("Pins setup");
  //set up RTC
  rtc_setup();
  Serial.println("RTC setup");
}

//=========== MAIN LOOP ==============
void loop() {
  DateTime now = RTC.now();
  if (now.day() != sun->today){
    Serial.println("Enjoy the new day!");
    Serial.print("It is now ");
    print_time();
    
    sun->day_of_year = days_in_month[now.month()-1] + now.day();
    sun->rise = A[0]*sin(B*(sun->day_of_year - C[0]))+D[0];
    sun->set = A[1]*sin(B*(sun->day_of_year - C[1]))+D[1];
    sun->today = now.day();
    morning_misted = false;
    evening_misted = false;
    Serial.print("Today's sunrise time: ");
    Serial.println(sun->rise);
    Serial.print("Today's sunset time: ");
    Serial.println(sun->set);
  }
  
  //-------------check switch state
  for (int ii=0; ii<3; ii++){
    switch_state(ii);
  }
  delay(50); // delay for switch debouncing
}

//=========== Functions ===============
void rtc_setup(){
  // Set RTC
  Wire.begin();
  RTC.begin();
  DateTime now = RTC.now();
  
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  // set sun variables
  sun->day_of_year = days_in_month[now.month()-1] + now.day();
  sun->rise = A[0]*sin(B*(sun->day_of_year - C[0]))+D[0];
  sun->set = A[1]*sin(B*(sun->day_of_year - C[1]))+D[1];
  sun->today = now.day();
  
  Serial.print("Today's sunrise time: ");
  Serial.print(floor(sun->rise/60));
  Serial.print(":");
  Serial.println(sun->rise%60);
  
  Serial.print("Today's sunset time: ");
  Serial.print(floor(sun->set/60));
  Serial.print(":");
  Serial.println(sun->set%60);
}

void print_time(){
  DateTime now = RTC.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  //Serial.println("--------------------------------------");
}

void auto_mode(int relay_ID, int relay_pin) {
  DateTime now = RTC.now();
  sun->current_minutes = now.hour() * 60 + now.minute();
  Serial.print("Current minute of the day: ");
  Serial.println(sun->current_minutes);

  // Check which relay is being switched
  switch (relay_ID){
    // ---------WATERFALL AUTO MODE---------
    case 0:
      Serial.println("WATERFALL AUTO CASE ACTIVATED");
      break;
      
    // ------------UV AUTO MODE-------------
    case 1:
      Serial.println("UV AUTO CASE ACTIVATED");
      if (sun->current_minutes > sun->rise && sun->current_minutes < sun->set){
        digitalWrite(relay_pin, LOW); //active low relays
      }
      else {
        digitalWrite(relay_pin, HIGH);
      }
      break;
      
    // -----------MIST AUTO MODE------------
    case 2:
      Serial.println("MIST AUTO CASE ACTIVATED");
      if (sun->current_minutes > sun->rise && morning_misted == false) {   //morning misting
        digitalWrite(relay_pin, LOW); //active low relays
        delay(mist_length);
        digitalWrite(relay_pin, HIGH);
        morning_misted = true;
      }
      else if (sun->current_minutes > sun->set && evening_misted == false) { //evening misting
        digitalWrite(relay_pin, LOW); //active low relays
        delay(mist_length);
        digitalWrite(relay_pin, HIGH);
        evening_misted = true;
      }
      // boolean misted gets reset at start of new day in main loop
      break;

    // ---default if no other case matches---
    default:
      Serial.println("ERROR: AUTO MODE - NO CASE MATCH");
      digitalWrite(relay_pin, HIGH);
      break;
  }
}
