#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define MS_DELAY 3000
#define DUTY_BITS 4
#define DUTY_MAX (1 << DUTY_BITS)

uint8_t R = PORTB4;
uint8_t G = PORTB5;
uint8_t B = PORTB3;

static uint8_t r_duty = 0;
static uint8_t g_duty = 0;
static uint8_t b_duty = 0;

static uint8_t r_counter = 0;
static uint8_t g_counter = 0;
static uint8_t b_counter = 0;

static uint32_t global_counter = 0;

void pwm(uint8_t *counter, uint8_t duty, uint8_t pin) {
    if (*counter < duty) {
        PORTB |= _BV(pin);
    } else {
        PORTB &= ~_BV(pin);
    }
    *counter = (*counter + 1) % DUTY_MAX;
}

ISR(TIMER1_COMPA_vect) {

    //Smooth - go 0, 1, ... DUTY_MAX - 1, DUTY_MAX - 1, DUTY_NAX - 2, ..., 1, 0, 1, ...
    r_duty = (global_counter >> (3 * (DUTY_BITS + 1))) % (2 * DUTY_MAX);
    if (r_duty > DUTY_MAX) {
        r_duty = 2 * DUTY_MAX - r_duty;
    }
    g_duty = (global_counter >> (2 * (DUTY_BITS + 1))) % (2 * DUTY_MAX);
    if (g_duty > DUTY_MAX) {
        g_duty = 2 * DUTY_MAX - g_duty;
    }
    b_duty = (global_counter >> (1 * (DUTY_BITS + 1))) % (2 * DUTY_MAX);
    if (b_duty > DUTY_MAX) {
        b_duty = 2 * DUTY_MAX - b_duty;
    }

    pwm(&r_counter, r_duty, R);
    pwm(&g_counter, g_duty, G);
    pwm(&b_counter, b_duty, B);

    global_counter++;
}

int main (void) {
    /*Set to one the fifth bit of DDRB to one
    **Set digital pin 13 to output mode */
    DDRB |= _BV(DDB4); // R
    DDRB |= _BV(DDB5); // G
    DDRB |= _BV(DDB3); // B

    TCCR1B |= (1 << WGM12);             // CTC
    OCR1A = 62;                         // 16MHz/64/62 ≈ 4kHz interrupt
    TIMSK1 |= (1 << OCIE1A);            // enable compare interrupt
    TCCR1B |= (1 << CS11) | (1 << CS10);// prescaler 64

    sei();

    while(1) {;
    }
}
