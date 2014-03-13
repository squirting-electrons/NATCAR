#include <SoftwareServo.h>
#include <Servo.h>


//PWM enabled digital pins: 3,5(not too good),6(not good either),9,10,11,13
//Analog inputs: A0-A5, A6-A11 (on digital pins: 4,6,7,8,9,10,12)
#define motorDRIVE    9    //forward PWM
#define motorREVERSE  10   //reverse PWM 
#define servo         3    //servo control signal
#define leftAFE       A0   //AFE left
#define centerAFE     A1   //AFE center
#define rightAFE      A2   //AFE right

//constant integers
const int TOLERANCE = 2;    //for little variations in signal
const int PWM_MAX = 255;
  // state definitions

//define useful constants
int i;    //for 'for' loops and such
int input_AFE;  //ranges from [0, 1024)
float turn_angle;  //used to control speed (PID control); in radians
int center;
int left;
int right;

SoftwareServo m_servo;  //create servo object

//define our functions
void servo_control(int left, int center, int right);
void motor_control(int turn_angle);

//Setup pins for specific uses
void setup()
{
  pinMode(motorDRIVE, OUTPUT);
  pinMode(motorREVERSE, OUTPUT);
  pinMode(servo, OUTPUT);
  pinMode(leftAFE, INPUT);
  pinMode(centerAFE, INPUT);
  pinMode(rightAFE, INPUT);
  
  m_servo.attach(servo);  //attach servo to servo object
  
  Serial.begin(9600);    //this will be here for monitoring AFE input
}

void motor_control(int turn_angle)
{
  //it's not this simple; this is just an idea :P
  analogWrite(motorDRIVE, PWM_MAX - 2*turn_angle);
  
  //FAST STOP
  //analogWrite(motor_f, 0);
}

void servo_control(int left, int center, int right)
{
  //map the readings (0 to 1023) to an angle (0 to 179)
  int mapL = map(left, 0, 1023, 0, 179);
  int mapC = map(center, 0, 1023, 0, 179);
  int mapR = map(right, 0, 1023, 0, 179);
  
  /*
  //algorithm for PID control
  turn_angle = angle calculated
  m_servo.write(turn_angle);
  */
}

void loop()
{
  center = analogRead(centerAFE);  
  left = analogRead(leftAFE);
  right = analogRead(rightAFE);
  servo_control(left, center, right);
  motor_control(turn_angle); 
}


