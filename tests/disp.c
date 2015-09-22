#include <kpd-al.h>
/*
Тест системы индикации

Все действия выполняются с задержкой в 500 мс

Шаг 1: (работоспособность индикатора)
	зажечь все сегменты всех разрядов
Шаг 2: (шина данных)
	на всех индикаторах должны поочерёдно загореться сегменты A-B-C-D-E-F-G-DP
Шаг 3: (шина адреса)
	горят все сегменты на разрядах поочерёдно: сотни-десятки-единицы
Шаг 4: (проверка print_d)
	аналогично шагу 3
Шаг 5: (проверка print)
	аналогично шагу 1
Шаг 5: (проверка print)
	аналогично шагу 1
Шаг 6: (проверка цифр)
	на дисплее должны последовательно появиться числа: 012 345 678 90.0
*/
int main(){
//шаг 1
	init();
	_delay_ms(500);
//шаг 
	PORTB = S(A);
	_delay_ms(500);
	PORTB = S(B);
	_delay_ms(500);
	PORTB = S(C);
	_delay_ms(500);
	PORTB = S(D);
	_delay_ms(500);
	PORTB = S(E);
	_delay_ms(500);
	PORTB = S(F);
	_delay_ms(500);
	PORTB = S(G);
	_delay_ms(500);
	PORTB = S(DP);
	_delay_ms(500);
//шаг 3
	PORTB = 0xFF;
	_delay_ms(500);
	PORTD = (PORTD|BUS_MSK)&((~BUS_MSK)|BUS_H_MSK);
	_delay_ms(500);
	PORTD = (PORTD|BUS_MSK)&((~BUS_MSK)|BUS_M_MSK);
	_delay_ms(500);
	PORTD = (PORTD|BUS_MSK)&((~BUS_MSK)|BUS_L_MSK);
	_delay_ms(500);
//шаг 4
	dh = dm = dl = 0xFF;
	
	print_d();
	_delay_ms(500);
	print_d();
	_delay_ms(500);
	print_d();
	_delay_ms(500);
	
//шаг 5
	print();
//шаг 6
	dh = pgm_read_byte(digs+0);
	dm = pgm_read_byte(digs+1);
	dl = pgm_read_byte(digs+2);
	print();
	dh = pgm_read_byte(digs+3);
	dm = pgm_read_byte(digs+4);
	dl = pgm_read_byte(digs+5);
	print();
	dh = pgm_read_byte(digs+6);
	dm = pgm_read_byte(digs+7);
	dl = pgm_read_byte(digs+8);
	print();
	dh = pgm_read_byte(digs+9);
	dm = pgm_read_byte(digs+0)|S(DP);
	dl = pgm_read_byte(digs+0);
	print();
	PORTB = 0;

	return 0;
}
