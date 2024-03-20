//
//  main.c
//  MultiPhrom
//
//  Created by Barney Hilken on 20/03/2020.
//  Copyright © 2020-2024 Barney Hilken. All rights reserved.
//



#include "hardwaremap.h"
#include "flashrom.h"
#include "sdcard.h"
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>


//  interrupt to detect SD card

ISR(DTCT_INT_vect)
{
    // if card inserted, restart
    if (DTCT)
    {
        wdt_enable(WDTO_30MS);
        while(TRUE);
    }
}

void setupCardDetect (void)
{
    ATOMIC_BLOCK (ATOMIC_FORCEON)
    {
        DTCT_INT_enable;
    }
}


uint8_t ignoreSlot[16];

void read_rom_data (uint32_t address)
{
    uint8_t emulating = !ignoreSlot[(address >> 14) & 0xF];
    
    // wait for dummy read command, exit on address command
    while (!M0)
        if (M1) goto exit;
    while (M0)
        if (M1) goto exit;

    // set read address
    flashReadAddress(address);
    
	// wait for actual read command, exit on address command
	while (!M0)
		if (M1) goto exit;

    // if emulating this rom, take control of DATA/ADDB
    if (emulating)
    	SET_DATA_OUTPUT;

    uint8_t byte;
    while (TRUE)
    {
        // get the next byte of flash data
        byte = flashReadByte ();
        
        for (uint8_t bitcount = 0; bitcount < 8; bitcount++)
        {
			uint8_t databit = ((byte & 0x80) != 0);
			byte <<= 1;
			
			// wait for end of read command, exit on address command
			while (M0)
				if (M1) goto exit;

            // if emulating this rom, set DATA/ADDB according to the data bit
            if (emulating)
                SET_DATA(databit);

            // wait for start of next read command, exit on address command
            while (!M0)
                if (M1) goto exit;
            
            // if emulating this rom, check DATA/ADDB for conflict
            if (emulating && ADD8 != databit)
			{
				SET_DATA_INPUT;
				SET_DATA(0);
				ignoreSlot[(address >> 14) & 0xF] = TRUE;
				emulating = FALSE;
			}
        }

        // increment address
        address = (address + 1) & 0xFFFFF;
        
        // if moving to a new slot, check whether to emulate
        if ((address & 0x3FFF) == 0)
        {
            emulating = !ignoreSlot[address >> 14];
            if (emulating)
            {
                SET_DATA_OUTPUT;
            }
            else
            {
                SET_DATA_INPUT;
                SET_DATA(0);
            }
        }
    }

exit:
    // end read byte sequence
    flashEndRead ();
    
    // set DATA back to input
    SET_DATA_INPUT;
    SET_DATA(0);
}

int main (void)
{
    // turn off unused MCU features
    UNUSED_OFF;
    MCUSR = 0;
    wdt_disable();
    
    // Set all speech rom pins to input and turn off weak pull-ups
    SET_M0_INPUT;
    SET_M0_NOPULLUP;

    SET_M1_INPUT;
    SET_M1_NOPULLUP;

    SET_CLK_INPUT;
    SET_CLK_NOPULLUP;

    SET_ADD_INPUT;
    SET_ADD_NOPULLUP;
    
    SET_DATA_INPUT;
    SET_DATA(0);
    
    //  set SPI pins correctly
    SET_MISO_INPUT;
    SET_MISO_NOPULLUP;

    SET_MOSI_OUTPUT;
    SET_SCK_OUTPUT;
    
    // deselect flash chip & SD card
    SET_CHIP_OUTPUT;
    SET_CHIP(1);

    SET_CARD_OUTPUT;
    SET_CARD(1);
    
    // set other pins
    SET_DTCT_INPUT;
    SET_DTCT_PULLUP;
    SET_LED_OUTPUT;
    SET_LED(0);

    if (DTCT)
        readSDCard ();
    
    flashSetupSPI ();
    setupCardDetect();
    
    while (TRUE)
    {
        // load new address
        uint32_t address;
    loop: address = 0;
        for (uint8_t shift = 0; shift < 20; shift += 4)
        {
            // wait for start of address command
            while (!M1)
                if (M0) goto loop;

            // wait for clock
            while (CLK);

            // read nibble
            address |= (uint32_t)ADD << shift;

            //wait for end of address command
            while (M1)
                if (M0) goto loop;
        }
        
        // read the data, as requested
        read_rom_data (address);
    }
}

