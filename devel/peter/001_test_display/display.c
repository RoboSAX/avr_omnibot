/******************************************************************************
* examples/display.c                                                          *
* ==================                                                          *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 27.09.15                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/RoboAG/avr_robolib                                     *
******************************************************************************/

// include from gcc
#include <inttypes.h>

// include all necessary headers from robolib
#include "keplerboard.h"

int main (void) {
  // initialize robolib
  robolib_init();

  // draw a smiley on the Display
  display_clear();
  string_from_const(display_print, "Display :-)");

  // variable fo storing last position of "*"
  uint8_t position = 0;

  // endless loop
  while (1) {
    // change the smiley
    display_gotoxy(10,0);
    display_print('D');

    // wait for 0.25 seconds
    delay_ms(500);

    // change the smiley
    display_gotoxy(10,0);
    display_print('|');

    // wait for 0.25 seconds
    delay_ms(500);


    // clear old "*"
    display_gotoxy(position,1);
    display_print(' ');

    // calculate new position
    ++position;
    if (position > 15) {position = 0;}

    // draw new "*"
    display_gotoxy(position,1);
    display_print('*');
  }

  return (0);
}
