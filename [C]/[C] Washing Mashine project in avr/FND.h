﻿/*
 * FND.h
 *
 * Created: 2022-08-12 오후 2:47:46
 *  Author: kccistc
 */ 


#include <avr/io.h>

#define FND_DATA_DDR   DDRC
#define FND_DATA_PORT  PORTC

#define FND_DIGHT_DDR  DDRB
#define FND_DIGHT_PORT PORTB
#define FND_DIGHT_D1   4
#define FND_DIGHT_D2   5
#define FND_DIGHT_D3   6
#define FND_DIGHT_D4   7

#ifndef FND_H_
#define FND_H_

void set_fnd_data(uint16_t data);
uint16_t get_fnd_data(void);
void display_fnd(void);
void init_fnd();




#endif /* FND_H_ */
