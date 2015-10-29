#ifndef __KPD_AL_VARIABLES_H__
#define __KPD_AL_VARIABLES_H__


//Переменные, связанные с времянкой насоса
#ifdef KPD_LIB
extern				uint8_t		status;		/*Состояние устройста. Используется для определения реакции устройства на событие t2*/
extern				uint32_t	main_counter;	/*Системное время*/
#else
extern		volatile	uint8_t		status;		/*Состояние устройста. Используется для определения реакции устройства на событие t2*/
extern		volatile	uint32_t	main_counter;	/*Системное время*/
#endif

extern				uint32_t	t1;		/*Время, когда насос должен быть выключен*/
extern				uint32_t	t2;		/*Время, когда насос может быть включен снова. В регенерации имеет смысл Tbw и t3*/

extern				uint16_t	tau;		/*Время, в течении которого насос должен быть включен*/
extern				uint16_t	T;		/*Суммарный период работы насоса (t1+t2)*/
extern				uint32_t	Tbw;		/*Время обратной промывки фильтра*/
extern				uint16_t	t3;		/*Период ипульсов насоса в режиме прямой промывки*/

extern				uint32_t	Kli;		/*Kил используемого датчика*/

extern				uint32_t	A,B,C,D;	/*Предыдущие показания воды*/
extern				uint16_t	n;		/*количество импульсов необходимое для включения насоса*/
extern				uint16_t	n_before;	/*Количество импульсов до следующего включения насоса*/

extern				uint8_t		dh,dm,dl;	/*Значения разрядов*/
extern		const		uint8_t		PROGMEM digs[] asm("DIGS");
extern		const		uint32_t	PROGMEM	sensors[];
extern		const		uint8_t		PROGMEM	sensors_disp[];


extern				uint8_t		ee_sensor1, ee_sensor2;
extern				uint8_t		EEMEM	OSCCAL_REG;

extern				uint8_t		EEMEM	OSCCAL_REG;
extern				uint8_t 	EEMEM	mark[3];
extern				uint16_t	EEMEM	N[MAX_N];
extern				uint8_t		EEMEM	ee_N;
extern				uint8_t		EEMEM	ee_Tbw;
extern				uint8_t		EEMEM	ee_flt;

extern				uint8_t		EEMEM	ee_sensor1, ee_sensor2;

#endif
