#include "main.h"		// GPIO HAL

#define TRIG_PORT	GPIOC
#define TRIG_PIN	GPIO_PIN_5

extern volatile int TIM11_10ms_servomotor_counter;

// 여기는 cubeIDE와 만나지 않는 영역 -> 한글로 주석을 달아도 괜찮음
int distance;		// 거리 측정 값을 저장하는 변수
int distance_cm;
int it_cpt_flag;	// rising edge와 falling edge를 detect 했을 때 check하는 flag 변수

// rising edge/falling edge Interrupt가 발생되면 이곳으로 들어온다.
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t is_first_capture = 0;

	if(htim->Instance == TIM1)
	{
		if(is_first_capture == 0)				// rising edge detect
		{
			__HAL_TIM_SET_COUNTER(htim, 0);		// clear
			is_first_capture = 1;				// rising edge를 만났다는 indicator를 set
		}
		else if(is_first_capture == 1)			// falling edge detect
		{
			is_first_capture = 0;				// falling edge를 만났기에 indicator를 reset
			distance = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);		// 현재까지 count된 값을 읽어온다.
			it_cpt_flag = 1;					// rising edge와 falling edge를 만났다.
		}
	}
}

void make_trigger()
{
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
	delay_us(2);
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
	delay_us(10);
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}

extern volatile int TIM11_10ms_ultrasonic_counter;

void ultrasonic_processing()
{
	if(TIM11_10ms_ultrasonic_counter >= 100)	// 1sec마다
	{
		TIM11_10ms_ultrasonic_counter = 0;
		make_trigger();
		if(it_cpt_flag)
		{
			it_cpt_flag = 0;
			distance_cm = distance * 0.034 / 2;	// 1us -> 0.034cm, 왕복 값을 return -> /2
			printf("distance : %d\n", distance_cm);
		}
	}
}
