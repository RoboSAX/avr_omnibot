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


void init_hardware(void) {
    // PORTA - Display
    //   Pin 0: Databit 4
    //   Pin 1: Databit 5
    //   Pin 2: Databit 6
    //   Pin 3: Databit 7
    //   Pin 4: Enable
    //   Pin 5: RS
    //   Pin 6: Read/Write
    //   Pin 7: Frei
    // durch system.h angesteuert und gesetzt

    // PORTB - Motortreiber
    //   Pin 0: Frei
    //   Pin 1: Frei
    //   Pin 2: Motor3 DDR
    //   Pin 3: Motor2 DDR
    //   Pin 4: Motor1 DDR
    //   Pin 5: Motor1 PWM (OC1A)
    //   Pin 6: Motor2 PWM (OC1B)
    //   Pin 7: Motor3 PWM (OC1C)
    DDRB  = 0xFC;
    PORTB = 0x00;

    // PORTC - Bumper
    //   Pin 0: Frei (Taster3)
    //   Pin 1: Bumper Hinten Links
    //   Pin 2: Bumper Hinten Rechts
    //   Pin 3: Bumper Rechts Rechts
    //   Pin 4: Bumper Rechts Links
    //   Pin 5: Bumper Tennisball
    //   Pin 6: Bumper Links  Links
    //   Pin 7: Bumper Links  Rechts
    DDRC  = 0x00;
    PORTC = 0xFE;

    // PORTD - TWI, XBEE, Taster
    //   Pin 0: TWI SCL
    //   Pin 1: TWI SDA
    //   Pin 2: XBEE TX (RXD1)
    //   Pin 3: XBEE RX (TXD1)
    //   Pin 4: ?
    //   Pin 5: ?
    //   Pin 6: ?
    //   Pin 7: ?
    DDRD  = 0x99;
    PORTD = 0x60;

    // PORTE - UART, Encoder
    //   Pin 0: UART TX (RXD0)
    //   Pin 1: UART RX (TXD0)
    //   Pin 2: Encoder 1
    //   Pin 3: Encoder 2
    //   Pin 4: Encoder 3
    //   Pin 5: Encoder 1 (INT5)
    //   Pin 6: Encoder 2 (INT6)
    //   Pin 7: Encoder 3 (INT7)
    DDRE  = 0x01;
    PORTE = 0x00;

    // PORTF - Analoge Sensoren
    //   Pin 0: Taster 1 Led
    //   Pin 1: Taster 1 Taster
    //   Pin 2: Taster 1 Taster
    //   Pin 3: Taster 2 Led
    //   Pin 4: (3/4 Sharp)
    //   Pin 5: (3/4 Sharp)
    //   Pin 6: (3/4 Sharp)
    //   Pin 7: (3/4 Sharp)
    DDRF  = 0x09;
    PORTF = 0x06;

    // PORTG - Frei
    //   Pin 0: Frei
    //   Pin 1: Frei
    //   Pin 2: Bootpin
    DDRG  = 0x00;
    PORTG = 0x02;


    // Timer 1 - Motoren
    ICR1H = 0x00;
    ICR1L =  100;  // Max. 100%

    TCCR1A = 0x00; // phasen- und frequenzrichtiger PWM bis ICR1
    TCCR1B = _BV(CS12) | _BV(WGM13); // ... PWM bis ICR1; Prescaler 256

    TCNT1H = 0x00;
    TCNT1L = 0x00;

    OCR1AH = 0x00;
    OCR1AL = 0x00;

    OCR1BH = 0x00;
    OCR1BL = 0x00;

    OCR1CH = 0x00;
    OCR1CL = 0x00;

    mdelay(10);
    TCCR1A|= _BV(COM1A1) | _BV(COM1B1) | _BV(COM1C1); // Ausgänge an
}

#define MOT1 _BV(3)
#define MOT2 _BV(4)
#define MOT3 _BV(2)
#define setMotorDDR(MOT, VAL) (VAL ? (PORTB |= MOT) : (PORTB &= ~MOT))
#define _setMotor1(SPD) OCR1BH = 0x00, OCR1BL = speed
#define _setMotor2(SPD) OCR1AH = 0x00, OCR1AL = speed
#define _setMotor3(SPD) OCR1CH = 0x00, OCR1CL = speed

void setMotor(int8_t motor, int8_t speed) {
    setMotorDDR(motor, speed >= 0);
    if(speed < 0) speed = -speed;

    uint8_t mSREG = SREG; // save global interrupt enable bit 
    cli();                // disable interrupts
    switch(motor) {
        case MOT1: _setMotor1(speed); break;
        case MOT2: _setMotor2(speed); break;
        case MOT3: _setMotor3(speed); break;
    }
    SREG = mSREG; // restore global interrupt enable bit 
}

void setMotors(int8_t m1, int8_t m2, int8_t m3) {
    setMotor(MOT1, m1);
    setMotor(MOT2, m2);
    setMotor(MOT3, m3);
}

int main (void) {
    // initialize robolib
    robolib_init();
    init_hardware();

    // endless loop
    while (1) {
        setMotor(MOT1, getBtn(BTN1) * 50);
        setMotor(MOT2, getBtn(BTN2) * 50);
        setMotor(MOT3, getBtn(BTN3) * 50);
        delay_ms(300);
    }

    return 0;
}


/******************************************************************************
*                                                                             *
* How to compile this example                                                 *
* ===========================                                                 *
*                                                                             *
* required files                                                              *
*   + display.c             (source code, this file)                          *
*   + display_header.ini    (headerfile as '.ini')                            *
*   + Makefile              (needed for compiling, linking and downloading)   *
*                                                                             *
* 1. adjust Makefile                              (only needed once)          *
*   + set "MCU" to the controller type              (e.g. atmega64)           *
*   + set "PATH_LIB" to the library                 (e.g. ~/avr/robolib)      *
*                                                                             *
* 2. compile library and create headerfile        (only needed once)          *
*   + open a console                                (e.g. strg+alt+t)         *
*   + change to the directory of the files          (e.g. cd ~/avr/test/)     *
*   + run make with "ini" as parameter              (e.g. make ini)           *
*     ==> check output for errors                                             *
*                                                                             *
* 3. compile main file and link everything        (needed for all changes)    *
*   + open a console                                (e.g. strg+alt+t)         *
*   + change to the directory of the files          (e.g. cd ~/avr/test/)     *
*   + run make with "all" as parameter              (e.g. make all)           *
*     ==> check output for errors                                             *
*                                                                             *
* How to download this example                                                *
* ============================                                                *
*                                                                             *
* required files                                                              *
*   + out.hex          (machine code, created in the steps above)             *
*   + Makefile         (needed for compiling, linking and downloading)        *
*                                                                             *
* 1. adjust Makefile                              (only needed once)          *
*   + set "COMPORT_PROG" to the serial device       (e.g. /dev/ttyS0)         *
*   + set "BAUDRATE_PROG" to the speed              (e.g. 57600)              *
*                                                                             *
* 2. download result                              (needed for all changes)    *
*   + open a console                                (e.g. strg+alt+t)         *
*   + change to the directory of the files          (e.g. cd ~/avr/test/)     *
*   + run make with "program" as parameter          (e.g. make program)       *
*     ==> check output for errors                                             *
*                                                                             *
******************************************************************************/
