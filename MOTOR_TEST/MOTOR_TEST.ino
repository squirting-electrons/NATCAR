/*
This program tests the motor.
It will run forward from PWM = 90 to 255 and back down to 90.
Then it will run backward from PWM = 90 to 255 and back down to 90.
The loop will be called 20 times and then the motor should stop.
PWM values will be printed on console.
*/

const int motor_out1 = 10;  //forward pin
const int motor_out2 = 11;  //backward pin
int increment = true;  //increase PWM if true
int reverse = false;  //run backward if true
int terminate = false;  //stop the motor if true
int count = 0;  //count the number of times loop is called
int v = 0;  //initial PWM value

void setup()                    
{
  Serial.begin(9600);           // set up Serial library at 9600 bps
  pinMode(motor_out1, OUTPUT);
  pinMode(motor_out2, OUTPUT);
}

int getPWM() {
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
    if(v <= 90) {
      reverse = !reverse;
      increment = true;
    }
  }
  
  return v;
}

int motorFoward() {
  int v1 = getPWM();
  analogWrite(motor_out2, 0);
  analogWrite(motor_out1, v1);
  delay(5000);
}

int motorBackward() {
  int v2 = getPWM();
  analogWrite(motor_out1, 0);
  analogWrite(motor_out2, v2);
  delay(5000);
}

void motorStop() {
  analogWrite(motor_out1, 0);
  analogWrite(motor_out2, 0);
  delay(1000);
}

void loop() 
{
  count++;
  if(count == 20)
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
