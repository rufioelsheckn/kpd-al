#include <kpd-al.h>



inline void regeneration(){
	while(status&PUMP_STATUS_MSK)print_d();
	cli();
	status |= STATUS_REGENERARION|STATUS_BW;
	t2 = main_counter+Tbw;
	sei();
	BW();
	dh=dm=dl=S(G);
	while((!(REGEN_PORT&REGEN_MSK))^REGEN_LEVEL);
}
