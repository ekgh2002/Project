#include "main.h"
#include "button.h"
#include <stdio.h>

extern int get_button1(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *prev_button_state);
extern int get_button2(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *prev_button_state);
extern int get_button3(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *prev_button_state);
extern int get_button4(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *prev_button_state);
extern int get_button10(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *prev_button_state);
extern int get_button15(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *prev_button_state);

// 1 sec = 1000000us, 1 min = 60000000us
// rpm : revolutions per minutes
// 60000000 / 4096 / speed (1~13) in spec
#define STEPS_PER_REV 4096
#define IDLE			0
#define FORWARD	 		1
#define BACKWARD 		2
#define first_floor		1
#define second_floor	2
#define third_floor 	3
#define fourth_floor	4
#define fifth_floor     5
#define RUN				1
#define STOP			2


extern uint8_t prev_button1_state;
extern uint8_t prev_button2_state;
extern uint8_t prev_button3_state;
extern uint8_t prev_button4_state;
extern uint8_t prev_button10_state;
extern uint8_t prev_button15_state;
extern int PHOTO_COUPLER;
extern int led_state1;
extern void led_left_shift();
extern void led_right_shift();
extern void led_all_off();
extern void led_first_floor();
extern void led_second_floor();
extern void led_third_floor();
extern void led_fourth_floor();
extern void led_fifth_floor();
extern void floor_lcd1();
extern void floor_lcd2();
extern void floor();
extern void move_cursor();
extern void lcd_string();
extern void buzzer_main();
extern char sbuff[];
extern volatile int door_timer;
extern volatile int led_timer;


int motor_state = 0;
extern door_state;
int pre_state = 1;
volatile int floor_state = 0;
int run_state = 0;
int stop_state = 0;
volatile int out_state = 0;
volatile int c = 0;
volatile int current_state = 1;

volatile int led_off_timer;
int j = 0;
int lcd_state = 0;
int d = 0;

void set_rpm(int rpm)
{
	delay_us(60000000/STEPS_PER_REV/rpm);  //  maximum speed delay_us(1126)
}


void stepmotor_driver(int direction)  //  direction : forward or backward
{
	static int step = 0;  //  전역변수 처럼 동작

	switch(step)
	{
		case 0:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

			break;
		case 1:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

			break;
		case 2:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

			break;
		case 3:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

			break;
		case 4:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

			break;
		case 5:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);

			break;
		case 6:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);

			break;
		case 7:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);

			break;
	}
	if(direction == FORWARD) // for (i=0; i<8; i++;)
	{
		step++;
		step %= 8; // next step number
		c = c + 1;
	}
	else if(direction == BACKWARD)
	{
		step--;
		if(step < 0)
		step = 7;
		c = c + 1;
	}
}

void button_choice()
{
	if(run_state == IDLE)
	{
		if(get_button1(BTN1_GPIO_Port, BTN1_Pin, &prev_button1_state) == BUTTON_PRESS)
		{
			led_timer = 0;
			led_all_off();
			run_state = RUN;
			floor_state = first_floor;
			lcd_state = 2;
			led_state1 = 1;
		}
		else if(get_button2(BTN2_GPIO_Port, BTN2_Pin, &prev_button2_state) == BUTTON_PRESS)
		{
			led_timer = 0;
			led_all_off();
			run_state = RUN;
			floor_state = second_floor;
			lcd_state = 2;
			led_state1 = 1;
		}
		else if(get_button3(BTN3_GPIO_Port, BTN3_Pin, &prev_button3_state) == BUTTON_PRESS)
		{
			led_timer = 0;
			led_all_off();
			run_state = RUN;
			floor_state = third_floor;
			lcd_state = 2;
			led_state1 = 1;
		}
		else if(get_button4(BTN4_GPIO_Port, BTN4_Pin, &prev_button4_state) == BUTTON_PRESS)
		{
			led_timer = 0;
			led_all_off();
			run_state = RUN;
			floor_state = fourth_floor;
			lcd_state = 2;
			led_state1 = 1;
		}
		else if(get_button10(BTN10_GPIO_Port, BTN10_Pin, &prev_button10_state) == BUTTON_PRESS)
		{
			led_timer = 0;
			led_all_off();
			run_state = RUN;
			floor_state = fifth_floor;
			lcd_state = 2;
			led_state1 = 1;
		}
	}
}

void floor_up()
{
	run_state = RUN;
	current_state = 1;
	floor_state = 2;
}

void floor_down()
{
	run_state = RUN;
	current_state = 2;
	floor_state = 1;
}



void motor_run()
{
	if(run_state == RUN)
	{
		if(current_state < floor_state)
		{
			if(c < 4096 * (floor_state - current_state))
			{
				HAL_GPIO_WritePin(GPIOB, 0x20, GPIO_PIN_SET);
				stepmotor_driver(FORWARD);
				set_rpm(13);
				floor();
				lcd_state = 0;
				out_state = 0;
			}
			else if(c == 4096 * (floor_state - current_state))
			{
				HAL_GPIO_WritePin(GPIOB, 0xff, GPIO_PIN_RESET);
				out_state = 1;
				run_state = IDLE;
				c = 0;
				led();
				current_state = floor_state;
				door_timer = 0;
				lcd_state = 1;
				door_state = 1;

				buzzer_main();
			}
		}
		else if(current_state > floor_state)
		{
			if(c < 4096 * (current_state - floor_state))
			{
				HAL_GPIO_WritePin(GPIOB, 0x40, GPIO_PIN_SET);
				stepmotor_driver(BACKWARD);
				set_rpm(13);
				floor();
				lcd_state = 0;
				out_state = 0;
			}
			else if(c == 4096 * (current_state - floor_state))
			{
				HAL_GPIO_WritePin(GPIOB, 0xff, GPIO_PIN_RESET);
				out_state = 1;
				run_state = IDLE;
				c = 0;
				door_timer = 0;
				led();
				current_state = floor_state;
				lcd_state = 1;
				door_state = 1;
				buzzer_main();
			}
		}
	}
}



void flow_button_move()
{
	switch(run_state)
	{
	case 0:
		button_choice();
		break;
	case 1:
		motor_run();
		break;
	}
}

void led()
{
	if(floor_state == first_floor)
	{
		led_first_floor();
	}
	else if(floor_state == second_floor)
	{
		led_second_floor();
	}
	else if(floor_state == third_floor)
	{
		led_third_floor();
	}
	else if(floor_state == fourth_floor)
	{
		led_fourth_floor();
	}
	else if(floor_state == fifth_floor)
	{
		led_fifth_floor();
	}
}

void stop_led()
{
	led_off_timer = 0;
	led_all_off();
	if(led_off_timer <= 1500)
	{
		led_right_shift();
	}
}

void floor()
{
	if(lcd_state == 1)
	{
		if(current_state == first_floor)
		{
			sprintf(sbuff, "  first_floor     ");
			move_cursor(0,0);
			lcd_string(sbuff);
		}
		else if(current_state == second_floor)
		{
			sprintf(sbuff, "  second_floor     ");
			move_cursor(0,0);
			lcd_string(sbuff);

		}
		else if(current_state == third_floor)
		{
			sprintf(sbuff, "  third_floor     ");
			move_cursor(0,0);
			lcd_string(sbuff);
			sprintf(sbuff, "     Done           ");
			move_cursor(1,0);
			lcd_string(sbuff);
		}
		else if(current_state == fourth_floor)
		{
			sprintf(sbuff, "  fourth_floor     ");
			move_cursor(0,0);
			lcd_string(sbuff);

		}
		else if(current_state == fifth_floor)
		{
			sprintf(sbuff, "  fifth_floor     ");
			move_cursor(0,0);
			lcd_string(sbuff);

		}
		lcd_state = 0;
	}
}

void processing_led()
{
	if(lcd_state == 2)
	{
		sprintf(sbuff, "   %d  -->  %d    ", current_state, floor_state);
		move_cursor(0,0);
		lcd_string(sbuff);
		lcd_state = 0;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin)
	{
		case GPIO_PIN_0:
		if(current_state == 1)
		{
			door_timer = 0;
			door_state = 1;
		}
		else if(current_state == 2)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 1;
			HAL_GPIO_WritePin(GPIOB, 0x40, GPIO_PIN_SET);
		}
		else if(current_state == 3)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 1;
			HAL_GPIO_WritePin(GPIOB, 0x40, GPIO_PIN_SET);
		}
		else if(current_state == 4)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 1;
			HAL_GPIO_WritePin(GPIOB, 0x40, GPIO_PIN_SET);
		}
		else if(current_state == 5)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 1;
			HAL_GPIO_WritePin(GPIOB, 0x40, GPIO_PIN_SET);
		}
		break;

		case GPIO_PIN_1:	//  PA0

		if(current_state == 1)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 2;
			HAL_GPIO_WritePin(GPIOB, 0x20, GPIO_PIN_SET);
		}
		else if(current_state == 2)
		{
			door_timer = 0;
			door_state = 1;
		}
		else if(current_state == 3)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 2;
			HAL_GPIO_WritePin(GPIOB, 0x40, GPIO_PIN_SET);
		}
		else if(current_state == 4)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 2;
			HAL_GPIO_WritePin(GPIOB, 0x40, GPIO_PIN_SET);
		}
		else if(current_state == 5)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 2;
			HAL_GPIO_WritePin(GPIOB, 0x40, GPIO_PIN_SET);
		}
		break;

		case GPIO_PIN_4:	//  PA0;

		if(current_state == 1)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 3;
			HAL_GPIO_WritePin(GPIOB, 0x20, GPIO_PIN_SET);
		}
		else if(current_state == 2)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 3;
			HAL_GPIO_WritePin(GPIOB, 0x20, GPIO_PIN_SET);
		}
		else if(current_state == 3)
		{
			door_timer = 0;
			door_state = 1;
		}
		else if(current_state == 4)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 3;
			HAL_GPIO_WritePin(GPIOB, 0x40, GPIO_PIN_SET);
		}
		else if(current_state == 5)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 3;
			HAL_GPIO_WritePin(GPIOB, 0x40, GPIO_PIN_SET);
		}
		break;

		case GPIO_PIN_5:	//  PA0

		if(current_state == 1)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 4;
			HAL_GPIO_WritePin(GPIOB, 0x20, GPIO_PIN_SET);
		}
		else if(current_state == 2)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 4;
			HAL_GPIO_WritePin(GPIOB, 0x20, GPIO_PIN_SET);
		}
		else if(current_state == 3)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 4;
			HAL_GPIO_WritePin(GPIOB, 0x20, GPIO_PIN_SET);
		}
		else if(current_state == 4)
		{
			door_timer = 0;
			door_state = 1;
		}
		else if(current_state == 5)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 4;
			HAL_GPIO_WritePin(GPIOB, 0x40, GPIO_PIN_SET);
		}
		break;

		case GPIO_PIN_7:	//  PA0

		if(current_state == 1)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 5;
			HAL_GPIO_WritePin(GPIOB, 0x20, GPIO_PIN_SET);
		}
		else if(current_state == 2)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 5;
			HAL_GPIO_WritePin(GPIOB, 0x20, GPIO_PIN_SET);
		}
		else if(current_state == 3)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 5;
			HAL_GPIO_WritePin(GPIOB, 0x20, GPIO_PIN_SET);
		}
		else if(current_state == 4)
		{
			run_state = RUN;
			lcd_state = 2;
			floor_state = 5;
			HAL_GPIO_WritePin(GPIOB, 0x20, GPIO_PIN_SET);
		}
		else if(current_state == 5)
		{
			door_timer = 0;
			door_state = 1;
		}
		break;
	}
}








//void motor_stop()
//{
//	if(run_state == RUN)
//	{
//		if(get_button1(BTN1_GPIO_Port, BTN1_Pin, &prev_button1_state) == BUTTON_PRESS)
//		{
//			led_all_off();
//			led_right_shift();
//			run_state = IDLE;
//			c = 0;
//		}
//		else if(get_button10(BTN10_GPIO_Port, BTN10_Pin, &prev_button10_state) == BUTTON_PRESS)
//		{
//			led_all_off();
//			led_right_shift();
//			run_state = IDLE;
//			c = 0;
//		}
//		else if(get_button3(BTN3_GPIO_Port, BTN3_Pin, &prev_button3_state) == BUTTON_PRESS)
//		{
//			led_all_off();
//			led_right_shift();
//			run_state = IDLE;
//			c = 0;
//		}
//		else if(get_button4(BTN4_GPIO_Port, BTN4_Pin, &prev_button4_state) == BUTTON_PRESS)
//		{
//			stop_led();
//			run_state = IDLE;
//			c = 0;
//		}
//	}
//}





