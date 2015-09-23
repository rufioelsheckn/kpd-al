#include <kpd-al.h>


const	uint32_t	PROGMEM	TBW[]		= {1,MAIN_TIME_m(4),MAIN_TIME_m(6),MAIN_TIME_m(8),MAIN_TIME_m(10),MAIN_TIME_m(12),MAIN_TIME_m(16),MAIN_TIME_m(20),MAIN_TIME_m(24)};
const	uint16_t	PROGMEM	PUMP_tau	= MAIN_TIME_ms(100);
const	uint16_t	PROGMEM	PUMP_T		= MAIN_TIME_ms(200);
const	uint16_t	PROGMEM	D_K[]		= {60,90,100,120,150};
const	uint8_t		PROGMEM	D_MARK[]	= {D1,D0,D1,D2,D1,D3,D1,D4,D1,D6};




uint8_t 	EEMEM	mark[3]			= {D1,D2,D3};
uint16_t	EEMEM	N[MAX_N]		= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
uint8_t		EEMEM	ee_N			= 0;
uint8_t		EEMEM	ee_Tbw			= 0;
uint8_t		EEMEM	ee_flt			= 0;

uint8_t		EEMEM	ee_sensor1=0, ee_sensor2=1;

void prog_qmax(int8_t add);
void makeQmax();
void read_ftl(uint8_t x);


int main(){
	init();
	_delay_ms(500);
	
	if ((!(BUTTON_PORT&BUTTON_MSK))^BUTTON_LEVEL){
		status	|= STATUS_NEED_PROGRAM;
	}

	pm_init_const();
	
	ee_read_mark();
	
	print();

	if ((status&&STATUS_NEED_PROGRAM)&&((!(BUTTON_PORT&BUTTON_MSK))^BUTTON_LEVEL)){
		status	|= STATUS_NEED_PROGRAM;
	}
	else{
		status = 0;
	}
	
	{
		uint8_t u;
		u = eeprom_read_byte(SENSOR_CHANGE_PORT&SENSOR_CHANGE_MSK?&ee_sensor2:&ee_sensor1);
		Kli = pgm_read_word(sensors+u);
		mul3(u);
		
		asm volatile (
			"add %A2,%4\n\t"
			"adc %B2,__zero_reg__\n\t"
			"lpm %0,Z+\n\t"
			"lpm %1,Z+\n\t"
			"lpm %2,Z\n\t"
			:"=r"(dh),"=r"(dm),"=r"(dl)
			:"e"(sensors_disp),"r"(u)
		);
	}
	
	print();
	
	ee_read_n(n);
	read_n(n);
	makeQmax();
	print();
	
	if (status&&STATUS_NEED_PROGRAM){
		programming();
	}
	t3 = ((uint32_t)n)*Kli/1248/pgm_read_byte(D_K+eeprom_read_byte(&ee_flt));
	service();
	return 0;
}


inline void programming(){
	uint8_t u, tmp;
	
	ee_read_n(tmp);
	read_n(tmp);
	makeQmax();
	
	print();
	
	dh=dm=dl=S(A);
	INIT_IF_BUTTON(3);
	for ( u = PROG_WAIT ; u != 0 ; --u ){
		IF_BUTTON();
			prog_qmax(1);
			goto l1;
		END_IF_BUTTON();
		_delay_ms(10);
	}
	CLOSE_IF_BUTTON();
	dh=dm=dl=S(D);
	INIT_IF_BUTTON(3);
	for ( u = PROG_WAIT ; u != 0 ; --u ){
		IF_BUTTON()
			prog_qmax(-1);
			goto l1;
		END_IF_BUTTON()
		_delay_ms(10);
	}
	CLOSE_IF_BUTTON();
	
l1:	ee_read_tbw(tmp);
	read_tbw(tmp);
	makeTbw();
	INIT_IF_BUTTON(3);
	for ( u = PROG_WAIT ; u != 0 ; --u ){
		IF_BUTTON();
			tmp++;
			if ( tmp==sizeof(TBW)/sizeof(uint32_t) ) tmp = 0;
			read_tbw(tmp)
			makeTbw();
			u=PROG_WAIT;
		END_IF_BUTTON();
		print_d();
		_delay_ms(10);
		if ( dl ){
			dl = 0;
		}
		else{
			dl = DT;
		}
	}
	CLOSE_IF_BUTTON();
	ee_write_tbw(tmp);
	
	ee_read_flt(tmp);
	read_ftl(tmp);
	dl = 0;
	INIT_IF_BUTTON(3);
	for ( u = PROG_WAIT ; u != 0 ; --u ){
		IF_BUTTON()
			tmp++;
			if ( tmp==sizeof(D_MARK)/sizeof(uint8_t) ) tmp = 0;
			read_ftl(tmp);
			u=PROG_WAIT;
		END_IF_BUTTON()
		print_d();
		_delay_ms(10);
		if ( dl ){
			dl = 0;
		}
		else{
			dl = DINCH;
		}
	}
	CLOSE_IF_BUTTON();
	ee_write_flt(tmp);
}

void prog_qmax(int8_t add){
	uint8_t u, tmp;
	ee_read_n(tmp);
	read_n(tmp);
	makeQmax();
	INIT_IF_BUTTON(3);
	for ( u = PROG_WAIT ; u != 0 ; --u ){
		IF_BUTTON()
			tmp+=add;
			if ( tmp==255 ) tmp = MAX_N-1;
			else if ( tmp==MAX_N ) tmp = 0;
			read_n(tmp)
			makeQmax();
			u=PROG_WAIT;
		END_IF_BUTTON()
		print_d();
		_delay_ms(10);
	}
	CLOSE_IF_BUTTON();
	ee_write_n(tmp);
}



void makeQmax(){
	div_10000(((uint32_t)n)*Kli/T/1248);
}

void makeTbw(){
	uint16_t tmp = Tbw/3004;
	dh  = digs[((uint8_t)tmp)/10];
	dm  = digs[((uint8_t)tmp)%10];
	dh  = 0;
}



void read_ftl(uint8_t x){
	asm volatile (
		"lsl %3\n\t"
		"add %A2,%3\n\t"
		"adc %B2,__zero_reg__\n\t"
		"lpm %0,Z+\n\t"
		"lpm %1,Z\n\t"
		:"=r"(dh),"=r"(dm)
		:"e"(D_MARK),"r"(x)
	);
}
