#ifndef __KPD_AL_H__
#define __KPD_AL_H__


#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>


#include <kpd-al-defines.h>
#include <kpd-al-variables.h>
#include <kpd-al-pump.h>


ISR(TIMER0_COMPA_vect);
ISR(TIMER1_OVF_vect);
ISR(TIMER1_CAPT_vect);

inline	void	service();			/*то, что крутится в состоянии "Сервис"*/
inline	void	programming();			/*то, что крутится в состоянии "Программирование"*/
inline	void	regeneration();			/*то, что крутится в состоянии "Регенерация"*/

	void	print_d();			/*отображение следующего разряда индикатора*/
	
	void	print();			/*Отображает все разряды в течении какго-то времени (блокирующая)*/


#endif
