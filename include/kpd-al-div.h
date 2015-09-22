#ifndef __KPD_AL_DIV_H__
#define __KPD_AL_DIV_H__


#define div_10000(value) \
	__asm__ __volatile__ ( \
		";div_10000 BEGIN\n" \
		"\tldi %0, -1\n" \
		"\tldi %1, lo8(1000)\n" \
		"\tldi %2, hi8(1000)\n" \
		"L_div_0%=:inc %0\n" \
		"\tsub %A3, %1\n" \
		"\tsbc %B3, %2\n" \
		"\tbrcc L_div_0%=\n" \
		"\tadd %A3, %1\n" \
		"\tadc %B3, %2\n" \
		"\tldi %1, -1\n" \
		"L_div_1%=:inc %1\n" \
		"\tsubi %A3, 100\n" \
		"\tsbc %B3, __zero_reg__\n" \
		"\tbrcc L_div_1%=\n" \
		"\tsubi %A3, -100\n" \
		"\tldi %2, -1\n" \
		"L_div_2%=:inc %2\n" \
		"\tsubi %A3, 10\n" \
		"\tbrcc L_div_2%=\n" \
		"\tsubi %A3, -10\n" \
		"\tclr __tmp_reg__\n" \
		"\ttst %0\n" \
		"\tbreq L_div_exit%=\n" \
		"\tmov %0, %1\n" \
		"\tmov %1, %2\n" \
		"\tmov %2, %A3\n" \
		"\tinc __tmp_reg__\n" \
		"L_div_exit%=:\n" \
		"\tldi %A3, lo8(DIGS)\n" \
		"\tldi %B3, hi8(DIGS)\n" \
		"\tadd %A3, %0\n" \
		"\tadc %B3, __zero_reg__\n" \
		"\tlpm %0, Z\n" \
		"\tsbrc __tmp_reg__, 0\n" \
		"\tori %0, %4\n" \
		"\tldi %A3, lo8(DIGS)\n" \
		"\tldi %B3, hi8(DIGS)\n" \
		"\tadd %A3, %1\n" \
		"\tadc %B3, __zero_reg__\n" \
		"\tlpm %1, Z\n" \
		"\tsbrs __tmp_reg__, 0\n" \
		"\tori %1, %4\n" \
		"\tldi %A3, lo8(DIGS)\n" \
		"\tldi %B3, hi8(DIGS)\n" \
		"\tadd %A3, %2\n" \
		"\tadc %B3, __zero_reg__\n" \
		"\tlpm %2, Z\n" \
		";div_10000 END\n" \
		: "=a"(dh), "=a" (dm), "=a" (dl) \
		: "e"((uint16_t)(value)), "M"(S(DP)) \
	)

#define div_1000_dp(value) \
	__asm__ __volatile__ ( \
		";div_1000 BEGIN\n" \
		"\tldi %0,-1\n" \
		"L_div_1%=:inc %0\n" \
		"\tsubi %A3,100\n" \
		"\tsbc %B3,__zero_reg__\n" \
		"\tbrcc L_div_1%=\n" \
		"\tsubi %A3,-100\n" \
		"\tldi %1,-1\n" \
		"L_div_2%=:inc %1\n" \
		"\tsubi %A3,10\n" \
		"\tbrcc L_div_2%=\n" \
		"\tsubi %A3,-10\n" \
		"\tmov %2,%A3\n" \
		"\tcpi %0, 10\n" \
		"\tbrcs L_div_exit%=\n" \
		"\tldi %1,9\n" \
		"\tmov %0,%1\n" \
		"\tmov %2,%1\n" \
		"L_div_exit%=:\n" \
		"\tldi %A3, lo8(DIGS)\n" \
		"\tldi %B3, hi8(DIGS)\n" \
		"\tadd %A3, %0\n" \
		"\tadc %B3, __zero_reg__\n" \
		"\tlpm %0, Z\n" \
		"\tori %0, %4\n" \
		"\tldi %A3, lo8(DIGS)\n" \
		"\tldi %B3, hi8(DIGS)\n" \
		"\tadd %A3, %1\n" \
		"\tadc %B3, __zero_reg__\n" \
		"\tlpm %1, Z\n" \
		"\tldi %A3, lo8(DIGS)\n" \
		"\tldi %B3, hi8(DIGS)\n" \
		"\tadd %A3, %2\n" \
		"\tadc %B3, __zero_reg__\n" \
		"\tlpm %2, Z\n" \
		";div_1000 END\n" \
		: "=a"(dh), "=a"(dm), "=a"(dl) \
		: "e"((uint16_t)(value)), "M"(S(DP)) \
	)
#endif
