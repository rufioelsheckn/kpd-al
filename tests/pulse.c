#include <kpd-al.h>
/*
Тест импульсов в штатном режиме и режиме регенерации
Когда загорятся сегменты A, тест будет ожидать нажатия кнопки (кнопку необходимо удерживать в течении 0,5с)
При первом нажатии сработает эмитация обычного импульса: секунда должны гореть сегменты G, потом секунду D
При втором нажатии сработает эмитация режима регенерации: секунда сегменты G, 9 секунд сегменты D
Реакция на последующие нажатия эквивалентна первым двум
*/
int main (){
	init();
//	_delay_ms(500);
	PORTB = S(A);
	tau = MAIN_TIME_ms(1000);
	T = MAIN_TIME_ms(2000);
	t3 = MAIN_TIME_ms(10000);
	sei();
	pump_push_normal();
	PORTB = S(G);
	while((status&STATUS_T1)!=0);
	PORTB = S(D);
	while((status&STATUS_T2)!=0);
	PORTB = S(A);
	while(1){
		while((status&STATUS_T1)==0){
			IF_BUTTON(3)
				pump_push_normal();
			END_IF_BUTTON(3)
			_delay_ms(10);
		}
		PORTB = S(G);
		while((status&STATUS_T1)!=0);
		PORTB = S(D);
		while((status&STATUS_T2)!=0);
		PORTB = S(A);
		while((status&STATUS_T1)==0){
			IF_BUTTON(3)
				pump_push_regen();
			END_IF_BUTTON(3)
			_delay_ms(10);
		}
		PORTB = S(G);
		while((status&STATUS_T1)!=0);
		PORTB = S(D);
		while((status&STATUS_T2)!=0);
		PORTB = S(A);
	}
	return 0;
}
