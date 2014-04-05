//////////////////////////////////////////////////////////////////////////////

#include <Servo.h>

//PWM enabled digital pins: 3,5(not too good),6(not good either),9,10,11,13
//Analog inputs: A0-A5, A6-A11 (on digital pins: 4,6,7,8,9,10,12)
#define motorDRIVE    9    //forward PWM
#define motorREVERSE  10   //reverse PWM 
#define servo         3    //servo control signal
#define leftAFE       A0   //AFE left
#define centerAFE     A1   //AFE center
#define rightAFE      A2   //AFE right

//////////////////////////////////////////////////////////////////////////////

//constant integers
const int MU = 0.85;        //coefficient of friction
const int LENGTH = 0.26;    //arc length (length of car)
const float MAX_SPEED = 9;  //max speed for car
const int MAX_ANGLE = 179;  //max turn angle
const float SCALING_FACTOR = 28.65;

//define useful variables
int i;    //for 'for' loops and such
int sumL, sumC, sumR, baselineL, baselineC, baselineR;
int TOLERANCE;    //for little variations in signal [0-1024)
int MAXREAD;

//AFE
int input_AFE;  //ranges from [0, 1024)

//SERVO
Servo m_servo;  //create servo object
int turn_angle;   //units: radians
int differential;
int mapL, mapC, mapR; 

//MOTOR
float radius, velocity;    
float test_array[1000];

//main
int center, left, right;

//functions
void servo_control(int* left, int* center, int* right);
void motor_control(int* angle);

//////////////////////////////////////////////////////////////////////////////

//Setup pins for specific uses
void setup()
{
  pinMode(motorDRIVE, OUTPUT);
  pinMode(motorREVERSE, OUTPUT);
  pinMode(servo, OUTPUT);
  pinMode(leftAFE, INPUT);
  pinMode(centerAFE, INPUT);
  pinMode(rightAFE, INPUT);
  
  /*
  //sensor calibration
  sumL = 0;
  sumC = 0;
  sumR = 0;
  
  for (i = 0; i < 10; i++)
  {
    sumL += analogRead(leftAFE);
    sumC += analogRead(centerAFE);
    sumR += analogRead(rightAFE);
  }
  baselineL = sumL/10;
  baselineC = sumC/10;
  baselineR = sumR/10;
  
  TOLERANCE = (baselineL - baselineR);
  */
  
  //motor control test array
  for (i = 0; i < 200; i++)
    test_array[i] = 0;
  for (i = 200; i < 1000; i++)
    test_array[i] = .09;
  
  //m_servo.attach(servo);  //attach servo to servo object
  
  Serial.begin(9600);    //this will be here for monitoring AFE input (remove after)
}

//////////////////////////////////////////////////////////////////////////////

void servo_control(int* left, int* center, int* right)
{
  //map the readings (0 to 1023) to an angle (0 to 179)
  mapL = map(*left,0,MAXREAD,0,179);
  mapC = map(*center,0,MAXREAD,0,179);
  mapR = map(*right,0,MAXREAD,0,179);
  
  differential = mapL-mapR;
  //what if we use differential to "predict" the course?
  
  if (differential <= TOLERANCE && differential >= -1*TOLERANCE)  //go straight
  {
    turn_angle = 0;
    m_servo.write (turn_angle);
  }
  
  else if (differential > TOLERANCE)  //turn left
  {
    turn_angle = MAX_ANGLE*differential/baselineC;
    m_servo.write(turn_angle);
  }
  
  else if (differential > -1*TOLERANCE)  //turn right
  {
    turn_angle = MAX_ANGLE*differential/baselineC;
    m_servo.write(turn_angle);
  }
}

//////////////////////////////////////////////////////////////////////////////

void motor_control(int* turn_angle)
{
  radius = LENGTH/(*turn_angle);    //radius = S/theta
  velocity = 0.143*radius*radius*radius 
    - 0.2707*radius*radius
    + 2.1558*radius 
    + .7362;
  analogWrite(motorDRIVE, SCALING_FACTOR*velocity);
}

//////////////////////////////////////////////////////////////////////////////

void loop()
{
  /*
  center = analogRead(centerAFE);  
  left = analogRead(leftAFE);
  right = analogRead(rightAFE);
  servo_control(left, center, right);
  delay(15);
  motor_control(turn_angle); 
  */
  
  //motor control test code 
  for (i = 0; i < 1000; i++)
  {
    //m_servo.writeMicroseconds(700);  //1.5ms
    motor_control(test_array[i]);
    delay(25);
  }
  
}
