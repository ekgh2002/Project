/*
 * MOTOR.c
 *
 * Created: 2022-08-12 오후 2:13:20
 *  Author: kccistc
 */


// orginal
// ENA : PORTE.5 (OCR3B(PE5) <=== PORTE.4
// IN1 : PORTE.0
// IN2 : PORTE.2
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int a;
int lcd_state = 0;
extern int get_button1();
extern int motor_state;
extern int main_state;
extern int bt3;
extern int bt4;
extern char sbuf[40];
extern void init_speaker();
extern void Music_Player();
extern const int School_Bell_Tune[];
extern const int School_Bell_Beats[];
extern int time_state;
extern int f_time;
extern void init_fnd();

extern volatile uint32_t m_timer;
volatile uint32_t motor_timer;
volatile int state1 = 1;
volatile int modo = 0;
int buzzer_state = 0;



void pwm_init()
{
	DDRE |= ( 1 << 0) | ( 1 << 2) | ( 1 << 5);  // PE.0 PE.2 PE5 출력 모드로 설정
		// 모드5 : 8비트고속PWM모드 timer 3
		TCCR3A |= (1 << WGM30);
		TCCR3B |= (1 << WGM32);
		
		// 비반전 모드 TOP: 0xff 비교일치값 : OCR3C : PE5
		TCCR3A |= (1 << COM3C1);
		// 분주비 64  16000000/64  ==> 250000HZ(250KHZ)
		// 256 / 250000 ==> 1.02ms
		// 127 / 250000 ==> 0.5ms
		TCCR3B |= ( 1 << CS31)  | (1 << CS30);   // 분주비 64
		// 1600000HZ / 64분주 ==> 250000HZ (250KHZ)
		//
		OCR3C=0;  // PORTE.5
		DDRA = 0xff;
		DDRF = 0xff;

}

// ENA : PORTE.5 (OCR3B(PE5) <=== PORTE.4
// IN1 : PORTE.0
// IN2 : PORTE.2



/*
void motor ()
{
	motor_timer = (m_timer / 1000);
	if(motor_state == 1)
	{
		if(motor_timer/10 == 0)
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
	}
} */

/*
		   if ((m_timer >= 0) && (m_timer <2))
		   {
		   OCR3C=0;
		   PORTE |= 0b00000101;  // 모터정지 : PE0:1 PE2:1
		   }
		   
		   if ((m_timer >= 2) && (m_timer <4))
		   {
		   // 정회전 : PE2:0 PE0 1
		   PORTE &= 0b11111011;  // cw : PE0:1 PE2:0
		   PORTE |= 0b00000001;
		   OCR3C=127;    // 0.5ms 중간속도
		   m_timer = 0;
		   }

		   OCR3C=255;      // max speed
		   _delay_ms(2000);  // 2초간 유지
		   
		   OCR3C=0;
		   PORTE |= 0b00000101;  // 모터정지 : PE0:1 PE2:1
		   _delay_ms(500);  // 2초간 유지
		   
		   // 역회전 : PE2:1 PE0 0
		   PORTE &= 0b11111110;
		   PORTE |= 0b00000100;
		   OCR3C=127;    // 0.5ms 중간속도
		   _delay_ms(2000);  // 2초간 유지

		   OCR3C=255;      // max speed
		   _delay_ms(2000);  // 2초간 유지  
*/
void motor_a()
{
	PORTE &= 0b11111011;  // cw : PE0:1 PE2:0
	PORTE |= 0b00000001;

	OCR3C = 125 + bt3 * 40; 
}

void motor_b()
{
	PORTE &= 0b11111110;  // 역회전 : PE2:1 PE0 0
	PORTE |= 0b00000100;
	OCR3C = 255; 
}
void motor_stop()
{
	OCR3C=0;
	
	PORTE |= 0b00000101;  // 모터정지 : PE0:1 PE2:1
	
}



void running (void)
{
	if((motor_state == 1) && (main_state == 1))
	{
		motor_timer = m_timer / 1000;
		if((motor_timer >= 0) && (motor_timer <= (bt4 * 5)+5))
		{			
			if((motor_timer >= 0) && (motor_timer < bt4 * 5))
			{
				motor_a();

				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"rinsing");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "processing...");
				I2C_LCD_write_string_XY(1, 0, sbuf);
			}
			else if(motor_timer == bt4 * 5)
			{
				motor_stop();

				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"rinsing");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "end" );
				I2C_LCD_write_string_XY(1, 0, sbuf);
				
				time_state = 2;
				
				init_speaker();
				Music_Player(School_Bell_Tune, School_Bell_Beats);
				//init_fnd();
					
			}
			else if(motor_timer == (bt4 * 5)+5)
			{
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"rinsing");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "pwr:%d  time:%d", bt3+1, bt4 * 5);
				I2C_LCD_write_string_XY(1, 0, sbuf);
			}
		}
	}
	else if((motor_state == 1) && (main_state == 2))
	{
		motor_timer = m_timer / 1000;
		if((motor_timer >= 0) && (motor_timer <= (bt4 * 5)+5))
		{
			if((motor_timer >= 0) && (motor_timer < bt4 * 5))
			{
				motor_b();
				
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"washing");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "processing...");
				I2C_LCD_write_string_XY(1, 0, sbuf);
			}
			else if(motor_timer == bt4 * 5)
			{
				motor_stop();
			
				
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"washing");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "end" );
				I2C_LCD_write_string_XY(1, 0, sbuf);
				
				init_speaker();
				Music_Player(School_Bell_Tune, School_Bell_Beats);
			}
			else if(motor_timer == (bt4 * 5)+5)
			{
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"washing");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "pwr:%d  time:%d", bt3+1, bt4 * 5);
				I2C_LCD_write_string_XY(1, 0, sbuf);	
			}
			
		}
	}
	else if((motor_state == 1) && (main_state == 3))
	{
		motor_timer = m_timer / 1000;
		if((motor_timer >= 0) && (motor_timer <= (bt4 * 10)+5))
		{
			if((motor_timer >= 0) && (motor_timer < bt4 * 5))
			{
				motor_a();
				
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"dry");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "processing..." );
				I2C_LCD_write_string_XY(1, 0, sbuf);
			}
			else if(motor_timer == bt4 * 5)
			{
				motor_stop();
				
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"dry");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "processing..." );
				I2C_LCD_write_string_XY(1, 0, sbuf);			
			}
			else if((motor_timer > bt4 * 5) && (motor_timer < bt4 * 10))
			{
				motor_b();
				
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"dry");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "processing..." );
				I2C_LCD_write_string_XY(1, 0, sbuf);
			}
			else if(motor_timer == bt4 * 10)
			{
				motor_stop();
			
				
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"dry");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "end" );
				I2C_LCD_write_string_XY(1, 0, sbuf);
				
				init_speaker();
				Music_Player(School_Bell_Tune, School_Bell_Beats);
			}
			else if(motor_timer == (bt4 * 10)+5)
			{
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"dry");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "pwr:%d  time:%d", bt3+1, bt4 * 5);
				I2C_LCD_write_string_XY(1, 0, sbuf);				
			}
			
		}
	}
	else if((motor_state == 1) && (main_state == 0))
	{
		motor_timer = m_timer / 1000;
		if((motor_timer >= 0) && (motor_timer <= (bt4 * 15)+5))
		{
			if((motor_timer >= 0) && (motor_timer < bt4 * 5))
			{
				motor_a();
				
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"full washing");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "processing..." );
				I2C_LCD_write_string_XY(1, 0, sbuf);
			}
			else if(motor_timer == (bt4 * 5))
			{
				motor_stop();
				
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"full washing");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "processing..." );
				I2C_LCD_write_string_XY(1, 0, sbuf);
			}
			else if((motor_timer >= 0) && (motor_timer < bt4 * 10))
			{
				motor_b();
				
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"full washing");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "processing..." );
				I2C_LCD_write_string_XY(1, 0, sbuf);
			}
			else if(motor_timer == (bt4 * 10))
			{
				motor_stop();
				
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"full washing");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "processing..." );
				I2C_LCD_write_string_XY(1, 0, sbuf);
			}
			else if((motor_timer >= 0) && (motor_timer < bt4 * 15))
			{
				motor_a();
				
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"full washing");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "processing..." );
				I2C_LCD_write_string_XY(1, 0, sbuf);
			}
			else if(motor_timer == (bt4 * 15))
			{
				motor_stop();
				
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"full washing");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "end" );
				I2C_LCD_write_string_XY(1, 0, sbuf);
				
				init_speaker();
				Music_Player(School_Bell_Tune, School_Bell_Beats);
			}
			else if(motor_timer == (bt4 * 15)+5)
			{
				I2C_LCD_write_string_XY(0,0, "                ");
				sprintf(sbuf,"full washing");
				I2C_LCD_write_string_XY(0,0,sbuf);
				I2C_LCD_write_string_XY(1, 0, "                ");
				sprintf(sbuf, "pwr:%d  time:%d", bt3+1, bt4 * 5);
				I2C_LCD_write_string_XY(1, 0, sbuf);	
			}
		}
	}
}