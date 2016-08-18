#ifndef CONSTANTS
#define CONSTANTS

#include <Arduino.h>
#include "relay.h"

// RTC constants
//static volatile int today = 0;
//static volatile int day_of_year = 0;
static const int days_in_month[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
//static volatile current_minutes = 0;

// Sunrise / set constants
// sunrise (0) and set (1) variables
static const int late_sun[] = {7*60 + 35, 21*60 + 7};
static const int early_sun[] = {5*60 + 41, 16*60 + 46};
//static volatile float sunrise = 0;
//static volatile float sunset = 0;

// sin constants
static const float A[] = {floor((late_sun[0] - early_sun[0]) / 2.0), floor((late_sun[1] - early_sun[1]) / 2.0)};  // amplitude
static const float B =  2.0*3.14159 / 365.0;                                                                      // period
//x = 355                                                                                                         // CURRENT DAY
static const float C[] = {264, 81};                                                                               // day offset
static const float D[] = {early_sun[0], early_sun[1]};                                                            // time offset

// Set relay pins
static const int _auto_pins[] = {2,4,6};
static const int _manual_pins[] = {3,5,7};
static const int _relay_pins[] = {8,9,10};
static volatile int _switch_cases[] = {2,2,2}; //0 auto, 1 manual, 2 off, 3 error
static const char* switch_status[] = {"AUTO ON RELAY ","MANUAL ON RELAY ","OFF RELAY ","ERROR: RELAY SWITCH NOT READ"};
static const char* switch_label[] = {"Waterfall","UV Light","Mist"};

#endif
