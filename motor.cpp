#include "motor.h"
#include <AccelStepper.h>
#include <FastLED.h>
#include "led.h"

AccelStepper motor1(AccelStepper::FULL4WIRE, MOTOR1_PIN1, MOTOR1_PIN2, MOTOR1_PIN3, MOTOR1_PIN4);
AccelStepper motor2(AccelStepper::FULL4WIRE, MOTOR2_PIN1, MOTOR2_PIN2, MOTOR2_PIN3, MOTOR2_PIN4);
bool do_autopilot;
extern CRGB leds[NUM_LEDS];

void setupMotors() {
  motor1.setMaxSpeed(1000);
  motor1.setAcceleration(1000);
  motor1.setSpeed(0);

  motor2.setMaxSpeed(1000);
  motor2.setAcceleration(1000);
  motor2.setSpeed(0);
}

void motor1Task(void *parameter) {
  while (1) {
    motor1.runSpeed();
    vTaskDelay(1);
  }
}

void motor2Task(void *parameter) {
  while (1) {
    motor2.runSpeed();
    vTaskDelay(1);
  }
}

const float MAX_SPEED_FACTOR = 0.7; // Adjust this value as needed (e.g., 0.85 for 85% of maximum speed)

void controlMotors(int16_t x, int16_t y) {
  int16_t motor1Speed, motor2Speed;

  // Calculate the scaling factor based on the joystick position
  float scale = sqrt(pow(x, 2) + pow(y, 2)) / 1000.0 * MAX_SPEED_FACTOR;
	//foreward
	if(x>0 && y<0){
		Serial.println("forward");
		leds[FRONT_LEFT] = CRGB::FloralWhite;
		leds[FRONT_LEFT_MID] = CRGB::Green;

		leds[FRONT_RIGHT] = CRGB::FloralWhite;
		leds[FRONT_RIGHT_MID] = CRGB::Green;

		leds[BACK_LEFT] = CRGB::Red;
		leds[BACK_LEFT_MID] = CRGB::Green;

		leds[BACK_RIGHT] = CRGB::Red;
		leds[BACK_RIGHT_MID] = CRGB::Green;
	}
	//backward
	else if(x<0 && y>0){
		Serial.println("back");
		leds[FRONT_LEFT] = CRGB::FloralWhite;
		leds[FRONT_LEFT_MID] = CRGB::Green;

		leds[FRONT_RIGHT] = CRGB::FloralWhite;
		leds[FRONT_RIGHT_MID] = CRGB::Green;

		leds[BACK_LEFT] = CRGB::White;
		leds[BACK_LEFT_MID] = CRGB::Green;

		leds[BACK_RIGHT] = CRGB::White;
		leds[BACK_RIGHT_MID] = CRGB::Green;
	}
	//turning right
	else if(x>0 && y>0){
		Serial.println("right");
		leds[FRONT_LEFT] = CRGB::FloralWhite;
		leds[FRONT_LEFT_MID] = CRGB::Green;

		leds[FRONT_RIGHT] = CRGB::Yellow;
		leds[FRONT_RIGHT_MID] = CRGB::Green;

		leds[BACK_LEFT] = CRGB::Red;
		leds[BACK_LEFT_MID] = CRGB::Green;

		leds[BACK_RIGHT] = CRGB::Red;
		leds[BACK_RIGHT_MID] = CRGB::Green;
	}
	//turning left
	else{
		Serial.println("left");
		leds[FRONT_LEFT] = CRGB::Yellow;
		leds[FRONT_LEFT_MID] = CRGB::Green;

		leds[FRONT_RIGHT] = CRGB::FloralWhite;
		leds[FRONT_RIGHT_MID] = CRGB::Green;

		leds[BACK_LEFT] = CRGB::Red;
		leds[BACK_LEFT_MID] = CRGB::Green;

		leds[BACK_RIGHT] = CRGB::Red;
		leds[BACK_RIGHT_MID] = CRGB::Green;
	}
	FastLED.show();	

  if (y >= 0) {
    // Forward or turning (both motors in the same direction)
    motor1Speed = scale * (y - x);
    motor2Speed = scale * (y + x);
  } else {
    // Backward or turning (both motors in opposite directions)
    motor1Speed = scale * (y + x);
    motor2Speed = scale * (y - x);
  }

  // Reverse one of the motors since they face outward
  // motor1Speed = -motor1Speed;

  motor1.setSpeed(motor1Speed);
  motor2.setSpeed(motor2Speed);
}


void enable_autopilot(){
	do_autopilot = true;
}

void disable_autopilot(){
	do_autopilot = false;
	controlMotors(0,0);
}

