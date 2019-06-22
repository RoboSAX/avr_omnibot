
/* * * * * * * * * * * * * * * * * *
 * PORTx : write
 * PINx  : read
 * DDRx  : set r(0) or w(1)
 *
 *
 * Hardware
 * ------------------------
 *
 * (im Uhrzeigersinn nummeriert)
 *
 * btn1 F1 (red top rig) // ?
 * btn2 F2 (red btm rig)
 * btn3 F3 (grn mid lef) // ?
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
 * * * * * * * * * * * * * * * * * */

// gcc headers
#include <inttypes.h>
#include <math.h>

// robolib headers
#include "keplerboard.h"

#define BTN1 _BV(1)
#define BTN2 _BV(2)
#define BTN3 _BV(3)

//#define IRS1
//#define IRS2
//#define IRS3

//#define setLed(_P, x) ( x ? (PORTE |=  _BV(2)) : (PORTE &=  ~_BV(2))

#define getBtn(BTN) (PINF & BTN)
//#define getIrs(IRS) (PINE & IRS)

#define display_prints(S) string_from_const(display_print, S);
#define display_printsxy(X, Y, S) \
    display_gotoxy(X, Y), display_prints(S);
#define display_printi(I) string_from_int(display_print, I, log10(I))

// initialize
void init(void) {
    robolib_init();
    display_clear();
    display_home();

    DDRF = 0x00; // set Buttons to Input
    // PORTF = _BV(2); // set led2 to Output
}

int main(void) {
    init();

    while (1) {
        display_printsxy(3, 0, "* Hardware Tests *");

        display_gotoxy(0, 1);
        display_prints("BTN1:");
        display_printi(!getBtn(BTN1));

        display_prints(" BTN2:");
        display_printi(!getBtn(BTN2));

        display_prints(" BTN3:");
        display_printi(!getBtn(BTN3));

        delay_ms(200);
    }
}
