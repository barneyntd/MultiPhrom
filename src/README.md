Building MultiPhrom
===================

This folder contains the source code for MultiPhrom's firmware. In order to build it, you will need:
1.  avr-gcc
2.  avr-libc   (if you use the atmega168pb mcu, you may need to edit some of the avr-libc files)
4.  avrdude
5.  a suitable programmer.
   

Before building the firmware, you will probably need to edit the Makefile to suit your setup:
1.  You must set AVRDUDE_PROGRAMMER and AVRDUDE_PORT to whatever programmer you are using. See the avrdude documentation for details.
2.  The default microcontroller is an atmega168pb, but it should work with any atmega168 or atmega328 variant. Set MCU accordingly.

The board has a solder jumper marked "program". In order to protect the 3.3v memory chip from the 5v programming level, it is
advisable to desolder this jumper before programming, and resolder it after.

To build the firmware and program the mcu, connect up the programmer and execute the commands:
1.  `make setup`  (this is not needed if the mcu has been used before)
2.  `make clean`
3.  `make all`
4.  If you want to program several boards with the same firmware, you can use `make program`

If you get lots of errors about undefined things, you may need to edit the file `include/avr/io.h` in your avr-libc installation:
1.  After the line `#  include <avr/iom328p.h>` add the lines
     ```
    #elif defined (__AVR_ATmega328PB__)
    #  include <avr/iom328pb.h>
    ```
2.  After the line `#  include <avr/iom168pa.h>` add the lines
     ```
    #elif defined (__AVR_ATmega168PB__)
    #  include <avr/iom168pb.h>
    ```
3.  Make sure the files `include/avr/iom328pb.h` and `include/avr/iom168pb.h` exist
