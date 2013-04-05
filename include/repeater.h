#ifndef __REPEATER_H__
#define __REPEATER_H__ 1

#define PIN_VOX     PC0     /* VOX detection input */
#define PIN_TEST    PC1     /* Test mode jumper input */
#define PIN_CID     PC2     /* CID LF generator output */
#define PIN_PTT     PC3     /* PTT toggle output */

#define LED_VOX     PD0
#define LED_CID     PD1
#define LED_PTT     PD2
#define LED_PWR     PD3

// CID interval in seconds
#define CID_TIMER       300
// Hang timer in seconds
#define HANG_TIMER      10
// Kill timer in seconds
#define KILL_TIMER      150
// Kill sleep in seconds
#define KILL_SLEEP      15

#define CID_NONE        0x00    // No CID broadcast
#define CID_OPEN        0x01    // In CID broadcast

#define QSO_NONE        0x00    // No QSO
#define QSO_OPEN        0x01    // In QSO
#define QSO_HANG        0x02    // In QSO, waiting for hang timer
#define QSO_KILL        0x10    // No QSO, waiting for kill timer

typedef struct {
    uint8_t     cid_status;
    uint8_t     qso_status;
    uint8_t     cid_timer;
    uint8_t     qso_timer;      // Amount of seconds in QSO,    if ptt_status
    uint8_t     hang_timer;     // Amount of seconds after QSO, if ptt_status
    uint8_t     kill_timer;     // Amount of seconds in kill,   if ptt_status
} status_t;

// Shared status declaration
static status_t status;

// Forward declarations
void    ptt_open(void);
void    ptt_shut(void);
uint8_t ptt_peek(void);
void    qso_none(void);
void    qso_kill(void);
void    qso_open(void);
void    qso_shut(void);

#endif
