#ifndef SUN
#define SUN

#include <Arduino.h>

// sunrise/set struct
typedef struct {    //struct for relay auto/manual/off control
  int today;
  int day_of_year;
  int current_minutes;
  int rise;
  int set;
} Sun;

extern volatile Sun* sun;

#endif
