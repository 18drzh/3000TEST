#ifndef __RELAY_CONTROL_H
#define __RELAY_CONTROL_H
#include "main.h"
#include <stdint.h>

#define ULTRASONIC1_TRIG_GPIO_Port GPIOC
#define ULTRASONIC1_TRIG_Pin       GPIO_PIN_4
#define ULTRASONIC1_ECHO_GPIO_Port GPIOC
#define ULTRASONIC1_ECHO_Pin       GPIO_PIN_5

#define ULTRASONIC2_TRIG_GPIO_Port GPIOA
#define ULTRASONIC2_TRIG_Pin       GPIO_PIN_2
#define ULTRASONIC2_ECHO_GPIO_Port GPIOA
#define ULTRASONIC2_ECHO_Pin       GPIO_PIN_3

float Get_Distance(void);
float Get_Distance_2(void);
void Bed_Updown(uint8_t dir);




#endif
