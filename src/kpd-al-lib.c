#define KPD_LIB

#include <kpd-al.h>



const	uint16_t	PROGMEM	sensors[]	= {26,28};
const	uint8_t		PROGMEM	sensors_disp[]	= {D1,D0|PDP,D0,D6|PDP,D0,D0};




uint32_t t1 = 0;
uint32_t t2 = 0;

uint16_t tau;
uint16_t T;
uint32_t Tbw;
uint16_t t3;

uint16_t Kli;

uint8_t dh,dm,dl;
uint8_t status = 0;
const	uint8_t	PROGMEM digs[] asm("DIGS")={D0,D1,D2,D3,D4,D5,D6,D7,D8,D9};


uint32_t main_counter = 0;

uint16_t n_before,n;

uint32_t A=0,B=0,C=0,D=0;


#define pump_push(){			\
	if(PUMP_ON_LEVEL){__asm__ __volatile__("sbi %0,%1\n\t": :"I"(&PORTD-__SFR_OFFSET),"I"(PUMP_PIN));}	\
	else {__asm__ __volatile__("cbi %0,%1\n\t": :"I"(&PORTD-__SFR_OFFSET),"I"(PUMP_PIN));}			\
	status	|= STATUS_T1;		\
}

#define pump_relax(){			\
	if(PUMP_ON_LEVEL){__asm__ __volatile__("cbi %0,%1\n\t": :"I"(&PORTD-__SFR_OFFSET),"I"(PUMP_PIN));}	\
	else {__asm__ __volatile__("sbi %0,%1\n\t": :"I"(&PORTD-__SFR_OFFSET),"I"(PUMP_PIN));}			\
	status &= ~STATUS_T1;		\
	status |= STATUS_T2;		\
}

void print(){
	uint8_t	u;
	for (u = PRINT_DELAY ; u != 0 ; u--){
		print_d();
		_delay_ms(PRINT_DEL);
	}
}


void print_d(){
	PORTB = 0x00;
	switch(PORTD&BUS_MSK){
		case BUS_H_MSK:
			PORTD = (PORTD|BUS_MSK)&((~BUS_MSK)|BUS_M_MSK);
			PORTB = dm;
			break;
		case BUS_M_MSK:
			PORTD = (PORTD|BUS_MSK)&((~BUS_MSK)|BUS_L_MSK);
			PORTB = dl;
			break;
		default:
			PORTD = (PORTD|BUS_MSK)&((~BUS_MSK)|BUS_H_MSK);
			PORTB = dh;
			break;
	}
}

//sensor_counter

uint16_t hiword=0;

ISR(TIMER1_OVF_vect){
	hiword++;
}

ISR(TIMER1_CAPT_vect){
	A   = B;
	B   = C;
	C   = D;
	asm volatile (
	"\tmov %C0, %A1\n"
	"\tmov %D0, %B1\n"
	"\tin %A0, %2\n"
	"\tin %B0, %3\n"
	: "=r"(D)
	: "r" (hiword), "I"(&ICR1L-__SFR_OFFSET) ,"I"(&ICR1H-__SFR_OFFSET)
	);
	if(--n_before==0){
		n_before = n;
		pump_push_normal();
	}
}

//main_counter

inline void pump_push_normal(){
	if(status&PUMP_STATUS_MSK){
		status |= STATUS_OVER;
	}
	else{
		pump_push();
		t1 = main_counter + tau;
		t2 = main_counter + T;
	}
}
inline void pump_push_regen(){
	pump_push();
	t1 = main_counter + tau;
	t2 = main_counter + t3;
}


ISR(TIMER0_COMPA_vect){
	main_counter++;
	if ( main_counter == t1 ){
		PORTB ^= S(B);
		pump_relax();
	}
	if ( main_counter == t2 ){
		status &= ~STATUS_T2;
		if (status&STATUS_OVER){
			pump_push_normal();
		}
		else if (status & STATUS_REGENERARION){
			status&=~STATUS_BW;
			pump_push_regen();
		}
	}
}


EMPTY_INTERRUPT(BADISR_vect);

