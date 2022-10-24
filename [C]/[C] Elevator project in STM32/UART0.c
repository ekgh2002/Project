/*
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

int is_empty();
int is_full();
int digit_check(char *d, int len);

extern void set_rtc_data_time(char *data_time);
extern UART_HandleTypeDef huart2;  //pc
extern UART_HandleTypeDef huart1;  //bt
extern int dht11time;

extern void led_all_on();
extern void led_all_off();
extern void door_open();
extern void door_close();
extern void buzzer_main();
extern void floor_up();
extern void floor_down();
extern void motor_run();
extern int run_state();

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
	if (huart == &huart1)   // BT
	{
		if(is_full())
		{
			//printf("Queue is Full");
		}
		else
		{
			unsigned char data;

			data = rx_data;

			if(data == '*' || data == '\r')
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
		HAL_UART_Receive_IT(&huart1, &rx_data, 1);
	}
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

int print_on = 1;

void pc_command_processing()
{
	if (is_empty())
	{
		// printf("Queue is Empty");
	}
	else
	{
		if (print_on)
		{
			printf("output_pointer = %d\n", output_pointer);
			printf("%s\n", rx_buff[output_pointer]);
		}

		if(strncmp((const char *)rx_buff[output_pointer], "dht11time", strlen("dht11time")-1) == 0)
		{
			if(digit_check(&rx_buff[output_pointer][9], 4))
			{
				dht11time = atoi((char *)&rx_buff[output_pointer][9]);
				printf("dht11time : %d\n", dht11time);
				dht11time /= 10;
				printf("dht11time : %d\n", dht11time);
			}
			else
			{
				printf("invalid number : %s\n", &rx_buff[output_pointer][9]);
			}
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "led_all_on", strlen("led_all_on")) == 0)
		{
			led_all_on();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "led_all_off", strlen("led_all_off")) == 0)
		{
			led_all_off();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "door_open", strlen("door_open")) == 0)
		{
			door_open();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "door_close", strlen("door_close")) == 0)
		{
			door_close();
		}
		// setrtc220830111500
		else if(strncmp((const char *)rx_buff[output_pointer], "buzzer_main", strlen("buzzer_main")) == 0)
		{
			buzzer_main();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "floor_up", strlen("floor_up")) == 0)
		{
			floor_up();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "down", strlen("down")) == 0)
		{
			floor_down();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "led_8", strlen("led_8")) == 0)
		{
			led8();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "setrtc", strlen("setrtc")) == 0)
		{
			 // Filter가 필요함
//			printf("Update Clock!!!\n");
			set_rtc_data_time(&rx_buff[output_pointer][6]);		//220830111500이 들어있는 주소값을 넘겨 set_rtc_data_time 함수를 call
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "led1", strlen("led1")) == 0)
		{
			led1();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "led2", strlen("led2")) == 0)
		{
			led2();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "led3", strlen("led3")) == 0)
		{
			led3();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "led4", strlen("led4")) == 0)
		{
			led4();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "led5", strlen("led5")) == 0)
		{
			led5();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "led6", strlen("led6")) == 0)
		{
			led6();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "led7", strlen("led7")) == 0)
		{
			led7();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "led8", strlen("led8")) == 0)
		{
			led8();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "offa", strlen("offa")) == 0)
		{
			led1a();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "offb", strlen("offb")) == 0)
		{
			led2a();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "offc", strlen("offc")) == 0)
		{
			led3a();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "offd", strlen("offd")) == 0)
		{
			led4a();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "offe", strlen("offe")) == 0)
		{
			led5a();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "offf", strlen("offf")) == 0)
		{
			led6a();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "offg", strlen("offg")) == 0)
		{
			led7a();
		}
		else if(strncmp((const char *)rx_buff[output_pointer], "offh", strlen("offh")) == 0)
		{
			led8a();
		}
		output_pointer = (output_pointer + 1) % COMMAND_MAX;
	}
}


// true : 1
// false : 0
int digit_check(char *d, int len)
{
	for(int i=0; i<len; i++)
	{
		if(d[i] >= '0' && d[i] <= '9')
			;
		else
			return 0;		// false
	}
	return 1;				// true
}
