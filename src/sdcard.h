//
//  sdcard.h
//  MultiPhrom
//
//  Created by Barney Hilken on 21/01/2021.
//  Copyright © 2021 Barney Hilken. All rights reserved.
//

#ifndef sdcard_h
#define sdcard_h

// Root name for Phrom files on SD card
#define PHROM_PATH "Ph_XXR"
// Character to indicate slot number C0..FF
#define PHROM_PATH_NUM 3
// Character to indicate bit order N normal, R reverse
#define PHROM_PATH_REV 5


void readSDCard (void);

#endif /* sdcard_h */
