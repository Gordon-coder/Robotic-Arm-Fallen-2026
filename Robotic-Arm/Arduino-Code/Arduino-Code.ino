#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int input_values[6];
int motor_values[6];

int arm_angle = 900;
int claw_angle = 0;

void setup() {
  Serial.begin(9600);
  for (int pin = 2; pin < 8; pin++) {
    pinMode(pin, INPUT);
  }

  pwm.begin();

  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
}

void loop() {
  for (int pin = 2; pin < 8; pin++) 
    input_values[pin-2] = pulseIn(pin, HIGH);

  arm_angle = constrain(arm_angle + (input_values[1]-1500) * 0.1, 0, 900); 
  claw_angle = constrain(claw_angle + (input_values[0]-1500) * 0.1, -450, 450);

  motor_values[0] = constrain(motor_values[0] + (input_values[3]-1500) * 0.1, 500, 2500);
  motor_values[1] = constrain(constrain(input_values[2]-1000, 0, 1000)/1000.0*1350/2.0+600+arm_angle, 600, 2400);
  motor_values[2] = 2400-constrain(input_values[2]-1000, 0, 1000)/1000.0*1350;
  motor_values[3] = constrain(constrain(input_values[2]-1000, 0, 1000)/1000.0*1350/2.0+600+claw_angle, 600, 2400);
  motor_values[4] = input_values[4];


  for (int pin = 0; pin < 6; pin++) 
    pwm.writeMicroseconds(pin, motor_values[pin]);
}