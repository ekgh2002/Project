#include "main.h"	// for GPIO HAL

extern RTC_HandleTypeDef hrtc;

RTC_TimeTypeDef sTime = {0};	// 시간 정보
RTC_DateTypeDef sDate = {0};	// 날짜 정보

char sbuff[40];

// ex) 22의 BCD의 메모리에 저장된 Format
// High nibble(4bit) |  Low nibble(4bit)
//      0010         |       0010
//       20          +         2          = 22

unsigned char bin_2_dec(unsigned char byte)		// call by value (byte)
{
	unsigned char high, low;					// high nibble과 low nibble을 저장하는 임시 저장소

	low = byte & 0x0F;							// 하위 4bit만을 취한다.  0010 [0010] & 0000 1111 = 0000 [0010] ------> * 1 = 2
	high = ((byte >> 4) & 0x0F) * 10;			// 상위 4bit만을 취한다. [0010] 0010 >> 4 = 0000 [0010] -----> *10 ===> 20

	return high + low;
}

unsigned char dec_2_bin(unsigned char byte)		// call by value (byte)
{
	unsigned char high, low;
	low = byte % 10;							// 1의자리 ---> 22 % 10 = 2 = 0000 0010
	high = (byte / 10) << 4;					// 10의 자리 ---> 22 / 10 = 2 = 0000 0010 ----> 0000 0010 << 4 = 0010 0000

	return high + low;
}

// RTC에서 날짜와 시간 정보를 가져오는 함수
void get_rtc_time()
{
	static RTC_TimeTypeDef oTime;	// 이전 시간 정보 old Time & 유지 -> static

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

	if(oTime.Seconds != sTime.Seconds)
	{
		// YYYY-MM-DD hh:mm:ss
//		printf("%04d-%02d-%02d %02d:%02d:%02d\n",
//				(bin_2_dec(sDate.Year)+2000), bin_2_dec(sDate.Month), bin_2_dec(sDate.Date),
//				bin_2_dec(sTime.Hours), bin_2_dec(sTime.Minutes), bin_2_dec(sTime.Seconds));
		// text lcd의 1번째 라인의 0번째 col에 YYYY-MM-DD hh:mm:ss를 출력
		// text lcd의 1번째 라인 : Date : 22- 8-30
		// text lcd의 2번째 라인 : Time : 14:10:00

		sprintf(sbuff, "TIME : %02d:%02d:%02d", bin_2_dec(sTime.Hours), bin_2_dec(sTime.Minutes), bin_2_dec(sTime.Seconds));
		move_cursor(1,0);
		lcd_string(sbuff);
//		sprintf(sbuff, "DATE : %02d-%02d-%02d", bin_2_dec(sDate.Year), bin_2_dec(sDate.Month), bin_2_dec(sDate.Date));
//		move_cursor(1,0);
//		lcd_string(sbuff);
	}
	oTime.Seconds = sTime.Seconds;
}

// setrtc220830111500
// 012345678901234567
// data_time이라는 변수에는 220830111500의 주소가 넘어온다.
void set_rtc_data_time(char *data_time)
{
	char YY[4], MM[4], DD[4];	// 날짜
	char hh[4], mm[4], ss[4];	// 시간

//	strncpy(YY, data_time, 2);			// 연
//	strncpy(MM, data_time + 2, 2);		// 월
//	strncpy(DD, data_time + 4, 2);		// 일

	strncpy(hh, data_time, 2);		// 시
	strncpy(mm, data_time + 2, 2);		// 분
	strncpy(ss, data_time + 4, 2);		// 초

	// 1. ascii -> int
	// 2. int -> BCD
	// 3. BCD -> RTC
//	sDate.Year = dec_2_bin(atoi((char *)YY));
//	sDate.Month = dec_2_bin(atoi((char *)MM));
//	sDate.Date = dec_2_bin(atoi((char *)DD));

	sTime.Hours = dec_2_bin(atoi((char *)hh));
	sTime.Minutes = dec_2_bin(atoi((char *)mm));
	sTime.Seconds = dec_2_bin(atoi((char *)ss));

//	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
}
