#ifndef __KEY_H
#define __KEY_H
#include "stm32f1xx_hal.h"


//æÿ’Ûº¸≈ÃΩ”œﬂ
#define MATRIX_KEY_PORT_0 GPIOB
#define MATRIX_KEY_PIN_0    GPIO_PIN_9
#define MATRIX_KEY_PORT_1 GPIOB      
#define MATRIX_KEY_PIN_1    GPIO_PIN_8
#define MATRIX_KEY_PORT_2 GPIOB      
#define MATRIX_KEY_PIN_2    GPIO_PIN_7
#define MATRIX_KEY_PORT_3 GPIOB      
#define MATRIX_KEY_PIN_3    GPIO_PIN_6
#define MATRIX_KEY_PORT_4 GPIOB      
#define MATRIX_KEY_PIN_4    GPIO_PIN_12
#define MATRIX_KEY_PORT_5 GPIOB        
#define MATRIX_KEY_PIN_5    GPIO_PIN_3
#define MATRIX_KEY_PORT_6 GPIOB        
#define MATRIX_KEY_PIN_6    GPIO_PIN_4
#define MATRIX_KEY_PORT_7 GPIOB      
#define MATRIX_KEY_PIN_7    GPIO_PIN_5

int Matrix_Key_GetDownNum(void);
#endif






