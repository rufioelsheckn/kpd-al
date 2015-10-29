#include <kpd-al.h>


const	uint32_t	PROGMEM	TBW[]		= {1,MAIN_TIME_m(4)+1,MAIN_TIME_m(6)+1,MAIN_TIME_m(8)+1,MAIN_TIME_m(10)+1,MAIN_TIME_m(12)+1,MAIN_TIME_m(16)+1,MAIN_TIME_m(20)+1,MAIN_TIME_m(24)+1};
const	uint16_t	PROGMEM	PUMP_tau	= MAIN_TIME_ms(100);
const	uint16_t	PROGMEM	PUMP_T		= MAIN_TIME_ms(200);
const	uint16_t	PROGMEM	D_K[]		= {60,90,100,120,150};
const	uint8_t		PROGMEM	D_MARK[]	= {D1,D0,D1,D2,D1,D3,D1,D4,D1,D6};



void prog_qmax(int8_t add);
void makeQmax();
void read_ftl(uint8_t x);

void makeTbw();

int main(){
	init();
	_delay_ms(500);
	
	if (BUTTON_LEVEL?(BUTTON_PORT&BUTTON_MSK):!(BUTTON_PORT&BUTTON_MSK)){
		status	= STATUS_NEED_PROGRAM;
	}

	pm_init_const();
	
	ee_read_mark();
	
	print();

	if (BUTTON_LEVEL?!(BUTTON_PORT&BUTTON_MSK):(BUTTON_PORT&BUTTON_MSK)){
		status = 0;
	}
	
	{
		uint8_t u;
		u = eeprom_read_byte(SENSOR_CHANGE_PORT&SENSOR_CHANGE_MSK?&ee_sensor2:&ee_sensor1);
		Kli = pgm_read_dword(sensors+u);
		u*=3;

		asm volatile (
			"add %A3,%4\n\t"
			"adc %B3,__zero_reg__\n\t"
			"lpm %0,Z+\n\t"
			"lpm %1,Z+\n\t"
			"lpm %2,Z\n\t"
			:"=a"(dh),"=a"(dm),"=a"(dl)
			:"z"(sensors_disp),"a"(u)
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
	n_before = n;
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
		_delay_ms(PRINT_DEL);
	}
	CLOSE_IF_BUTTON();
	dh=dm=dl=S(D);
	INIT_IF_BUTTON(3);
	for ( u = PROG_WAIT ; u != 0 ; --u ){
		IF_BUTTON()
			prog_qmax(-1);
			goto l1;
		END_IF_BUTTON()
		_delay_ms(PRINT_DEL);
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
		_delay_ms(PRINT_DEL);
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
		_delay_ms(PRINT_DEL);
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
		_delay_ms(PRINT_DEL);
	}
	CLOSE_IF_BUTTON();
	ee_write_n(tmp);
}



void makeQmax(){
	uint32_t tmp = (n*Kli/T/1248);
	dh = DIGS(tmp/1000);
	tmp %= 1000;
	dm = DIGS(tmp/100)|S(DP);
	tmp %= 100;
	dl = DIGS(tmp/10);
	if (dh == D0){
		dh = dm;
		dm = dl;
		dl = DIGS(tmp%10);
	}
}

void makeTbw(){
	uint8_t tmp = Tbw/MAIN_TIME_m(1);
	dh  = DIGS(tmp/10);
	dm  = DIGS(tmp%10);
	dh  = 0;
}



void read_ftl(uint8_t x){
	asm volatile (
		"lsl %3\n\t"
		"add %A2,%3\n\t"
		"adc %B2,__zero_reg__\n\t"
		"lpm %0,Z+\n\t"
		"lpm %1,Z\n\t"
		:"=a"(dh),"=a"(dm)
		:"z"(D_MARK),"r"(x)
	);
}
