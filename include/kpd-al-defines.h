#ifndef __KPD_AL_DEFINES_H__
#define __KPD_AL_DEFINES_H__


// ------------------------------------ индикация ------------------------------------

// выводы шины данных
#define	PA	0
#define	PB	7
#define	PC	5
#define	PD	3
#define	PE	2
#define	PF	1
#define	PG	6
#define	PDP	4

// их маски в байте
#define	S(x)	(1<<(P##x))

// выводы шина адреса (старший, средний, младший)
#define	BUS_H_PIN	2
#define BUS_M_PIN	1
#define	BUS_L_PIN	0

// маска шины адреса (вывод в порт прячет разряды)
#define	BUS_MSK		((1<<BUS_H_PIN)|(1<<BUS_M_PIN)|(1<<BUS_L_PIN))

// маски отображения разрядов (старший, средний, младший)
#define	BUS_H_MSK	((1<<BUS_M_PIN)|(1<<BUS_L_PIN))
#define	BUS_M_MSK	((1<<BUS_H_PIN)|(1<<BUS_L_PIN))
#define	BUS_L_MSK	((1<<BUS_H_PIN)|(1<<BUS_M_PIN))


// цифры
#define	D0	(S(A)|S(B)|S(C)|S(D)|S(E)|S(F))
#define	D1	(S(B)|S(C))
#define	D2	(S(A)|S(B)|S(D)|S(E)|S(G))
#define	D3	(S(A)|S(B)|S(C)|S(D)|S(G))
#define	D4	(S(B)|S(C)|S(F)|S(G))
#define	D5	(S(A)|S(C)|S(D)|S(F)|S(G))
#define	D6	(S(A)|S(C)|S(D)|S(E)|S(F)|S(G))
#define	D7	(S(A)|S(B)|S(C))
#define	D8	(S(A)|S(B)|S(C)|S(D)|S(E)|S(F)|S(G))
#define	D9	(S(A)|S(B)|S(C)|S(D)|S(F)|S(G))



// символ t
#define	DT	(S(D)|S(E)|S(F)|S(G))
// символ дюйма
#define	DINCH	(S(F))



// ------------------------------------- датчик --------------------------------------

// вывод на котором сидит датчик (изменению не подлежит)
#define	SENSOR_PIN	6
// маска вывода датчика
#define	SENSOR_MSK	(1<<(SENSOR_PIN))

// порт на котором висит выбор насоса
#define	SENSOR_CHANGE_PORT	PIND
// вывод на котором висит выбор датчика
#define	SENSOR_CHANGE_PIN	4
// маска вывода выбора датчика
#define	SENSOR_CHANGE_MSK	(1<<(SENSOR_CHANGE_PIN))




// ------------------------------------- кнопка --------------------------------------
#define	BUTTON_PORT	(PIND)
#define	BUTTON_PIN	3
#define	BUTTON_MSK	(1<<BUTTON_PIN)
#define	BUTTON_LEVEL	1
#define	BUTTON_TRUE	((BUTTON_LEVEL)<<(BUTTON_PIN))
#define	BUTTON_FALSE	((!(BUTTON_LEVEL))<<(BUTTON_PIN))



//-------------------------------- режим регенерации ---------------------------------
#define	REGEN_PORT	(PINA)
#define	REGEN_PIN	0
#define	REGEN_MSK	(1<<(REGEN_PIN))
#define	REGEN_LEVEL	0
#define	REGEN_TRUE	((REGEN_LEVEL)<<(REGEN_PIN))
#define	REGEN_FALSE	((!(REGEN_LEVEL))<<(REGEN_PIN))



// ------------------------------- управление насосом --------------------------------
// вывод на котором сидит насос
#define	PUMP_PIN	5
// маска вывода насоса
#define	PUMP_MSK	(1<<(PUMP_PIN))


#define PUMP_ON_LEVEL	0


// ------------------------------------- авария --------------------------------------
#define	CRASH_PORT	(PORTA)
#define	CRASH_PIN	1
#define	CRASH_MSK	(1<<(CRASH_PIN))
#define	CRASH_LEVEL	0
#define	CRASH_TRUE	((CRASH_LEVEL)<<(CRASH_PIN))
#define	CRASH_FALSE	((!(CRASH_LEVEL))<<(CRASH_PIN))
#define PUMP_CHANGE_MSK	(1<<2)



//------------------------------ режимы работы таймеров ------------------------------
#define F_MAIN_TIME	50
#define	TCCR0A_REG	0x02
#define	TCCR0B_REG	0x05

#define	TCCR1A_REG	0x00
#define	TCCR1B_REG	0x04
#define	TCCR1C_REG	0x00

#define	TIMSK_REG	(TOIE1|ICIE1|OCIE0A)

//определение предделителя по режиму работы

#if ((TCCR0B_REG&0x07)==0x01)
#define	T0PRESC	1
#endif
#if ((TCCR0B_REG&0x07)==0x02)
#define	T0PRESC	8
#endif
#if ((TCCR0B_REG&0x07)==0x03)
#define	T0PRESC	64
#endif
#if ((TCCR0B_REG&0x07)==0x04)
#define	T0PRESC	256
#endif
#if ((TCCR0B_REG&0x07)==0x05)
#define	T0PRESC	1024
#endif

//определение необходимого порога отсчёта

#ifndef T0PRESC
#error T0PRESC not defined
#endif

#define	OCR0A_REG	(F_CPU/F_MAIN_TIME/T0PRESC-1)
#if (OCR0A_REG>255)
#error too small T0PRESC
#endif

//---------------------------------- статусные биты ----------------------------------
#define	STATUS_REGENERARION	(1<<0)
#define STATUS_OVER		(1<<1)
#define	STATUS_BW		(1<<2)
#define MAIN_STATUS_MSK		(STATUS_REGENERATION|STATUS_OVER|STATUS_BW)

#define STATUS_REFRESH		(1<<3)
#define	STATUS_NEED_PROGRAM	(1<<3)

#define	STATUS_T1		(1<<7)
#define STATUS_T2		(1<<6)
#define PUMP_STATUS_MSK		(STATUS_T1|STATUS_T2)


//-------------------------------------- прочее --------------------------------------
#define	MAX_N			20
#define	PROG_WAIT		255
#define PRINT_DELAY		100



//------------------------------------- макросы --------------------------------------
#define	ee_read_n(x) {x = eeprom_read_byte(&ee_N);}
#define	ee_write_n(x) {eeprom_write_byte(&ee_N,x);}
#define read_n(x) {n = eeprom_read_word(N+x);}

#define	ee_read_tbw(x) {x = eeprom_read_byte(&ee_Tbw);}
#define	ee_write_tbw(x) {eeprom_write_byte(&ee_Tbw,x);}
#define read_tbw(x) {Tbw = pgm_read_dword(TBW+x);}

#define	ee_read_flt(x) {x = eeprom_read_byte(&ee_flt);}
#define	ee_write_flt(x) {eeprom_write_byte(&ee_flt,x);}


#define	ee_read_mark() {dh = eeprom_read_byte(mark);dm = eeprom_read_byte(mark+1);dl = eeprom_read_byte(mark+2);}
#define	pm_init_const() {tau = pgm_read_word(&PUMP_tau);T = pgm_read_word(&PUMP_T);}
#define mul3(x) {x+=x<<1;}

#define MAIN_TIME_ms(x)	(x*F_CPU/(1000ULL*T0PRESC*(1+OCR0A_REG)))
#define MAIN_TIME_s(x)	(x*F_CPU/(T0PRESC*(1+OCR0A_REG)))
#define MAIN_TIME_m(x)	(x*60*F_CPU/(T0PRESC*(1+OCR0A_REG)))

//относительно автономный блок восприятия нажатия кнопки с антидребезгом. выделен отдельно для облегчения наладки
//код, который надо выполнить при условии, что кнопка была нажата, надо вставить между макросами IF_BUTTON и END_IF_BUTTON
//IF_BUTTON и END_IF_BUTTON должны стоять в цикле, INIT_IF_BUTTON и CLOSE_IF_BUTTON - вне цикла
//игнорирует удержание кнопки
//символа ; не требуют, но и помешать он не должен
//------------ВНИМАНИЕ!!------------- для корректной работы антидребезга вне конструкции должна стоять задержка
#define INIT_IF_BUTTON(_x_) {							\
				uint8_t __nc__ = _x_, __b_nc__=_x_;		\
			

#define IF_BUTTON()	{							\
				if (BUTTON_LEVEL?BUTTON_PORT&BUTTON_MSK:!(BUTTON_PORT&BUTTON_MSK)){	\
					if (__nc__!=0){				\
						__nc__--;			\
						if (__nc__==0){
				
#define END_IF_BUTTON()								\
						}				\
					}					\
				}						\
				else{						\
					__nc__ = __b_nc__;			\
				}						\
			}
#define CLOSE_IF_BUTTON()	}

#endif



#define init(){												\
													\
	DDRB	= 0xFF;											\
	PORTB	= 0xFF;											\
													\
	PORTA	= 0x00;											\
													\
	DDRD	= BUS_MSK|PUMP_MSK;									\
													\
	status	= 0;											\
													\
	PORTA	=	((&CRASH_PORT		==	&PINA)	?CRASH_MSK		:0	)|	\
			((&BUTTON_PORT		==	&PINA)	?BUTTON_MSK		:0	)|	\
			((&SENSOR_CHANGE_PORT	==	&PINA)	?SENSOR_CHANGE_MSK	:0	)|	\
			((&REGEN_PORT		==	&PINA)	?REGEN_MSK		:0	);	\
													\
	PORTD	=	((&CRASH_PORT		==	&PIND)	?CRASH_MSK		:0	)|	\
			((&BUTTON_PORT		==	&PIND)	?BUTTON_MSK		:0	)|	\
			((&SENSOR_CHANGE_PORT	==	&PIND)	?SENSOR_CHANGE_MSK	:0	)|	\
			((&REGEN_PORT		==	&PIND)	?REGEN_MSK		:0	);	\
	TIFR	= 0xFF;		\
	TIMSK	= TIMSK_REG;	\
	TCCR1A	= TCCR1A_REG;	\
	TCCR1B	= TCCR1B_REG;	\
				\
	OCR0A	 = OCR0A_REG;	\
	TCCR0A	 = TCCR0A_REG;	\
	TCCR0B	 = TCCR0B_REG;	\
}


#define BW(){					\
	uint8_t tmp;				\
	while(status&STATUS_BW){		\
		tmp = (t2-main_counter)/30000;	\
		dm  = digs[((uint8_t)tmp)/10];	\
		dl  = digs[((uint8_t)tmp)%10];	\
		dh  = 0;			\
		print_d();			\
		_delay_ms(10);			\
	}					\
}

