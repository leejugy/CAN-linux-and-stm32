/*
 * isotp_user.c
 *
 *  Created on: Jun 29, 2025
 *      Author: leejunggyun
 */


#include "isotp.h"
#include "fdcan.h"
#include "usart.h"

#define DEBUG_MSG_MAX 1024

void isotp_user_debug(const char* message, ...)
{
	char debug_msg[DEBUG_MSG_MAX] = {0, };
	va_list va = {0, };
	va_start(va, message);
	vsnprintf(debug_msg, sizeof(debug_msg), message, va);
	va_end(va);
	printr("%s", debug_msg);
}

/* user implemented, send can message. should return ISOTP_RET_OK when success.
*/
int isotp_user_send_can(uint32_t arbitration_id, uint8_t* data, uint8_t size)
{
	if(can1_send(data, size, arbitration_id) < 0)
	{
		return ISOTP_RET_ERROR;
	}

	return ISOTP_RET_OK;
}

/* user implemented, get millisecond */
uint32_t isotp_user_get_ms(void)
{
	return HAL_GetTick();
}
