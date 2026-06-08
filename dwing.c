#include "dwing.h"
#include <stdint.h>
#include "usart.h"

float current_height = 0.0f;
float current_height_2 = 0.0f;

static void Report_Distance(const char *label, float distance)
{
	if (distance == -1.0f) {
		uart_printf("%s: ERR echo rising timeout\r\n", label);
	} else if (distance == -2.0f) {
		uart_printf("%s: ERR echo stuck high\r\n", label);
	} else if (distance == -3.0f) {
		uart_printf("%s: ERR echo idle high\r\n", label);
	} else {
		uart_printf("%s: %.2f cm\r\n", label, distance);
	}
}

void Dwin_Now_Height(void){
		current_height = Get_Distance();
		Report_Distance("US1", current_height); 
		HAL_Delay(60); //60ms防止两个超声波冲突

		current_height_2 = Get_Distance_2();
		Report_Distance("US2", current_height_2);
			
		//一号床
		uint16_t value2 = (uint16_t)(current_height*100 + 0.5f);
		uint8_t buf2[8] = {0x5A,0xA5,0x05,0x82,0x16,0x00,(uint8_t)(value2 >> 8),(uint8_t)(value2 & 0xFF)}; // 05：后面数据长度 82：写变量  	
		HAL_UART_Transmit(&huart6,buf2,8,100);
	
		//二号床
		uint16_t value = (uint16_t)(current_height_2*100 + 0.5f);
		uint8_t buf[8] = {0x5A,0xA5,0x05,0x82,0x15,0x00,(uint8_t)(value >> 8),(uint8_t)(value & 0xFF)}; // 05：后面数据长度 82：写变量  	
		HAL_UART_Transmit(&huart6,buf,8,100);

		
}
