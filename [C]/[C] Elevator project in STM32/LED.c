#include "LED.h"
#include "BUTTON.H"

extern uint8_t prev_button1_state;
extern uint8_t prev_button2_state;
extern uint8_t prev_button3_state;

extern volatile int i;
volatile int led_timer;

int a = 0;
int b = 0;
extern int floor_state;
int led_state1 = 0;
extern volatile int led_timer;



void button1_ledall_on_off()
{
	static uint8_t button1_state = 0;
	if(get_button1(BTN1_GPIO_Port, BTN1_Pin, &prev_button1_state) == BUTTON_PRESS)
	{
		button1_state++;
		button1_state %= 2;
		if(button1_state)
			led_all_on();
		else
			led_all_off();
	}
}

void led2_toggle()
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	HAL_Delay(500);
}



void flower_on()
{
	for(int i=0; i<4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x10 << i | 0x08 >> i, GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
		HAL_Delay(10);
	}
}

void flower_off()
{
	for(int i=0; i<4; i++)
		{
			HAL_GPIO_WritePin(GPIOB, 0x80 >> i | 0x01 << i, GPIO_PIN_SET);
			HAL_Delay(200);
			HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET);
			HAL_Delay(10);
		}
}

void led_on_1_up()
{
	for(int i=0; i<9; i++)
		{
			HAL_GPIO_WritePin(GPIOB, 0x01 << i , GPIO_PIN_SET);
			HAL_Delay(200);
			HAL_GPIO_WritePin(GPIOB, 0x01 << i, GPIO_PIN_RESET);
			HAL_Delay(10);
		}
}

void led_left_shift()
{
   if(led_timer >= 600 * floor_state)
   {
	  led_timer = 0;
	  HAL_GPIO_WritePin(GPIOB, 0x01 << a , GPIO_PIN_SET);
	  a = (a + 1) % 8;
   }
}

void led_right_shift()
{
//   if(led_timer >= 200)
//   {
//	  led_timer = 0;
//	  HAL_GPIO_WritePin(GPIOB, 0x80 >> b , GPIO_PIN_SET);
//	  b = (b + 1) % 8;
//   }
	led_timer = 0;
	if(led_timer == 0)
	HAL_GPIO_WritePin(GPIOB, 0x01 , GPIO_PIN_SET);
	if(led_timer == 200)
	HAL_GPIO_WritePin(GPIOB, 0x02 , GPIO_PIN_SET);
	if(led_timer == 400)
	HAL_GPIO_WritePin(GPIOB, 0x04 , GPIO_PIN_SET);
	if(led_timer == 600)
	HAL_GPIO_WritePin(GPIOB, 0x08 , GPIO_PIN_SET);
	if(led_timer == 800)
	HAL_GPIO_WritePin(GPIOB, 0x10 , GPIO_PIN_SET);
	if(led_timer == 1000)
	HAL_GPIO_WritePin(GPIOB, 0x20 , GPIO_PIN_SET);
	if(led_timer == 1200)
	HAL_GPIO_WritePin(GPIOB, 0x40 , GPIO_PIN_SET);
	if(led_timer == 1400)
	HAL_GPIO_WritePin(GPIOB, 0x80 , GPIO_PIN_SET);
}
void led_all_off()
{
	HAL_GPIO_WritePin(GPIOB, 0xff , GPIO_PIN_RESET);
}
void led1()
{
	HAL_GPIO_WritePin(GPIOB, 0x01 , GPIO_PIN_SET);
}
void led2()
{
	HAL_GPIO_WritePin(GPIOB, 0x02 , GPIO_PIN_SET);
}
void led3()
{
	HAL_GPIO_WritePin(GPIOB, 0x04 , GPIO_PIN_SET);
}
void led4()
{
	HAL_GPIO_WritePin(GPIOB, 0x08 , GPIO_PIN_SET);
}
void led5()
{
	HAL_GPIO_WritePin(GPIOB, 0x10 , GPIO_PIN_SET);
}
void led6()
{
	HAL_GPIO_WritePin(GPIOB, 0x20 , GPIO_PIN_SET);
}
void led7()
{
	HAL_GPIO_WritePin(GPIOB, 0x40 , GPIO_PIN_SET);
}
void led8()
{
	HAL_GPIO_WritePin(GPIOB, 0x80 , GPIO_PIN_SET);
}
void led1a()
{
	HAL_GPIO_WritePin(GPIOB, 0x01 , GPIO_PIN_RESET);
}
void led2a()
{
	HAL_GPIO_WritePin(GPIOB, 0x02 , GPIO_PIN_RESET);
}
void led3a()
{
	HAL_GPIO_WritePin(GPIOB, 0x04 , GPIO_PIN_RESET);
}
void led4a()
{
	HAL_GPIO_WritePin(GPIOB, 0x08 , GPIO_PIN_RESET);
}
void led5a()
{
	HAL_GPIO_WritePin(GPIOB, 0x10 , GPIO_PIN_RESET);
}
void led6a()
{
	HAL_GPIO_WritePin(GPIOB, 0x20 , GPIO_PIN_RESET);
}
void led7a()
{
	HAL_GPIO_WritePin(GPIOB, 0x40 , GPIO_PIN_RESET);
}
void led8a()
{
	HAL_GPIO_WritePin(GPIOB, 0x80 , GPIO_PIN_RESET);
}

void led_first_floor()
{
	HAL_GPIO_WritePin(GPIOB, 0xff , GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, 0x01, GPIO_PIN_SET);
}
void led_second_floor()
{
	HAL_GPIO_WritePin(GPIOB, 0xff , GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, 0x02, GPIO_PIN_SET);
}
void led_third_floor()
{
	HAL_GPIO_WritePin(GPIOB, 0xff , GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, 0x04, GPIO_PIN_SET);
}
void led_fourth_floor()
{
	HAL_GPIO_WritePin(GPIOB, 0xff , GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, 0x08, GPIO_PIN_SET);
}
void led_fifth_floor()
{
	HAL_GPIO_WritePin(GPIOB, 0xff , GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, 0x10, GPIO_PIN_SET);
}


void led_on_1_down()
{
	for(int i=0; i<9; i++)
			{
				HAL_GPIO_WritePin(GPIOB, 0x80 >> i , GPIO_PIN_SET);
				HAL_Delay(200);
				HAL_GPIO_WritePin(GPIOB, 0x80 >> i, GPIO_PIN_RESET);
				HAL_Delay(10);
			}
}

void led_on_up()
{
	for(int i=0; i<8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x0001 << i , GPIO_PIN_SET);
		HAL_Delay(200);
	}
}

void led_on_down()
{
	for(int i=0; i<8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x0080 >> i, GPIO_PIN_SET);
		HAL_Delay(200);
	}
}

void led_all_on()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_SET);
}
