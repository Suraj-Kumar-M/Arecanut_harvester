#include <Servo.h>

Servo servo1;
Servo servo2;

const int servoPin1 = A4;
const int servoPin2 = A5;

const int mc_rpwm   = 3 ;  // RPWM1
const int mc_lpwm   = 5 ;  // LPWM1
const int mc_ren    = 6 ;  // R_EN1
const int mc_len    = 7 ;  // L_EN1

const int motor_b1  = 4 ; // RPWM3

const int potPin1   = 13; // RC input pins
const int potPin2   = 12;
const int potPin3   = 11;
const int potPin4   = 10;
const int potPin5   = 9 ;
const int potPin6   = 8 ;

int motorPwm1 = 0;
int motorPwm2 = 0;
int motorPwm3 = 0;
int motorPwm4 = 0;
int motorPwm5 = 0;
int motorPwm6 = 0;

void setup() {
  Serial.begin(9600);
  
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);

  pinMode(potPin1, INPUT );
  pinMode(potPin2, INPUT );
  pinMode(potPin3, INPUT );
  pinMode(potPin4, INPUT );
  pinMode(potPin5, INPUT );
  pinMode(potPin6, INPUT );
  pinMode(mc_rpwm, OUTPUT);
  pinMode(mc_lpwm, OUTPUT);
  pinMode(mc_ren , OUTPUT);
  pinMode(mc_len , OUTPUT);
  pinMode(motor_b1,OUTPUT);
  
  digitalWrite(mc_rpwm , LOW );
  digitalWrite(mc_lpwm , LOW );
  digitalWrite(mc_ren  , LOW );
  digitalWrite(mc_len  , LOW );
  digitalWrite(motor_b1, HIGH);
}

void loop() 
{
  readPots();
  controlMotors();
  controlServos();
}

void readPots() 
{ 
  motorPwm1 = map(pulseIn(potPin1, HIGH, 25000), 1100, 2000, 0, 255);
  motorPwm4 = map(pulseIn(potPin4, HIGH, 25000), 1100, 2000, 0, 255);
  motorPwm3 = map(pulseIn(potPin3, HIGH, 25000), 1100, 2000, 0, 255);
  motorPwm6 = map(pulseIn(potPin6, HIGH, 25000), 1100, 2000, 0, 255);  
  motorPwm5 = map(pulseIn(potPin5, HIGH, 25000), 1100, 2000, 0, 255);
  motorPwm2 = map(pulseIn(potPin2, HIGH, 25000), 1100, 2000, 0, 255);
  
  Serial.println(motorPwm1);
  Serial.println(motorPwm2);
  Serial.println(motorPwm3);
  Serial.println(motorPwm4);
  Serial.println(motorPwm5);
  Serial.println(motorPwm6);
  Serial.println("###"    );  
}

void controlMotors() 
{ 
  // Climbing motor
  if (motorPwm6 > 0) 
  {
    if (motorPwm6 > 150) {
      up(mc_rpwm,mc_lpwm,mc_ren,mc_len);
    } else if (motorPwm6 < 100) {
      down(mc_rpwm,mc_lpwm,mc_ren,mc_len);
    } else {
      stop(mc_rpwm,mc_lpwm,mc_ren,mc_len);
    }
  }
  // Rotating saw blade motor
  if (motorPwm3 > 0) 
  {
    digitalWrite(motor_b1, motorPwm3 > 150 ? LOW : HIGH);
  }
}

void controlServos() 
{
  moveServo(servo1, potPin5, 0, 180);
  moveServo(servo2, potPin2, 0, 180);
}

void moveServo(Servo servo, int pin, int minAngle, int maxAngle) 
{
  int angle = map(pulseIn(pin, HIGH, 25000), 1000, 2000, minAngle, maxAngle);
  servo.write(angle);
}

void up(int r_pwm,int l_pwm,int r_en,int l_en) 
{
  digitalWrite(r_en , HIGH);
  digitalWrite(l_en , HIGH);  
  digitalWrite(l_pwm, LOW );
  digitalWrite(r_pwm, HIGH);   
}

void down(int r_pwm,int l_pwm,int r_en,int l_en) 
{
  digitalWrite(r_en , HIGH);
  digitalWrite(l_en , HIGH);  
  digitalWrite(r_pwm, LOW );
  digitalWrite(l_pwm, HIGH);
}  

void stop(int r_pwm,int l_pwm,int r_en,int l_en) 
{
  digitalWrite(r_en, LOW);
  digitalWrite(l_en, LOW);
  digitalWrite(r_pwm,LOW);
  digitalWrite(l_pwm,LOW);  
}
