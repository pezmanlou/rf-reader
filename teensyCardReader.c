/**
* Teensy Interface for Generic Card Reader
* Louis Bodnar
*
* How to wire this baby up:
*
* Teensy     Card Reader
* -----      -----------
*  F0      Track 1 Clock
*  F1      Track 1 Data
*  F4      Track 2 Clock
*  F5      Track 2 Data
*  F6      Track 3 Clock
*  F7      Track 3 Data
*  B6      Card Present
*
*/


#include <avr/io.h>
//#include <avr/pgmspace.h>
//#include <util/delay.h>
#include "usb_keyboard.h"


#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))



int main(void)
{
    uint8_t TRACK_1[100];
    uint8_t TRACK_2[100];
    uint8_t TRACK_3[100];

    int PLACE_1;
    int PLACE_2;
    int PLACE_3;

    uint8_t NewByte;
    uint8_t OldByte;

    // Set pins to input mode
    DDRF = 0;
    DDRB = 0;

    // Set for 16 MHz clock
    CPU_PRESCALE(0);


    // Initialize USB
    usb_init();

    // Wait for USB to finish initializing
    while (!usb_configured());

    while (1)
    {

        if (~PINB & 32) // 32 masks bit 6
        {

            NewByte = 0;
            OldByte = 0;

            PLACE_1 = 0;
            PLACE_2 = 0;
            PLACE_3 = 0;

            while (~PINB & 32 && PLACE_1 < 750 && PLACE_2 < 750 && PLACE_3 < 750)
            {
                OldByte = NewByte;
                NewByte = PINF;


                // Track 1 read
                if (((OldByte ^ 1) & NewByte) & 1)
                {
                    if ((~NewByte >> 1) & 1)
                    {
                        TRACK_1[PLACE_1 >> 3] |= (1 << (PLACE_1 & 7));
                    }else
                    {
                        TRACK_1[PLACE_1 >> 3] &= ~(1 << (PLACE_1 & 7)); 
                    }
                    PLACE_1 += 1;
                }

                // Track 2 read
                if (((OldByte ^ 16) & NewByte) & 16)
                {
                    if ((~NewByte >> 5) & 1)
                    {
                        TRACK_2[PLACE_2 >> 3] |= (1 << (PLACE_2 & 7));
                    }else
                    {
                        TRACK_2[PLACE_2 >> 3] &= ~(1 << (PLACE_2 & 7)); 
                    }
                    PLACE_2 += 1;
                }

                // Track 3 read
                if (((OldByte ^ 64) & NewByte) & 64)
                {
                    if ((~NewByte >> 7) & 1)
                    {
                        TRACK_3[PLACE_3 >> 3] |= (1 << (PLACE_3 & 7));
                    }else
                    {
                        TRACK_3[PLACE_3 >> 3] &= ~(1 << (PLACE_3 & 7)); 
                    }
                    PLACE_3 += 1;
                }
            }

            // Track 1 output


            for (int i = 0; i < PLACE_1; i++)
            {
                if (i > 750)
                {
                    usb_keyboard_press(KEY_ENTER, 0);
                    usb_keyboard_press(KEY_E, 0);
                    usb_keyboard_press(KEY_ENTER, 0);
                }
                if(TRACK_1[i >> 3] & (1<<(i%8)))
                {
                    usb_keyboard_press(KEY_1, 0);
                }else
                {
                    usb_keyboard_press(KEY_0, 0);
                }
            }

            // Track 2 output
            usb_keyboard_press(KEY_TAB, 0);
            for (int i = 0; i < PLACE_2; i++)
            {
                if (i > 750)
                {
                    usb_keyboard_press(KEY_ENTER, 0);
                    usb_keyboard_press(KEY_E, 0);
                    usb_keyboard_press(KEY_ENTER, 0);
                }
                if(TRACK_2[i >> 3] & (1<<(i%8)))
                {
                    usb_keyboard_press(KEY_1, 0);
                }else
                {
                    usb_keyboard_press(KEY_0, 0);
                }
            }

            // Track 3 output
            usb_keyboard_press(KEY_TAB, 0);
            for (int i = 0; i < PLACE_3; i++)
            {
                if (i > 750)
                {
                    usb_keyboard_press(KEY_ENTER, 0);
                    usb_keyboard_press(KEY_E, 0);
                    usb_keyboard_press(KEY_ENTER, 0);
                }
                if(TRACK_3[i >> 3] & (1<<(i%8)))
                {
                    usb_keyboard_press(KEY_1, 0);
                }else
                {
                    usb_keyboard_press(KEY_0, 0);
                }
            }
            usb_keyboard_press(KEY_ENTER, 0);
        }
    }
}
