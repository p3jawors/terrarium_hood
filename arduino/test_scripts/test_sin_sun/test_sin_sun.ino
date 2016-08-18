void setup() {
 //sin constants
  volatile float A = (MAX_PW - MIN_PW)/2;
  volatile float period = time_to_record;//in seconds
  volatile float B =  2*3.14159 / period;
  volatile float D = MAX_PW - A;

  // 0 == rise, 1 == set
  late_sun[] = {7*60 + 35, 21*60 + 7};
  early_sun[] = {5*60 + 41, 16*60 + 46};
  
  sun_range[] = floor((late_sun - early_sun) / 2);
  
  A[] = {sun_range{0}, sun_range{1}};
  B =  2*3.14159 / 365;
  x = 355; /*CURRENT DAY*/
  C[] = {274, 92};
  D[] = {early_sun{0}, early_sun{1}};
}

void loop() {
   //sin constants
  sunrise = A{0}*sin(B*(x - C{0}))+D{0};
  sunset = A{1}*sin(B*(x - C{1}))+D{1};
}
