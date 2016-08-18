#ifndef RELAY
#define RELAY

#include <Arduino.h>

// relay struct
typedef struct {    //struct for relay auto/manual/off control
  int ID;
  int auto_pin;
  int manual_pin;
  int relay_pin;
  int switch_case;
} relay;

extern volatile relay* waterfall;
extern volatile relay* uv;
extern volatile relay* mist;
extern volatile relay* r_array[3]; //right to left on hood, in order of closest -> furthest from user

void switch_state(int relay_ID);
void pin_setup();
void auto_mode(int relay_ID, int relay_pin);
#endif
