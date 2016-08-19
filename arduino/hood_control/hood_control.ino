// Relays are active low (set pins to low to turn relay on)
#include <Wire.h>
#include "RTClib.h"
#include "relay.h"
#include "constants.h"
#include "sun.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, A0, A1, A2, A3);

// print to serial port
#define debugprint Serial.print
#define debugprintln Serial.println
// turn of printing
//#define debugprint //
//#define debugprintln //

RTC_DS1307 RTC;
//DateTime now = RTC.now();

void rtc_setup();
void print_time();
void lcd_print();

//void auto_mode(int relay_ID, int relay_pin);

//============ SETUP ================
void setup() {
  Serial.begin(57600);
  lcd.clear();
  lcd.begin(20,4);
  //setup pins
  pin_setup();
  debugprintln("Pins setup");
  //set up RTC
  rtc_setup();
  debugprintln("RTC setup");
}

//=========== MAIN LOOP ==============
void loop() {
  DateTime now = RTC.now();
  if (now.day() != sun->today){
    debugprintln("Enjoy the new day!");
    debugprint("It is now ");
    print_time();
    
    sun->day_of_year = days_in_month[now.month()-1] + now.day();
    sun->rise = A[0]*sin(B*(sun->day_of_year - C[0]))+D[0];
    sun->set = A[1]*sin(B*(sun->day_of_year - C[1]))+D[1];
    sun->today = now.day();
    morning_misted = false;
    evening_misted = false;
    debugprint("Today's sunrise time: ");
    debugprintln(sun->rise);
    debugprint("Today's sunset time: ");
    debugprintln(sun->set);
  }
  
  //-------------check switch state
  for (int ii=0; ii<3; ii++){
    switch_state(ii);
  }
  
  lcd_print();
  delay(50); // delay for switch debouncing and to see lcd
}

//================================ Functions ==========================================
void rtc_setup(){
  // Set RTC
  Wire.begin();
  RTC.begin();
  DateTime now = RTC.now();
  
  if (! RTC.isrunning()) {
    debugprintln("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  // set sun variables
  sun->day_of_year = days_in_month[now.month()-1] + now.day();
  sun->rise = A[0]*sin(B*(sun->day_of_year - C[0]))+D[0];
  sun->set = A[1]*sin(B*(sun->day_of_year - C[1]))+D[1];
  sun->today = now.day();
  
  debugprint("Today's sunrise time: ");
  debugprint(floor(sun->rise/60));
  debugprint(":");
  debugprintln(sun->rise%60);
  
  debugprint("Today's sunset time: ");
  debugprint(floor(sun->set/60));
  debugprint(":");
  debugprintln(sun->set%60);
}

void print_time(){
  DateTime now = RTC.now();
  debugprint(now.year(), DEC);
  debugprint('/');
  debugprint(now.month(), DEC);
  debugprint('/');
  debugprint(now.day(), DEC);
  debugprint(' ');
  debugprint(now.hour(), DEC);
  debugprint(':');
  debugprint(now.minute(), DEC);
  debugprint(':');
  debugprint(now.second(), DEC);
  //debugprintln("--------------------------------------");
}

void auto_mode(int relay_ID, int relay_pin) {
  DateTime now = RTC.now();
  sun->current_minutes = now.hour() * 60 + now.minute();
  debugprint("Current minute of the day: ");
  debugprintln(sun->current_minutes);

  // Check which relay is being switched
  switch (relay_ID){
    // ---------WATERFALL AUTO MODE---------
    case 0:
      debugprintln("WATERFALL AUTO CASE ACTIVATED");
      break;
      
    // ------------UV AUTO MODE-------------
    case 1:
      debugprintln("UV AUTO CASE ACTIVATED");
      if (sun->current_minutes > sun->rise && sun->current_minutes < sun->set){
        digitalWrite(relay_pin, LOW); //active low relays
      }
      else {
        digitalWrite(relay_pin, HIGH);
      }
      break;
      
    // -----------MIST AUTO MODE------------
    case 2:
      debugprintln("MIST AUTO CASE ACTIVATED");
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
      debugprintln("ERROR: AUTO MODE - NO CASE MATCH");
      digitalWrite(relay_pin, HIGH);
      break;
  }
}

void lcd_print() {
  DateTime now = RTC.now();
  if(digitalRead(lcd_mode) == LOW){ // print default screen
    if(lcd_switch == 1){
      lcd.clear();
      lcd_switch = 0;
    }
    lcd.setCursor(0,0);
    lcd.print("Property of Skittles");

    // Date and Time
    lcd.setCursor(0,1);
    lcd.print(now.year(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.day(), DEC);
    lcd.print(' ');
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.print(now.second(), DEC);

    // Sunrise time
    lcd.setCursor(0,2);
    lcd.print("Sunrise:   ");
    lcd.print(floor(sun->rise/60));
    lcd.setCursor(12,2);
    lcd.print(":");
    lcd.print(sun->rise%60);

    // Sunset time
    lcd.setCursor(0,3);
    lcd.print("Sunset :  ");
    lcd.print(floor(sun->set/60));
    lcd.setCursor(12,3);
    lcd.print(":");
    lcd.print(sun->set%60);
  }
  else{
    if(lcd_switch == 0 || switch_case_sum != waterfall->switch_case + uv->switch_case + mist->switch_case ){
      lcd.clear();
      lcd_switch = 1;
    }

    lcd.setCursor(0,0);
    lcd.print("Waterfall: ");
    lcd.print(switch_status[waterfall->switch_case]);

    lcd.setCursor(0,1);
    lcd.print("UV Light : ");
    lcd.print(switch_status[uv->switch_case]);
    
    lcd.setCursor(0,2);
    lcd.print("Mist     : ");
    lcd.print(switch_status[mist->switch_case]);

    switch_case_sum = waterfall->switch_case + uv->switch_case + mist->switch_case ;
  }
}
