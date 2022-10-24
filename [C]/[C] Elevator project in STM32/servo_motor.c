#include "main.h"
extern TIM_HandleTypeDef htim2;
extern volatile int TIM11_10ms_servomotor_counter;
volatile int arm_rotates_indicater = 0;
extern int distance_cm;
extern int door_state;
extern volatile int door_timer;
int operate_check = 0;
//  84000000HZ / 1680 ==> 50000HZ
//  T = 1/f 1/50000 ==> (1펄스의 소요시간)
//  2ms ==> 0.00002sec * 100 ==> 2ms : arm 180도 회전
//  1ms ==> arm 0도 회전
//  1.5ms ==> arm 90도 회전
void servo_motor_ultrasonic_sensor()
{
	if(operate_check == 0)
	{
		if(distance_cm <= 5)
		{
			TIM11_10ms_servomotor_counter = 0;
			operate_check = 1;
		}
	}
	else if(operate_check == 1)
	{
		if(TIM11_10ms_servomotor_counter == 0)
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 110);
		}
		else if(TIM11_10ms_servomotor_counter == 500)
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 10);
			operate_check = 0;
		}
	}
}

void door_servo_motor()
{
	if(door_state == 1)
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 150);

		if(door_timer >= 4000)
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 30);
			door_state = 0;
		}
	}
}

void door_open()
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 150);
}

void door_close()
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 30);
}


void servo_motor_control_main()
{
#if 1
	if(TIM11_10ms_servomotor_counter >= 100)
	{
		TIM11_10ms_servomotor_counter = 0;
		if(!arm_rotates_indicater)
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 150);
		}

		else
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 30);

		}
		arm_rotates_indicater = !arm_rotates_indicater; // next arm indicater
	}
}
#else
	while(1)
	{
		// 180도 회전
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 100);
		HAL_Delay(1000);  //  1초 유지
		// 0도
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 50);
		HAL_Delay(1000);  //  1초 유지

	}
#endif

