//
//  flashrom.c
//  MultiPhrom
//
//  Created by Barney Hilken on 20/03/2020.
//  Copyright © 2020 Barney Hilken. This file is free for noncommercial use: see licence for details.
//

#include "hardwaremap.h"
#include "flashrom.h"


// W25X10BV/20BV/40BV SPI commands

#define FlashWriteEnable                 0x06
#define FlashWriteDisable                0x04
#define FlashReadStatusRegister          0x05
#define FlashWriteStatusRegister         0x01
#define FlashReadData                    0x03
#define FlashFastRead                    0x0B
#define FlashFastReadDualOutput          0x3B
#define FlashFastReadDualIO              0xBB
#define FlashPageProgram                 0x02
#define FlashSectorErase4K               0x20
#define FlashBlockErase32K               0x52
#define FlashBlockErase64KB              0xD8
#define FlashChipErase                   0xC7
#define FlashPowerdown                   0xB9
#define FlashReleasePowerdown            0xAB
#define FlashDeviceID                    0xAB
#define FlashManufacturerDeviceID        0x90
#define FlashManufacturerDeviceIDDualIO  0x92
#define FlashJEDECID                     0x9F
#define FlashReadUniqueID                0x4B

#define FlashStatusBusy                  (1 << 0)
#define FlashStatusWriteEnableLatch      (1 << 1)
#define FlashStatusBlockProtect          (0xF << 2)
#define FlashStatusStatusProtect         (1 << 7)


void flashSetupSPI (void)
{
    //  disable SD-card & flash chip (just in case)
    SET_CARD(1);
    SET_CHIP(1);
    
    //  no interrupt, enable SPI, MSB first, SPI Master, SPI mode 0, 8MHz
    SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR0);
    SPSR = (1 << SPI2X);
}

uint8_t shiftSPIbyte(uint8_t b)
{
    // start shifting byte
    SPDR = b;
    // wait for byte to be shifted out
    while(!(SPSR & (1 << SPIF)));
    // return byte shifted in
    return SPDR;
}



void flashReadAddress(uint32_t address)
{
    // enable flash chip CS
    SET_CHIP(0);

    shiftSPIbyte (FlashReadData);
    shiftSPIbyte ((address >> 16) & 0xFF);
    shiftSPIbyte ((address >> 8) & 0xFF);
    shiftSPIbyte (address & 0xFF);
}

uint8_t flashReadByte (void)
{
    return shiftSPIbyte(0);
}

void flashEndRead (void)
{
    // disable flash chip CS
    SET_CHIP(1);
}

void setWriteEnable (void)
{
    uint8_t status;
    
    do
    {
        // set write enable
        SET_CHIP(0);
        shiftSPIbyte (FlashWriteEnable);
        SET_CHIP(1);
        
        // check write enable
        SET_CHIP(0);
        shiftSPIbyte (FlashReadStatusRegister);
        status = shiftSPIbyte(0);
        SET_CHIP(1);
    }
    while (!(status & FlashStatusWriteEnableLatch));
}

void flashEraseSlot (uint8_t slot)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        setWriteEnable();
        
        // erase 4K block
        SET_CHIP(0);
        shiftSPIbyte (FlashSectorErase4K);
        shiftSPIbyte (slot >> 2);
        shiftSPIbyte ((slot << 6) | (i << 4));
        shiftSPIbyte (0);
        SET_CHIP(1);
        
        // read busy state
        SET_CHIP(0);
        shiftSPIbyte (FlashReadStatusRegister);
        // wait for erase to complete
        while (shiftSPIbyte(0) & FlashStatusBusy);
        SET_CHIP(1);
    }
}

void flashWritePage (uint16_t page, uint8_t data[256])
{
    setWriteEnable();
    
    // write data
    SET_CHIP(0);
    shiftSPIbyte (FlashPageProgram);
    shiftSPIbyte (page >> 8);
    shiftSPIbyte (page & 0xFF);
    shiftSPIbyte (0);
    for (uint16_t i = 0; i < 256; i++)
        shiftSPIbyte (data[i]);
    SET_CHIP(1);

    // read busy state
    SET_CHIP(0);
    shiftSPIbyte (FlashReadStatusRegister);
    // wait for erase to complete
    while (shiftSPIbyte(0) & FlashStatusBusy);
    SET_CHIP(1);
}


