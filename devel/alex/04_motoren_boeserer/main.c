// gcc headers
#include <inttypes.h>

// robolib headers
#include "keplerboard.h"

#define getBtn1() ((PINF & _BV(1)) == 0)
#define getBtn2() ((PINF & _BV(2)) == 0)
#define getBtn3()  ((PING & _BV(2)) == 0)

#define setLed1(x) (x ? (PORTF |= _BV(0)) : (PORTF &= ~_BV(0)))
#define setLed2(x) (x ? (PORTF |= _BV(3)) : (PORTF &= ~_BV(3)))

#define setMotorDDR1(x) (x ? (PORTB |= _BV(4)) : (PORTB &= ~_BV(4)))
#define setMotorDDR2(x) (x ? (PORTB |= _BV(3)) : (PORTB &= ~_BV(3)))
#define setMotorDDR3(x) (x ? (PORTB |= _BV(2)) : (PORTB &= ~_BV(2)))

#define getBtnRo() (getBtn1())
#define getBtnRu() (getBtn2())
#define getBtnG()  (getBtn3())
#define setLedRo(x) (setLed1(x))
#define setLedRu(x) (setLed2(x))


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

void motors(int8_t m1, int8_t m2, int8_t m3 ) {
    if (m1 >= 0) {
        setMotorDDR1(1);
    } else {
        setMotorDDR1(0);
        m1 = -m1;
    }
    cli();
    OCR1AH = 0x00;
    OCR1AL = m1;
    sei();

    if (m2 >= 0) {
        setMotorDDR2(1);
    } else {
        setMotorDDR2(0);
        m2 = -m2;
    }
    cli();
    OCR1BH = 0x00;
    OCR1BL = m2;
    sei();

    if (m3 >= 0) {
        setMotorDDR3(1);
    } else {
        setMotorDDR3(0);
        m3 = -m3;
    }
    cli();
    OCR1CH = 0x00;
    OCR1CL = m3;
    sei();
}

int main(void) {

    robolib_init();
    init_hardware();

    sei();
    uint8_t a = 0;
    while (1) {
        a++;
        a &= 0x0F;
        lcdxy(a,3);
        lcdout('.');

        lcdxy(0,1);
        if (getBtn1()) {
            lcdout('1');
            setLed1(1);
            mdelay(500);
            motors(50, 0, 0);
        } else {
            lcdout(' ');
            setLed1(0);
        }

        lcdxy(7,1);
        if (getBtn2()) {
            lcdout('2');
            setLed2(1);
            mdelay(500);
            motors(0, 50, 0);
        } else {
            lcdout(' ');
            setLed2(0);
        }

        lcdxy(15,1);
        if (getBtn3()) {
            lcdout('3');
            mdelay(500);

            motors(0, 0, 0);
        } else {
            lcdout(' ');
        }

        delay_ms(200);
        lcdxy(a,3);
        lcdout(' ');
    }
    return 0;
}
