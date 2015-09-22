#include <kpd-al.h>
/*
Тест времянки обратной промывки. Т.к. обратная промывка занимает очень большой для контроллера промежуток времени, её приходится тестировать отдельно
Должна вести обратный отсчёт 30 минут
*/
int main (){
	init();
	_delay_ms(500);
	PORTB=S(A);
	Tbw = MAIN_TIME_m(30);
	status |= STATUS_REGENERARION|STATUS_BW;
	t2 = main_counter+Tbw;
	sei();
	BW();
	PORTD = PORTD&(~BUS_MSK);
	PORTB = S(G);
	return 0;
}
