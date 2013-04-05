#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "repeater.h"
#include "morse.h"

ISC(TIMER1_COMPA_vect) {
    // We always advance the CID timer, this is the first item to check because
    // we just mix it in with active QSO. This also means all the other timers
    // get delayed if we send out our CID.
    if (++status.cid_timer >= CID_TIMER) {
        cid_open();
        return 0;
    }

    // Check the QSO status
    switch(status.qso_status) {
        // If we're in a normal QSO, check a few timers
        case QSO_OPEN:
            // .. if we exceed the KILL_TIMER, kill off active QSO and cool
            // down
            if (++status.qso_timer >= KILL_TIMER) {
                qso_kill();
            }
            break;
        case QSO_HANG:
            // .. if we exceed the HANG_TIMER (waiting for new data on the VOX
            // detection circuit), shut the QSO
            if (++status.hang_timer >= HANG_TIMER) {
                qso_shut();
            }
            break;
        case QSO_KILL:
            // .. if we exceed the KILL_TIMER, reset the QSO status
            if (++status.kill_timer >= KILL_SLEEP) {
                qso_none();
            }
            break;
    }

    return 0;
}

void ptt_open(void) {
    PORTD |= (1 << LED_PTT);
    PORTC |= (1 << PIN_PTT);
}

void ptt_shut(void) {
    PORTC &= ~(1 << PIN_PTT);
    PORTD &= ~(1 << LED_PTT);
}

uint8_t ptt_peek(void) {
    return DDRC & (1 << PIN_PTT);
}

void qso_none(void) {
    // Reset the QSO status, we do NOT shut the PTT here, as we might be in a
    // CID transmission
    status.qso_status = QSO_NONE;
}

// Kill off active QSO and activate the KILL timer logic
void qso_kill(void) {
    ptt_shut();
    status.qso_status = QSO_KILL;
    status.qso_timer  = 0;
    status.kill_timer = 0;
}

// Open up QSO and activate the qso timer
void qso_open(void) {
    // Open PTT
     ptt_open();
}

// Clean QSO shutdown, after hang timer expires
void qso_shut(void) {
    // Shut PTT
    PORTC &= ~(1 << PIN_PTT);
    // Reset kill and QSO timers
    status.qso_timer = 0;
    status.kill_timer = 0;
}

// Get the VOX status
uint8_t vox_peek(void) {
    return DDRC & (1 << PIN_VOX);
}

void sleep(uint8_t ms) {
    while (ms) {
        _delay_ms(1); // tick tack
        --ms;
    }
}

void setup(void) {
    /* Setup all pins as output, except for PC0 and PC1 */
    DDRC  = 0b1100;
    DDRD  = 0b1111;
    PORTC = 0b0000;
    PORTD = 0b0000;

    /* Setup status flags */
    status.qso_status = QSO_NONE;
    status.cid_timer = 0;
    status.qso_timer = 0;
    status.hang_timer = 0;
    status.kill_timer = 0;

    /* Setup a hardware interrupt for the caller ID broadcast. */
    // 1 second on 8MHz
    ICR1 = 31249;
    // Mode 12, CTC on ICR1
    TCCR1B |= (1 << WGM13)| (1 << WGM12);
    // Interrupt on compare
    TIMSK |= (1 << TICIE1);
    // Set prescaler to 256
    TCCR1B |= (1 << CS12);
    // Enable interrupts
    sei();

    /* Done initializing, set LED_PWR pin to high */
    PORTC |= (1 << LED_PWR);
}

int main(void) {
    setup();

    while (1) {
        // Sample the VOX pin every second
        if (vox_peek()) {
            // We have VOX data, open up QSO if not in QSO already
            switch (status.qso_status) {
                case QSO_NONE:
                    qso_open();
                    break;
            }
        }
        _delay_ms(1);
    }

    return 0;
}
