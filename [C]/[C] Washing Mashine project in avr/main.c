/*
 * washing_machine_pj.c
 *
 * Created: 2022-08-12 오후 2:11:28
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h> // printf, scanf 등이 정의 되어 있다.
#include <string.h> // strcpy, strcat, strcmp 등이 들어 있음

#include "fnd.h"
#include "time_clock.h"

int on = 0;
int main_state = 0;
int sub_state = 0;
int third_state = 0;
int motor_state = 0;
int time_state = 0;
int pwr = 1;
int bt2 = 0;
int bt3 = 0;
int bt4 = 1;
int f_time = 0;



extern void UART0_transmit(uint8_t data);
extern void init_UART1();


// 외부에 있는 파일에 있는 변수를 참조할 때는 extern로 선언한다. (externally)
extern TIME time_clock;
extern void pc_command_processing();
extern int get_button1();
extern int get_button2();
extern int get_button3();
extern int get_button4();

extern void init_button();
extern void init_uart0();
extern void motor();
extern void pwm_init();
extern void motor_a();
extern void motor_stop();
extern void running();
extern void running_lcd();
extern uint16_t data;
extern volatile uint32_t nodelay;



// 1. FOR printf
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);



uint32_t ms_count=0;		// ms를 측정
uint32_t sec_count=0;

volatile int ms_clock_display = 0;  //  분초시계


void init_timer0();
volatile uint32_t nodelay = 0;
volatile uint32_t m_timer = 0;
volatile uint32_t fnd_timer = 0;








// portb.0 : 500ms on/off
// portb.1 : 300ms on/off
// 1. 분주 : 64분주  ==>  16000000 / 64 = 250000Hz
// 2. T 계산 ==>  1/f = 1/250000 = 0.000004sec (4us)
// 3. 8 bit Timer OV : 4us * 256 = 1.024ms
// 256개의 pulse 를 count 하면 이곳으로 온다
//

ISR(TIMER0_OVF_vect) // 인터럽트 루틴을 길게 짤 수록 output delay가 증가하여 원하는 시간에 출력이 나오지 않음
{
	TCNT0 = 6; // TCNT를 6~256 == > 정확히 1ms 를 유지하기 위해
	// TINT 0 OVF INT
	ms_count++;
	ms_clock_display++;  //  lcd에 분초 시계 display하기 위한 interval 조절
	nodelay++;
	m_timer++;
	fnd_timer++;
	
	if(time_state == 1)
	{
		data++;
	}
	if (ms_count >= 1000)   // 1000ms ==> 1sec
	{
		ms_count=0;
		
		sec_count++;    // sec counter 증가
		//inc_time_sec(); // 1초에 한번 씩 call
		if(time_state == 1)
		{
			if (ms_count%4 == 0)  //4ms 마다 fnd를 display
			{
				display_fnd();
			}
			f_time--;
		}
		if(time_state == 2)
		{
			f_time = 0x00;
			
			FND_DIGHT_PORT = 0x00;
		}
	}
	
	if (ms_count%4 == 0)  //4ms 마다 fnd를 display
	{
		display_fnd();
	}
}
//watch의 를 선언
#define WATCH 0
#define STOPWATCH 1
int mode = WATCH;


char sbuf[40];
TIME myTIME; // 시계를 구동




int main(void)
{
	
	
			
	pwm_init();
	init_fnd();
	init_button();
	//init_uart0(); // UART0를 초기화 한다.
	///*init_UART1();*/ // UART1를 초기화 한다.
	I2C_LCD_init();
	//stdout = &OUTPUT; // for printf /fprintf(stdout, "test"); ==> printf stdin : 입력
	init_timer0();
		 
	sei();			 // global interrupt 활성화
	
	mode = WATCH; // 시계모드로 시작
	
	m_timer=0;
	
	
	OCR3C=0;  // PORTE.5

	while (1)
	{
		get_time_clock(&myTIME);
		set_fnd_data(myTIME.min*100 + myTIME.sec);
		
		BUTTON1_toggle();
		BUTTON2_toggle();
		BUTTON3_toggle();
		BUTTON4_toggle();
		running();

	}
	
	
}

void init_timer0()
{
	TCNT0 = 6;
	
	TCCR0 |= (1 << CS02) | (0 << CS01) | (0 << CS00);  //  분주비를 64로 설정
	
	TIMSK |= (1 << TOIE0);
}




void BUTTON1_toggle(void)  // power on 및 동작 실행
{
	if(get_button1())
	{
		m_timer = 0;
		motor_state = 1;
		time_state = 1;
		f_time = bt4 * 5; 
	}
}


void BUTTON2_toggle(void)
{
	if(get_button2())
	{
		if (main_state == 0)
		{
			main_state = main_state + 1;
			sub_state = 1;
			
			PORTF = 0x01;
			PORTA = 0x10;
			I2C_LCD_write_string_XY(0,0, "                ");
			sprintf(sbuf,"rinsing");
			I2C_LCD_write_string_XY(0,0,sbuf);
			I2C_LCD_write_string_XY(1, 0, "                ");
			sprintf(sbuf, "pwr:1  time:5");
			I2C_LCD_write_string_XY(1, 0, sbuf);
		}
		else if (main_state == 1)
		{
			main_state = main_state + 1;
			sub_state = 2;
			PORTF = 0x01;
			PORTA = 0x10;
			I2C_LCD_write_string_XY(0,0, "                ");
			sprintf(sbuf,"washing");
			I2C_LCD_write_string_XY(0,0,sbuf);
			I2C_LCD_write_string_XY(1, 0, "                ");
			sprintf(sbuf, "pwr:1  time:5");
			I2C_LCD_write_string_XY(1, 0, sbuf);
		}
		else if (main_state == 2)
		{
			main_state = main_state + 1;
			sub_state = 3;
			PORTF = 0x01;
			PORTA = 0x10;
			I2C_LCD_write_string_XY(0,0, "                ");
			sprintf(sbuf,"dry");
			I2C_LCD_write_string_XY(0,0,sbuf);
			I2C_LCD_write_string_XY(1, 0, "                ");
			sprintf(sbuf, "pwr:1  time:5");
			I2C_LCD_write_string_XY(1, 0, sbuf);
		}
		else if (main_state == 3)
		{
			main_state = 0;
			sub_state = 4;
			PORTF = 0x01;
			PORTA = 0x10;
			I2C_LCD_write_string_XY(0,0, "                ");
			sprintf(sbuf,"full washing");
			I2C_LCD_write_string_XY(0,0,sbuf);
			I2C_LCD_write_string_XY(1, 0, "                ");
			sprintf(sbuf, "pwr:1  time:5");
			I2C_LCD_write_string_XY(1, 0, sbuf);
		}
	}
}

void BUTTON3_toggle(void)
{
	if(get_button3())
	{
		if ((bt3 >= 0) && (bt3 < 2))
		{
			bt3 = bt3 + 0x01;
		}
		else if (bt3 == 2)
		{
			bt3 = 0;
		}

		if (sub_state == 1)
		{
		
			third_state = 1;
			PORTF = 0x01 << bt3;
			PORTA = 0x08 << bt4;
			
			I2C_LCD_write_string_XY(0,0, "                ");
			sprintf(sbuf,"rinsing");
			I2C_LCD_write_string_XY(0,0,sbuf);
			I2C_LCD_write_string_XY(1, 0, "                ");
			sprintf(sbuf, "pwr:%d  time:%d", bt3+1, bt4 * 5);
			I2C_LCD_write_string_XY(1, 0, sbuf);
		}
		
		else if (sub_state == 2)
		{
			third_state = 2;
			PORTF = 0x01 << bt3;
			PORTA = 0x08 << bt4;
			
			I2C_LCD_write_string_XY(0,0, "                ");
			sprintf(sbuf,"washing");
			I2C_LCD_write_string_XY(0,0,sbuf);
			I2C_LCD_write_string_XY(1, 0, "                ");
			sprintf(sbuf, "pwr:%d  time:%d", bt3+1, bt4 * 5);
			I2C_LCD_write_string_XY(1, 0, sbuf);
		}
		
		else if (sub_state == 3)
		{
			third_state = 3;
			PORTF = 0x01 << bt3;
			PORTA = 0x08 << bt4;
			
			I2C_LCD_write_string_XY(0,0, "                ");
			sprintf(sbuf,"dry");
			I2C_LCD_write_string_XY(0,0,sbuf);
			I2C_LCD_write_string_XY(1, 0, "                ");
			sprintf(sbuf, "pwr:%d  time:%d", bt3+1, bt4 * 5);
			I2C_LCD_write_string_XY(1, 0, sbuf);
		}
		
		if (sub_state == 4)
		{
			third_state = 4;
			PORTF = 0x01 << bt3;
			PORTA = 0x08 << bt4;
			
			I2C_LCD_write_string_XY(0,0, "                ");
			sprintf(sbuf,"full washing");
			I2C_LCD_write_string_XY(0,0,sbuf);
			I2C_LCD_write_string_XY(1, 0, "                ");
			sprintf(sbuf, "pwr:%d  time:%d", bt3+1, bt4 * 5);
			I2C_LCD_write_string_XY(1, 0, sbuf);
		}
	}
}

void BUTTON4_toggle(void)
{
	
	if(get_button4())
	{
		if ((bt4 >= 0) && (bt4 < 4))
		{
			bt4 = bt4 + 0x01;
		}
		else if (bt4 == 4)
		{
			bt4 = 1;
		}

		if (third_state == 1)
		{
			PORTF = 0x01 << bt3;
			PORTA = 0x08 << bt4;
			
			I2C_LCD_write_string_XY(0,0, "                ");
			sprintf(sbuf,"rinsing");
			I2C_LCD_write_string_XY(0,0,sbuf);
			I2C_LCD_write_string_XY(1, 0, "                ");
			sprintf(sbuf, "pwr:%d  time:%d", bt3+1, bt4 * 5);
			I2C_LCD_write_string_XY(1, 0, sbuf);
		}
		
		if (third_state == 2)
		{
			PORTF = 0x01 << bt3;
			PORTA = 0x08 << bt4;
			
			I2C_LCD_write_string_XY(0,0, "                ");
			sprintf(sbuf,"washing");
			I2C_LCD_write_string_XY(0,0,sbuf);
			I2C_LCD_write_string_XY(1, 0, "                ");
			sprintf(sbuf, "pwr:%d  time:%d", bt3+1, bt4 * 5);
			I2C_LCD_write_string_XY(1, 0, sbuf);
		}
		if (third_state == 3)
		{
			PORTF = 0x01 << bt3;		
			PORTA = 0x08 << bt4;
			
			I2C_LCD_write_string_XY(0,0, "                ");
			sprintf(sbuf,"dry");
			I2C_LCD_write_string_XY(0,0,sbuf);
			I2C_LCD_write_string_XY(1, 0, "                ");
			sprintf(sbuf, "pwr:%d  time:%d", bt3+1, bt4 * 5);
			I2C_LCD_write_string_XY(1, 0, sbuf);
		}
		if (third_state == 4)
		{
			PORTF = 0x01 << bt3;
			PORTA = 0x08 << bt4;
			
			I2C_LCD_write_string_XY(0,0, "                ");
			sprintf(sbuf,"full washing");
			I2C_LCD_write_string_XY(0,0,sbuf);
			I2C_LCD_write_string_XY(1, 0, "                ");
			sprintf(sbuf, "pwr:%d  time:%d", bt3+1, bt4 * 5);
			I2C_LCD_write_string_XY(1, 0, sbuf);
		}
	}
}
/*
		m_timer = 0;
		
		if(motor_state == 0)
		{
			int motor_timer = (m_timer / 1000);
			if(motor_timer < 9)
			{
				
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"plz");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "plzplz");
				I2C_LCD_write_string_XY(1, 0, sbuf);

				PORTE &= 0b11111011;  // cw : PE0:1 PE2:0
				PORTE |= 0b00000001;
				OCR3C=200;    // 0.5ms 중간속도
			}
			else if(motor_timer == 9)
			{
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"plz");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "plzplz");
				I2C_LCD_write_string_XY(1, 0, sbuf);
				OCR3C=0;
				PORTE |= 0b00000101;  // 모터정지 : PE0:1 PE2:1
			}
			*/



/*
void motor ()
{
	switch(motor_state)
	{
		case 1:  // stop status
		if (m_timer >= 2000)
		{
			m_timer=0;
			motor_state++;
			// 정회전 : PE2:0 PE0 1
			PORTE &= 0b11111011;  // cw : PE0:1 PE2:0
			PORTE |= 0b00000001;
			OCR3C=200;    // 0.5ms 중간속도
		}
		break;
		case 2:
		if (m_timer >= 2000)
		{
			m_timer=0;
			OCR3C=0;
			PORTE |= 0b00000101;  // 모터정지 : PE0:1 PE2:1
		}
		break;
	}
}
*/








/*	int motor_timer = (m_timer / 1000);
	if(motor_timer < 9)
	{
		
		I2C_LCD_write_string_XY(0,0, "                ");
		sprintf(sbuf,"plz");
		I2C_LCD_write_string_XY(0,0,sbuf);
		I2C_LCD_write_string_XY(1, 0, "                ");
		sprintf(sbuf, "plzplz");
		I2C_LCD_write_string_XY(1, 0, sbuf);

		PORTE &= 0b11111011;  // cw : PE0:1 PE2:0
		PORTE |= 0b00000001;
		OCR3C=200;    // 0.5ms 중간속도
	}
	else if(motor_timer == 9)
	{
		I2C_LCD_write_string_XY(0,0, "                ");
		sprintf(sbuf,"plz");
		I2C_LCD_write_string_XY(0,0,sbuf);
		I2C_LCD_write_string_XY(1, 0, "                ");
		sprintf(sbuf, "plzplz");
		I2C_LCD_write_string_XY(1, 0, sbuf);
		OCR3C=0;
		PORTE |= 0b00000101;  // 모터정지 : PE0:1 PE2:1				
	}
		
	
}*/




