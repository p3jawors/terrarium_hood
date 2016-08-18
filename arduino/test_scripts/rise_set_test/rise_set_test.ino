#include <TimeLord.h>

// Example by Nick Gammon
// http://forum.arduino.cc/index.php?topic=129249.msg972860#msg972860

// what is our longitude (west values negative) and latitude (south values negative)
float const LONGITUDE = 43.44;
float const LATITUDE = -80.52;

void setup()
  {
    
  Serial.begin (115200);
  
  TimeLord tardis; 
  tardis.TimeZone(-5 * 60); // tell TimeLord what timezone your RTC is synchronized to. You can ignore DST
  // as long as the RTC never changes back and forth between DST and non-DST
  tardis.Position(LATITUDE, LONGITUDE); // tell TimeLord where in the world we are

  // {sec, min, hour, day, month, year}
  byte today[] = {  49, 26, 12, 13, 8, 2016    }; // store today's date (at noon) in an array for TimeLord to use

//if (tardis.SunRise(today)) // if the sun will rise today (it might not, in the [ant]arctic)
   //{
   Serial.print("Sunrise: ");
   Serial.print((int) today[tl_hour]);
   Serial.print(":");
   Serial.println((int) today[tl_minute]);
  // }
   
  // if (tardis.SunSet(today)) // if the sun will set today (it might not, in the [ant]arctic)
  // {
   Serial.print("Sunset: ");
   Serial.print((int) today[tl_hour]);
   Serial.print(":");
   Serial.println((int) today[tl_minute]);
  // }
   Serial.println(); 
  }
  
void loop() 
  {
  }
