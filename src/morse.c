#include <avr/interrupt.h>
#include <util/delay.h>
#include "repeater.h"
#include "morse.h"

void nop(void) {
    uint8_t l = DIT_LEN;
    for (; l; --l) {
        _delay_ms(1);
    }
}

void dit(void) {
    uint8_t l = DIT_LEN;
    PORTD |= (1 << LED_CID);
    for (; l; --l) {
        DDRC |= _BV(PIN_CID);
        _delay_ms(1);
        DDRC &= ~_BV(PIN_CID);
        _delay_ms(1);
    }
    PORTD &= ~(1 << LED_CID);
    nop();
}

void dah(void) {
    uint8_t l = DAH_LEN;
    PORTD |= (1 << LED_CID);
    for (; l; --l) {
        DDRC |= _BV(PIN_CID);
        _delay_ms(1);
        DDRC &= ~_BV(PIN_CID);
        _delay_ms(1);
    }
    PORTD &= ~(1 << LED_CID);
    nop();
}

void pause(void) {
    uint8_t l = PAUSE_LEN;
    for (; l; --l) {
        _delay_ms(1);
    }
}

void cid_open(void) {
    // Open up PTT if required
    ptt_open();

    if (cid_peek()) {
        cid_test();
    } else {
        cid_send();
    }

    // Cleanly shut the CID
    cid_shut();
}

void cid_shut(void) {
    switch (status.qso_status) {
        case QSO_NONE:
            // If we are not in QSO, pretend we are in QSO_HANG status, this
            // saves us from flapping
            status.qso_status = QSO_HANG;
            status.hang_timer = 0;
            break;
        case QSO_KILL:
            // If we are in QSO KILL status, shut the PTT
            ptt_shut();
            break;
    }
}

void cid_test(void) {
    /* t */ dah();
    pause();
    /* e */ dit();
    pause();
    /* s */ dit(); dit(); dit();
    pause();
    /* t */ dah();
    pause();
}

void cid_send(void) {
    /* p */ dit(); dah(); dah(); dit();
    pause();
    /* d */ dah(); dit(); dit();
    pause();
    /* 0 */ dah(); dah(); dah(); dah(); dah();
    pause();
    /* m */ dah(); dah();
    pause();
    /* z */ dah(); dah(); dit(); dit();
    pause();
    /* / */ dah(); dit(); dit(); dah(); dit();
    pause();
    /* m */ dah(); dah();
    pause();
}
