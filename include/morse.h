#ifndef __MORSE_H__
#define __MORSE_H__ 1

/* Lenghts are in milliseconds */
#define DIT_LEN     (uint8_t) 100
#define DAH_LEN     (uint8_t) 300
#define PAUSE_LEN   (uint8_t) 400 /* Because the previous char already ends
                                     with a DIT_LEN delay */

void        nop(void);
void        dit(void);
void        dah(void);
void        pause(void);

void        cid_open(void);
void        cid_shut(void);
uint8_t     cid_peek(void);
void        cid_test(void);
void        cid_send(void);

#endif
