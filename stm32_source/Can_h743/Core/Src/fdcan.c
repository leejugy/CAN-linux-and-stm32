/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    fdcan.c
  * @brief   This file provides code for the configuration
  *          of the FDCAN instances.
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
/* Includes ------------------------------------------------------------------*/
#include "fdcan.h"

/* USER CODE BEGIN 0 */
#include "usart.h"
#include "isotp.h"
/* USER CODE END 0 */

FDCAN_HandleTypeDef hfdcan1;

/* FDCAN1 init function */
void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = ENABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = ENABLE;
  hfdcan1.Init.NominalPrescaler = 6;
  hfdcan1.Init.NominalSyncJumpWidth = 4;
  hfdcan1.Init.NominalTimeSeg1 = 14;
  hfdcan1.Init.NominalTimeSeg2 = 5;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 1;
  hfdcan1.Init.DataTimeSeg1 = 1;
  hfdcan1.Init.DataTimeSeg2 = 1;
  hfdcan1.Init.MessageRAMOffset = 0;
  hfdcan1.Init.StdFiltersNbr = 2;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.RxFifo0ElmtsNbr = 32;
  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxFifo1ElmtsNbr = 0;
  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxBuffersNbr = 0;
  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.TxEventsNbr = 1;
  hfdcan1.Init.TxBuffersNbr = 0;
  hfdcan1.Init.TxFifoQueueElmtsNbr = 32;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */

  /* USER CODE END FDCAN1_Init 2 */

}

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspInit 0 */

  /* USER CODE END FDCAN1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* FDCAN1 clock enable */
    __HAL_RCC_FDCAN_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**FDCAN1 GPIO Configuration
    PA11     ------> FDCAN1_RX
    PA12     ------> FDCAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* FDCAN1 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
  /* USER CODE BEGIN FDCAN1_MspInit 1 */

  /* USER CODE END FDCAN1_MspInit 1 */
  }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspDeInit 0 */

  /* USER CODE END FDCAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_FDCAN_CLK_DISABLE();

    /**FDCAN1 GPIO Configuration
    PA11     ------> FDCAN1_RX
    PA12     ------> FDCAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* FDCAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
  /* USER CODE BEGIN FDCAN1_MspDeInit 1 */

  /* USER CODE END FDCAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
static IsoTpLink can1_link1 = {0, };
uint8_t can1_link1_recv[ISOTP_MAX_SIZE] = {0, };
uint8_t can1_link1_send[ISOTP_MAX_SIZE] = {0, };
int sizeof_candl(int can_size)
{
	if(can_size <= 8)
	{
		return can_size;
	}
	else if(can_size == 12)
	{
		return FDCAN_DLC_BYTES_12;
	}
	else if (can_size == 16)
	{
		return FDCAN_DLC_BYTES_16;
	}
	else if (can_size == 20)
	{
		return FDCAN_DLC_BYTES_20;
	}
	else if (can_size == 24)
	{
		return FDCAN_DLC_BYTES_24;
	}
	else if (can_size == 32)
	{
		return FDCAN_DLC_BYTES_32;
	}
	else if (can_size == 48)
	{
		return FDCAN_DLC_BYTES_48;
	}
	else if (can_size == 64)
	{
		return FDCAN_DLC_BYTES_64;
	}

	return -1;
}

int sizeof_can(int can_size)
{
	switch(can_size)
	{
	case FDCAN_DATA_BYTES_8:
		return 8;
	case FDCAN_DATA_BYTES_12:
		return 12;
	case FDCAN_DATA_BYTES_16:
		return 16;
	case FDCAN_DATA_BYTES_20:
		return 20;
	case FDCAN_DATA_BYTES_24:
		return 24;
	case FDCAN_DATA_BYTES_32:
		return 32;
	case FDCAN_DATA_BYTES_48:
		return 48;
	case FDCAN_DATA_BYTES_64:
		return 64;
	default:
		return -1;
	}
}

void can1_init()
{
	FDCAN_FilterTypeDef can1_filter = {0, };

	can1_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	can1_filter.FilterID1 = 0x01;
	can1_filter.FilterID2 = 0x02;
	can1_filter.FilterIndex = 0;
	can1_filter.FilterType = FDCAN_FILTER_RANGE;
	can1_filter.IdType = FDCAN_STANDARD_ID;
	can1_filter.IsCalibrationMsg = 0;
	can1_filter.RxBufferIndex = 0;

	if(HAL_FDCAN_ConfigFilter(&hfdcan1, &can1_filter) != HAL_OK)
	{
		printr("fail to configure filter in can 1");
		Error_Handler();
	}

	if(HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
	{
		printr("fail to configure global filter in can 1");
		Error_Handler();
	}

	if(HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_BUS_OFF, 0) != HAL_OK)
	{
		printr("fail to activate Notification");
		Error_Handler();
	}

	if(HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
	{
		printr("fail to start in can 1");
		Error_Handler();
	}

	isotp_init_link(&can1_link1, ISOTP_ID, can1_link1_send, sizeof(can1_link1_send), can1_link1_recv, sizeof(can1_link1_recv));
}

void  HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs)
{
	if ((ErrorStatusITs & FDCAN_IE_BOE) != 0) /* If Bus-Off error occured */
	{
		hfdcan->Instance->CCCR &= FDCAN_CCCR_INIT; /* set fdcan init */
	}
}

static int can1_recv(uint8_t *buf, uint32_t buf_size, uint16_t *id)
{
	FDCAN_RxHeaderTypeDef can1_rx = {0, };

	if(buf_size != sizeof_can(hfdcan1.Init.RxFifo0ElmtSize))
	{
		printr("element size is not equal to %d", sizeof_can(hfdcan1.Init.RxFifo0ElmtSize));
		return -1;
	}

	if(HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) <= 0)
	{
		return -1;
	}

	if(HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &can1_rx, buf) != HAL_OK)
	{
		printr("can 1 get rx message fail");
		return -1;
	}

	*id = can1_rx.Identifier;
	return sizeof_can(hfdcan1.Init.RxFifo0ElmtSize);
}

int can1_send(uint8_t *buf, uint32_t buf_size, int send_id)
{
	FDCAN_TxHeaderTypeDef can1_tx = {0, };

	if(buf_size != sizeof_can(hfdcan1.Init.TxElmtSize))
	{
		printr("element size is not equal to %d", sizeof_can(hfdcan1.Init.RxFifo0ElmtSize));
		return -1;
	}

	can1_tx.BitRateSwitch = FDCAN_BRS_OFF;
	can1_tx.DataLength = sizeof_candl(buf_size);
	can1_tx.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	can1_tx.FDFormat = FDCAN_CLASSIC_CAN;
	can1_tx.IdType = FDCAN_STANDARD_ID;
	can1_tx.Identifier = send_id;
	can1_tx.MessageMarker = 0;
	can1_tx.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;
	can1_tx.TxFrameType = FDCAN_DATA_FRAME;

	if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &can1_tx, buf) != HAL_OK)
	{
		printr("fail to can 1 add message to tx fifioq");
		return -1;
	}

	return sizeof_can(hfdcan1.Init.RxFifo0ElmtSize);
}

void can1_work()
{
	uint8_t recv[CLASSIC_CAN_SIZE] = {0, };
	uint8_t can1_isotp_recv1[ISOTP_MAX_SIZE] = {0, };
	uint8_t can1_isotp_send1[ISOTP_MAX_SIZE] = "hellow world! this is isotp can 1 send data.";
	uint16_t id = 0;
	uint16_t out_size = 0;
	int ret = 0;

	static int init_flag = 0;
	static user_timer_t user_timer =
	{
		.timer = 0,
		.condition = 1000
	};

	static user_timer_t reset_timer =
	{
		.timer = 0,
		.condition = 100
	};

	if((hfdcan1.Instance->CCCR & FDCAN_CCCR_INIT_Msk) == FDCAN_CCCR_INIT && init_flag == 0)
	{
		/* reset init */
		init_flag = 1;
	}

	if(init_flag == 1)
	{
		HAL_FDCAN_DeInit(&hfdcan1);
		HAL_FDCAN_Init(&hfdcan1);
		reset_timer.timer = HAL_GetTick();
		init_flag = 2;
		return;
	}

	else if(init_flag == 2)
	{
		check_timer(&reset_timer)
		{
			can1_init();
			init_flag = 0;
		}
		return;
	}

	if ((ret = can1_recv(recv, sizeof(recv), &id)) > 0)
	{
		isotp_on_can_message(&can1_link1, recv, ret);
	}
	isotp_poll(&can1_link1);
	ret = isotp_receive(&can1_link1, can1_isotp_recv1, sizeof(can1_isotp_recv1), &out_size);
	if (ISOTP_RET_OK == ret)
	{
		printb("can 1 recv : %s", can1_isotp_recv1);
	}

	check_timer(&user_timer)
	{
		user_timer.timer = HAL_GetTick();
		isotp_send(&can1_link1, can1_isotp_send1, strlen((char *)can1_isotp_send1));
	}
}
/* USER CODE END 1 */
