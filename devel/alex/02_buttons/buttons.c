#define led1(x) ( x ? (PORTE |=  _BV(2)) : (PORTE &=  ~_BV(2))
#define taster1() (PINF & _BV(2))

/*
 * btn2 F2
 * ir2  F7
 * 
 * */

// include from gcc
#include <inttypes.h>

// include all necessary headers from robolib
#include "keplerboard.h"

void init(void) {
    
    DDRF  = 0x00;
    PORTF = _BV(2);
       // PIN 2: Taster 2 (ak roten unten)
    
    
    display_clear();
}

int main (void) {
  // initialize robolib
  robolib_init();
  init();
   
   while(1) {
   
        display_gotoxy(10,0);
        uint8_t t1 = taster1();
        display_print("01"[!!t1]);
       
       delay_ms(200);
       
       
}
}