//-------| src/dot.c |-------//
#include "dot.h"


static unsigned short dot_decimal[10][MAX_DOT] = {
	{0x01, 0x02, 0x04, 0x08, 0x40},
	{0x02, 0x14, 0x78, 0x15, 0x02},
	{0x04, 0x14, 0x78, 0x14, 0x03},
	{0x08, 0x29, 0x73, 0x29, 0x04},
	{0x01, 0x4A, 0x6C, 0x4B, 0x10}
};

static short * dot[MAX_DOT];

/* mmap() 된 주소를 받아오는 함수 */
void init_dot(short * address[]) {
	int i;
	for( i=0; i<MAX_DOT; i++ ) {
		dot[i] = address[i];
	}
}

/* 화면을 지우는 함수 */
void dot_clear() {
	int i;
	for(i=0; i<MAX_DOT; i++){
		*dot[i] =  0;
	}
	usleep(0); // for Ximulator
}

/* 화면에 숫자를 그리는 함수 */
void dot_write(int decimal) {
	int i;
	for(i=0; i<MAX_DOT; i++){
		   *dot[i] = dot_decimal[decimal][i];
	}
	usleep(0); // for Ximulator
}

/* 화면의 숫자가 50ms마다 올라가는 함수 */
void dot_up_shift(int decimal) {
	int shift, i;
	for( shift=0; shift<=7; shift++ ) {
		for(i=0; i<MAX_DOT; i++){
			*dot[i] = dot_decimal[decimal][i] << shift;
		}
		usleep(50000);
	}
}

/* 화면의 숫자가 50ms마다 내려가는 함수 */
void dot_down_shift(int decimal) {
	int shift, i;
	for( shift=0; shift<=7; shift++ ) {
		for(i=0; i<MAX_DOT; i++){
			*dot[i] = dot_decimal[decimal][i] >> shift;
		}
		usleep(50000);
	}	
}

/* 화면을 0.3초 간격으로 흑백 반전하고 지우는 함수 */
void dot_inverse(int decimal) {
	int shift, i;
	for( shift=0; shift<3; shift++ ) {
		for(i=0; i<MAX_DOT; i++){
			*dot[i] = dot_decimal[decimal][i];
		}
		usleep(300000);
		for(i=0; i<MAX_DOT; i++){
			*dot[i] = ~dot_decimal[decimal][i];
		}
		usleep(300000);	
	}
	dot_clear();
}
