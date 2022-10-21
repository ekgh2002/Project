/*
 * buzzer.c
 *
 * Created: 2022-08-15 오후 6:38:15
 *  Author: kccistc
 */ 


#define F_CPU	16000000UL
#include <avr/io.h>
#include <util/delay.h>

extern void Music_Player(int *tone, int *Beats);
extern void init_speaker(void);
extern void Beep(int  repeat);
extern void Siren(int repeat);
extern void RRR(void);

extern const int Elise_Tune[];
extern const int Elise_Beats[];

extern int buzzer_state;


// PE3 (OC3A) PWM 출력 사용.
// 16bit Timer/Counter
// OCR3A값이 같아지면 Low 출력.
/*
void buzzer(void)
{
	

	if(buzzer_state == 1)
	{		 
		OCR3A=1702;   // 삐~뽀 레
		_delay_ms(1000);
		OCR3A=1431;   // 파
		_delay_ms(1000);


		
		Beep(5);
		RRR();
		_delay_ms(3000);
		Siren(5);
		_delay_ms(3000);
		Music_Player(Elise_Tune, Elise_Beats);
	}
}*/