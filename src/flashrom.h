//
//  flashrom.h
//  MultiPhrom
//
//  Created by Barney Hilken on 20/03/2020.
//  Copyright © 2020 Barney Hilken. All rights reserved.
//

#ifndef flashrom_h
#define flashrom_h

void flashSetupSPI (void);
void flashReadAddress(uint32_t address);
uint8_t flashReadByte (void);
void flashEndRead (void);
void flashEraseSlot (uint8_t slot);
void flashWritePage (uint16_t page, uint8_t data[256]);

#endif /* flashrom_h */
