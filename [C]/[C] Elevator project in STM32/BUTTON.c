#include "BUTTON.h"

uint8_t prev_button1_state = BUTTON_RELEASE;
uint8_t prev_button2_state = BUTTON_RELEASE;
uint8_t prev_button3_state = BUTTON_RELEASE;
uint8_t prev_button4_state = BUTTON_RELEASE;
uint8_t prev_button10_state = BUTTON_RELEASE;

// 1. GPIO
// 2. GPIO_PIN
// 3. previous Button State
int get_button1(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *prev_button_state)
{
	unsigned char current_state;

	current_state = HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin);

	if(current_state == BUTTON_PRESS && *prev_button_state == BUTTON_RELEASE)		// 버튼을 처음 누른 상태
	{
		*prev_button_state = BUTTON_PRESS;
		HAL_Delay(200);					// 노이즈가 지나가기를 기다린다.
		return BUTTON_RELEASE;			// 아직 버튼이 눌러지지 않았음
	}
	else if(current_state == BUTTON_RELEASE && *prev_button_state == BUTTON_PRESS)	// 버튼을 뗀 상태
	{
		*prev_button_state = BUTTON_RELEASE;
		HAL_Delay(100);
		return BUTTON_PRESS;			// 버튼이 완전히 눌렸음
	}
	return BUTTON_RELEASE;				// 버튼을 눌렀다 뗀 상태가 아니다.
}

int get_button2(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *prev_button_state)
{
	unsigned char current_state;

	current_state = HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin);

	if(current_state == BUTTON_PRESS && *prev_button_state == BUTTON_RELEASE)		// 버튼을 처음 누른 상태
	{
		*prev_button_state = BUTTON_PRESS;
		HAL_Delay(200);					// 노이즈가 지나가기를 기다린다.
		return BUTTON_RELEASE;			// 아직 버튼이 눌러지지 않았음
	}
	else if(current_state == BUTTON_RELEASE && *prev_button_state == BUTTON_PRESS)	// 버튼을 뗀 상태
	{
		*prev_button_state = BUTTON_RELEASE;
		return BUTTON_PRESS;			// 버튼이 완전히 눌렸음
	}
	return BUTTON_RELEASE;				// 버튼을 눌렀다 뗀 상태가 아니다.
}

int get_button3(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *prev_button_state)
{
	unsigned char current_state;

	current_state = HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin);

	if(current_state == BUTTON_PRESS && *prev_button_state == BUTTON_RELEASE)		// 버튼을 처음 누른 상태
	{
		*prev_button_state = BUTTON_PRESS;
		HAL_Delay(200);					// 노이즈가 지나가기를 기다린다.
		return BUTTON_RELEASE;			// 아직 버튼이 눌러지지 않았음
	}
	else if(current_state == BUTTON_RELEASE && *prev_button_state == BUTTON_PRESS)	// 버튼을 뗀 상태
	{
		*prev_button_state = BUTTON_RELEASE;
		return BUTTON_PRESS;			// 버튼이 완전히 눌렸음
	}
	return BUTTON_RELEASE;				// 버튼을 눌렀다 뗀 상태가 아니다.
}

int get_button4(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *prev_button_state)
{
	unsigned char current_state;

	current_state = HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin);

	if(current_state == BUTTON_PRESS && *prev_button_state == BUTTON_RELEASE)		// 버튼을 처음 누른 상태
	{
		*prev_button_state = BUTTON_PRESS;
		HAL_Delay(200);					// 노이즈가 지나가기를 기다린다.
		return BUTTON_RELEASE;			// 아직 버튼이 눌러지지 않았음
	}
	else if(current_state == BUTTON_RELEASE && *prev_button_state == BUTTON_PRESS)	// 버튼을 뗀 상태
	{
		*prev_button_state = BUTTON_RELEASE;
		return BUTTON_PRESS;			// 버튼이 완전히 눌렸음
	}
	return BUTTON_RELEASE;				// 버튼을 눌렀다 뗀 상태가 아니다.
}

int get_button10(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *prev_button_state)
{
	unsigned char current_state;

	current_state = HAL_GPIO_ReadPin(BTN10_GPIO_Port, BTN10_Pin);

	if(current_state == BUTTON_PRESS && *prev_button_state == BUTTON_RELEASE)		// 버튼을 처음 누른 상태
	{
		*prev_button_state = BUTTON_PRESS;
		HAL_Delay(200);					// 노이즈가 지나가기를 기다린다.
		return BUTTON_RELEASE;			// 아직 버튼이 눌러지지 않았음
	}
	else if(current_state == BUTTON_RELEASE && *prev_button_state == BUTTON_PRESS)	// 버튼을 뗀 상태
	{
		*prev_button_state = BUTTON_RELEASE;
		return BUTTON_PRESS;			// 버튼이 완전히 눌렸음
	}
	return BUTTON_RELEASE;				// 버튼을 눌렀다 뗀 상태가 아니다.
}
