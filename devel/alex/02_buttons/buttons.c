
/* * * * * * * * * * * * * * * * * *
 * PORTx : write
 * PINx  : read
 * DDRx  : set r(0) or w(1)
 *
 *
 * Hardware   (check 2014/init.h)
 * ------------------------
 *
 * (im Uhrzeigersinn nummeriert)
 *
 * btn1 F1 (red top rig)
 * btn2 F2 (red btm rig)
 * btn3 G2 (grn mid lef)
 *
 * led1 F0 (red top rig)
 * led2 F3 (red btm rig)
 *
 * // note: must be byte or smth, bit is nonsense
 * ifr1    (frn mid) // ?
 * ifr2 F7 (bck rig)
 * ifr3    (bck lef) // ?
 *
 * uls1    (frn rig) // ?
 * uls2    (bck mid) // ?
 * uls3    (frn lef) // ?
 *
 * encoders: E
 * 
 * * * * * * * * * * * * * * * * * */

// gcc headers
#include <inttypes.h>
#include <math.h>

// robolib headers
#include "keplerboard.h"

// custom display_print functions   (... : digits)
#define DEFAULT(a, b) ((a+0)?a:b)
#define display_prints(S) string_from_const(display_print, S);
#define display_printsxy(X, Y, S) display_gotoxy(X, Y), display_prints(S);
#define display_printi(INT, ...) string_from_int(display_print, INT, DEFAULT(__VA_ARGS__, log10(I)))
#define display_printb(BOOL) string_from_int(display_print, BOOL, 1)
#define display_printB(BYTE, ...) (string_from_bits(display_print, BYTE, DEFAULT(__VA_ARGS__, 8)))

// data dir access modes
#define READ   0
#define WRITE ~0
#define setMode(DIR, MODE) (DIR  = MODE)
#define addMode(DIR, MODE) (DIR |= MODE)

// BTNs  buttons
#define BTN1 1
#define BTN2 2
#define BTN3 3
#define BTNS_ALL 0b00000110

#define getBtn(BTN) !((BTN == BTN3 ? PING  & _BV(2) : PINF & _BV(BTN)))

// LEDs  button leds
#define LED1 _BV(0)
#define LED2 _BV(3)
#define LEDS_ALL 0b00001001

#define getLed(LED) (!!(PINF & LED))
#define setLed(LED, X) (X ? (PORTF |= LED) : (PORTF &= ~LED))
#define clearLed(LED) (PORTF &= ~LED)

// BMPs  bumpers  (3 not used)
#define BMP1 _BV(5)
#define BMP2 _BV(2)
#define BMP3 _BV(0)
#define BMP4 _BV(1)
#define BMP5 _BV(6)
#define BMP6 _BV(4)
#define BMP7 _BV(7)
#define BMPS_ALL ~0

#define getBmp(BMP) (!(PINC & BMP))

// IRS  infrared sensors
#define IRS1
#define IRS2
#define IRS3

//#define getIrs(IRS) (PINE & IRS)

// USSs  ultrasound sensors

// MOTs motors (from 2014/DRIVE/motors.h)
//#define motors_ddr_h(x) (x ? (PORTB|=  _BV(4)) : (PORTB&= ~_BV(4)))
//#define motors_ddr_r(x) (x ? (PORTB&= ~_BV(3)) : (PORTB|=  _BV(3)))
//#define motors_ddr_l(x) (x ? (PORTB|=  _BV(2)) : (PORTB&= ~_BV(2)))


// initialize
void init(void) {
    robolib_init();
    display_clear();
    // display_home();
    
    setMode(DDRF, BTNS_ALL & READ);
    addMode(DDRF, LEDS_ALL & WRITE);
    setMode(DDRG, BMPS_ALL & READ);
    
    PORTF = BTNS_ALL;  // set buttons to output
    
    PORTC = BMPS_ALL;  // set bumpers to output
}

int main(void) {
    init();
    
    while (1) {
        display_printsxy(1, 0, "Hardware Tests");
        
        // BTNs and LEDs
        setLed(LED1, getBtn(BTN1));
        setLed(LED2, getBtn(BTN2));
        
        display_printsxy(0, 1, "1B");
        display_printb(getBtn(BTN1));
        display_prints("L");
        display_printb(getLed(LED1));
        
        display_printsxy(0, 2, "2B");
        display_printb(getBtn(BTN2));
        display_prints("L");
        display_printb(getLed(LED2));

        display_printsxy(0, 3, "3B");
        display_printb(getBtn(BTN3));
        
        // bumpers
        display_gotoxy(7, 1);
        display_printB(PINC);
        
        delay_ms(200);
    }
}
