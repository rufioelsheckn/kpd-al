#include <kpd-al.h>

void inline refresh();

inline void service(){
	uint8_t regen_on = 3;
	sei();
	while(1){
		print_d();
		_delay_ms(10);
		if ((!(REGEN_PORT&REGEN_MSK))^REGEN_LEVEL){
			if (--regen_on==0)regeneration();
		}
		else{
			regen_on = 3;
		}
		if (((uint8_t)main_counter)&(1<<4)){
			status |= STATUS_REFRESH;
		}
		else{
			if ( status&STATUS_REFRESH ){
				status &= ~STATUS_REFRESH;
				refresh();
			}
		}
	}
}

void inline refresh(){
	uint32_t tmp = (Kli/((D-A)>>2));
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
