//const int pot_in = 0;
const int motor_out1 = 10;
const int motor_out2 = 11;
int increment = true;
int reverse = false;
int terminate = false;
int count = 0;
int v = 0;

void setup()                    
{
  Serial.begin(9600);           // set up Serial library at 9600 bps
  //pinMode(pot_in, INPUT); 
  pinMode(motor_out1, OUTPUT);
  pinMode(motor_out2, OUTPUT);
}

int getPWM() {
  //int v;
  //v = analogRead(pot_in);
  //v = map(v, 0, 1023, 0, 255);  //scaling function: from 10-bit voltage scale to 8-bit PWM scale
  if(increment) {
    v += 50;
    v = max(v, 90); 
    v = min(v, 255);
    if(v >= 250)
      increment = false;
  }
  else {
    v -= 50;
    v = max(v, 90); 
    v = min(v, 255);
    if(v == 90) {
      reverse = !reverse;
      increment = true;
    }
  }
  
  return v;
}

int motorFoward() {
  int v1 = getPWM();
  //Serial.println(v1, DEC);
  analogWrite(motor_out2, 0);
  analogWrite(motor_out1, v1);
  delay(1000);
}

int motorBackward() {
  int v2 = getPWM();
  //Serial.println(v2, DEC);
  analogWrite(motor_out1, 0);
  analogWrite(motor_out2, v2);
  delay(1000);
}

void motorStop() {
  analogWrite(motor_out1, 0);
  analogWrite(motor_out2, 0);
  delay(1000);
}

void loop() 
{
  count++;
  if(count == 40)
    terminate = true;
  if(!reverse && !terminate) {
    motorFoward();
    Serial.print("Forward PWM: ");
    Serial.println(v, DEC);
    Serial.print("\n");
  }
  else if(reverse && !terminate) {
    motorBackward();
    Serial.print("Backward PWM: ");
    Serial.println(v, DEC);
    Serial.print("\n");
  }
  else {
    motorStop();
    Serial.print("Motor is stopped.");
    Serial.print("\n");
  }
}
