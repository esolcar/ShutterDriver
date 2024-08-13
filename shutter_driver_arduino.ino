//Arduino PWM Speed Control：
int E1 = 5;   // speed control
int M1 = 4;   // direction control
int E2 = 6;
int M2 = 7;

bool TTL1_previous = false;
bool TTL2_previous = false;

int threshold = 1020; // (Has to be > 1000 to register TTLs properly as there is somehow a crosstalk between analog pins. It's maybe something to check.) [Analog channel from 0(0V)-1023(5V)]
int pulse_duration = 30; // in ms. Can be modified so long as its not too long to fry the shutters.

void setup() {
  analogReference(DEFAULT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT); 

  // Serial.begin(9600); // Uncomment for debugging with serial monitoring.
}

void loop() {
  // TTL´s are read in an analog channel. We create the truth values.
  bool TTL1_current = analogRead(A1) > threshold; //TTL1
  bool TTL2_current = analogRead(A0) > threshold; //TTL2

  // Detecting state change and its direction
  bool up1 = !TTL1_previous && TTL1_current;
  bool up2 = !TTL2_previous && TTL2_current;
  bool down1 = TTL1_previous && !TTL1_current;
  bool down2 = TTL2_previous && !TTL2_current;

  // When state changes, we drive the shutters
  if(up1 || down1 || up2 || down2){
    digitalWrite(M1,down1*LOW + up1*HIGH); // assigning the sign of the shutter move
    digitalWrite(E1,(up1 + down1)*HIGH); // drive when state changes
    digitalWrite(M2,down2*LOW + up2*HIGH); // assigning the sign of the shutter move
    digitalWrite(E2,(up2 + down2)*HIGH); // drive when state changes
    delay(pulse_duration); // the shutters require a short pulse to change state 
    digitalWrite(M1,0);
    digitalWrite(E1,0);
    digitalWrite(M2,0);
    digitalWrite(E2,0);
  }

  // Keeping track of the previous truth value of the TTLs.
  TTL1_previous = TTL1_current;
  TTL2_previous = TTL2_current;
}