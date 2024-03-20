//
//  sdcard.c
//  MultiPhrom
//
//  Created by Barney Hilken on 21/01/2021.
//  Copyright © 2021 Barney Hilken. This file is free for noncommercial use: see licence for details.
//

#include "hardwaremap.h"
#include "sdcard.h"
#include "flashrom.h"
#include "sd_raw.h"
#include "partition.h"
#include "fat.h"


uint8_t reverseBits (uint8_t b)
{
    b = ((b & 0x55) << 1) | ((b >> 1) & 0x55);
    b = ((b & 0x33) << 2) | ((b >> 2) & 0x33);
    b = (b << 4) | (b >> 4);
    return b;
}

uint8_t buffer[256];
void writeSlot (uint8_t slot, uint8_t reverse, struct fat_file_struct* phromFile)
{
    for (uint16_t page = 0; page < 64; page++)
    {
        if (fat_read_file(phromFile, buffer, 256) < 1)
            return;
        if (reverse)
        {
            for (uint16_t i = 0; i < 256; i++)
                buffer[i] = reverseBits(buffer[i]);
        }
        flashWritePage (page | (slot << 6), buffer);
    }
    
}

void readSDCard (void)
{
    if(!sd_raw_init())
        goto fsError;
    
    struct partition_struct* partition = partition_open(sd_raw_read, sd_raw_read_interval, 0, 0, 0);
    if (!partition)
        goto fsError;
    
    struct fat_fs_struct* cardFS = fat_open(partition);
    if (!cardFS)
    {
        partition_close(partition);
        goto fsError;
    }
    
    
    SET_LED(1);

    char filePath[] = PHROM_PATH;
    
    for (uint8_t i = 0; i < 0x40; i++)
    {
        filePath[PHROM_PATH_NUM] = (i >> 4) + 'C';
        filePath[PHROM_PATH_NUM+1] = (i & 0xF) <= 9 ? '0' + (i & 0xF) : 'A' - 10 + (i & 0xF);
        
        for (uint8_t j = 0; j < 2; j++)
        {
            filePath[PHROM_PATH_REV] = j ? 'R' : 'N';
            
            struct fat_dir_entry_struct dir_entry;
            
            if (!fat_get_dir_entry_of_path(cardFS, filePath, &dir_entry))
                continue;

            struct fat_file_struct* phromFile = fat_open_file(cardFS, &dir_entry);
            if (!phromFile)
                continue;

            flashEraseSlot (i);
            writeSlot (i, j, phromFile);
            
            fat_close_file(phromFile);
            break;
        }
    }

    SET_LED(0);

    return;
    
fsError:
    return;

}

