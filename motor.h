// Motor 1
#define MOTOR1_PIN1 5
#define MOTOR1_PIN2 16
#define MOTOR1_PIN3 17
#define MOTOR1_PIN4 4

// Motor 2
#define MOTOR2_PIN1 12
#define MOTOR2_PIN2 27
#define MOTOR2_PIN3 14
#define MOTOR2_PIN4 26

#include <stdint.h>

void setupMotors();
void motor1Task(void *parameter);
void motor2Task(void *parameter);
void controlMotors(int16_t x, int16_t y);
void disable_autopilot();
void enable_autopilot();
