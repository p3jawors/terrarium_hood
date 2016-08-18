const int manual_pin = 2;
const int auto_pin = 3;
const int relay_1 = 4;

void setup() { 
  pinMode(manual_pin, INPUT);
  pinMode(auto_pin, INPUT);
  pinMode(relay_1, OUTPUT);

  digitalWrite(relay_1, HIGH);
  digitalWrite(auto_pin, LOW);
  digitalWrite(manual_pin, LOW);

  Serial.begin(9600);
}

void loop() {
  if (digitalRead(auto_pin) == HIGH){
    Serial.println("auto mode");
    digitalWrite(relay_1, LOW);
    delay(1000);
    digitalWrite(relay_1, HIGH);
    delay(1000);
  }
  else if (digitalRead(manual_pin) == HIGH){
    Serial.println("manual mode");
    digitalWrite(relay_1, LOW);
  }
  else {
    //switch is off, so both manual and auto are off
    Serial.println("off");
    digitalWrite(relay_1, HIGH);
  }

  //delay(1000); //wait a ms each loop to allow time for switch to debounce
}
