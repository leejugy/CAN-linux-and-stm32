/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define UART_RECV_MAX 256
typedef struct
{
	uint8_t buf[UART_RECV_MAX];
	uint32_t read_index;
}uart_recv_t;

#define printr(fmt, ...) printf("[\x1b[31mERR\x1b[0m]" fmt "\r\n", ##__VA_ARGS__)
#define printg(fmt, ...) printf("[\x1b[32mSUCCESS\x1b[0m]" fmt "\r\n", ##__VA_ARGS__)
#define printy(fmt, ...) printf("[\x1b[33mWARNING\x1b[0m]" fmt "\r\n", ##__VA_ARGS__)
#define printb(fmt, ...) printf("[\x1b[34mDEBUG\x1b[0m]" fmt "\r\n", ##__VA_ARGS__)
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void terminal_init();
void uart1_work();
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

