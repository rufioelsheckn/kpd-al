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
	div_1000_dp(Kli/((uint16_t)((D-A)>>2)));
}
