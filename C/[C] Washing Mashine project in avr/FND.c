/*
 * FND.c
 *
 * Created: 2022-08-12 오후 2:47:57
 *  Author: kccistc
 */ 


#include "FND.h"

uint16_t fnd_data;
uint16_t sec_data;
uint16_t data;
extern int f_time;
extern int bt4;
extern int motor_state;
extern int time_state;
extern volatile uint32_t fnd_timer;

void set_fnd_data(uint16_t data)
{
	fnd_data=data;
}

uint16_t get_fnd_data(void)
{
	return fnd_data;
}

void display_fnd(void)
{
	// uint8_t
	#if 1
	unsigned char fnd_font[] = {0xc0, 0xf9, 0xa4,0xb0, 0x99,0x92, 0x82, 0xd8, 0x80, 0x98};	// common 애노우드
	#else
	unsigned char fnd_font[] = {~0xc0, ~0xf9, ~0xa4,~0xb0, ~0x99,~0x92, ~0x82, ~0xd8, ~0x80, ~0x98};	// common 캐소우드
	#endif
	
	static uint16_t digit_position = 0;  //  static
	uint16_t data = f_time;
	
	if(time_state == 1)
	{
		switch(digit_position)
		{
			case 0:   // 1단위
			#if 1
			FND_DIGHT_PORT = 0b00000010;  // 에노우드
			#else
			FND_DIGHT_PORT = ~0b10000000;  // 캐소우드
			#endif
			FND_DATA_PORT = (fnd_font[f_time%10]);  // 0~9 123456789 101112
			break;
		
			case 1:   // 10단위
			#if 1
			FND_DIGHT_PORT = 0b00000100;  // 에노우드
			#else
			FND_DIGHT_PORT = ~0b01000000;   // 캐소우드
			#endif
			FND_DATA_PORT = (fnd_font[f_time/10%10]);
			break;
			/*case 2:   // 100단위   분
			#if 1
			FND_DIGHT_PORT = 0b00001000;  // 에노우드
			#else
			FND_DIGHT_PORT = ~0b00100000;  // 캐소우드
			#endif
			FND_DATA_PORT = fnd_font[data/100%10];
			break;
			case 3:   // 1000
			#if 1
			FND_DIGHT_PORT = 0b00010000;  // 에노우드
			#else
			FND_DIGHT_PORT = ~0b00010000;  // 캐소우드
			#endif
			FND_DATA_PORT = fnd_font[data/1000%6];
			break; */
		}
		digit_position++;
		digit_position %= 2;
	}
}


void init_fnd()
{
	FND_DATA_DDR = 0xff;
	FND_DIGHT_DDR |= 0b000111;   // 4567만 1로 3210은 그대로 유지
	#if 1
	FND_DATA_PORT = ~0x00;   // common 애노우드 FND를 all off
	#else
	FND_DATA_PORT = 0x00;   // 0xff common 케소우드 FND를 all off
	#endif
}