﻿/*
 * UART0.c
 *
 * Created: 2022-08-09 오전 10:44:49
 *  Author: kccistc
 */ 

#include "main.h"
#include "UART0.h"

#define  COMMAND_MAX  50    // 최대로 저장할 수 있는 command수
#define  COMMAND_LENGTH 30	// 1 line에 최대로 save 할 수 있는 data 수

volatile int input_pointer = 0;				// ISR(UART0_RX_vect)에서 save하는 index값
volatile int output_pointer = 0;			// pc_command_processing 에서 가져가는 index값
volatile unsigned char rx_buff[COMMAND_MAX][COMMAND_LENGTH];	// 문자열을 저장하는 buffer
volatile uint8_t rx_data;   // 1. PC로 부터 1byte의 INT가 들어오면 저장 하는 변수
volatile int rx_index = 0;					// 문장의 끝을 판단하는 변수
// rx_index ==> ledalloff
int is_empty();
int is_full();

extern UART_HandleTypeDef huart2;
extern int dht11time;
extern void led2_toggle();
extern void led_pwm1();
extern void led_pwm2();
extern void led_pwm3();
// copy from HAL_UART_RxCpltCallback of stm32f4xx_hal_uart to here
// UART로 부터 1byte가 수신되면 H/W가 call을 해 준다.
// UART1 / UART2번으로 부터 1 byte가 수신(stop bit) 하면 rx interrupt가 발생
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
	if (huart == &huart2)   // PC
	{
		if(is_full())
		{
			//printf("Queue is Full");
		}
		else
		{
			unsigned char data;

			data = rx_data;

			if(data == '\n' || data == '\r')						// 문장의 끝을 만났을 때
			{
				rx_buff[input_pointer][rx_index] = '\0';					// 문장의 끝일 때는 NULL을 집어넣는다.
				rx_index = 0;												// 개선점 : circular queue(환영큐)로 개선을 해야 한다.
				//printf("input_pointer = %d\n", input_pointer);
				input_pointer++;
				input_pointer %= COMMAND_MAX;
			}
			else											    // 문장이 끝나지 않았을 때
			{
				rx_buff[input_pointer][rx_index] = data;
				rx_index++;
			}
		}
		// 주의: 반드시 HAL_UART_Receive_IT를 call 해주야 다음 INT가 발생
		HAL_UART_Receive_IT(&huart2, &rx_data, 1);
	}

//	if (huart == &huart1)   // bluetooth
//	{
//
//	}
}

int is_empty()		// 큐의 공백 상태를 알려주는 함수
{
	if (input_pointer == output_pointer)
		return 1;
	else
		return 0;
}

int is_full()		// 큐의 포화 상태를 알려주는 함수
{
	int temp;

	temp = (input_pointer + 1) % COMMAND_MAX;
	if (temp == output_pointer)
		return 1;
	else
		return 0;
}

int printon=1;
void pc_command_processing()
{
	if (is_empty())
	{
		// printf("Queue is Empty");
	}
	else
	{
		if (printon)
		{
			printf("output_pointer = %d\n", output_pointer);
			printf("%s\n", rx_buff[output_pointer]);
		}
		if(strncmp((const char *)rx_buff[output_pointer], "printon", strlen("printon")-1) == 0)
		{
			printon=1;
		}
		if(strncmp((const char *)rx_buff[output_pointer], "printoff", strlen("printoff")-1) == 0)
		{
			printon=0;
		}
		// dht11time1500
		// dht11time@@#!
		if(strncmp((const char *)rx_buff[output_pointer], "dht11time", strlen("dht11time")-1) == 0)
		{
			if (digit_check((char *) &rx_buff[output_pointer][9], 4))
			{
	printf("@@@@%s\n", &rx_buff[output_pointer][9]);
				dht11time = atoi((char *) &rx_buff[output_pointer][9]);   // 1500 의 ascii to int
	printf("dht11time: %d\n", dht11time);
				dht11time /= 10;   // 1000ms ==> 10ms이기 때문에 10으로 나눈것이다.
	printf("dht11time /= 10: %d\n", dht11time);
			}
			else
			{
				printf("invald number: %s\n", &rx_buff[output_pointer][9]);
			}
		}

		if(strncmp((const char *)rx_buff[output_pointer], "led2", strlen("led2")-1) == 0)
		{
			led2_toggle();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "pwm1", strlen("pwm1")-1) == 0)
		{
			led_pwm1();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "pwm2", strlen("pwm2")-1) == 0)
		{
			led_pwm2();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "pwm3", strlen("pwm3")-1) == 0)
		{
			led_pwm3();
		}

		output_pointer = (output_pointer + 1) % COMMAND_MAX;
		// (1) output_pointer = output_pointer + 1
		// (2) if ( output_pointer + 1 == COMMAND_MAX)
		//         output_pointer = 0;
	}
}

// true : 1
// false : 0
int digit_check(char *d, int len)  // 1500 1a1a
{
	for (int i=0; i < len; i++)
	{
		if (d[i] >= '0' && d[i] <= '9')  // '0' ~ '9'
			;
		else
			return 0;  // false
	}
	return 1;  // true
}
