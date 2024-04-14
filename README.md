Multiphrom
==========

Multiphrom is a small microcontroller based board designed to emulate serial speech ROMS (phroms) for the BBC microcomputer.
It differs from many similar devices in several ways:
1.  Rather than emulating just one phrom, Multiphrom emulates up to 64;
2.  The contents of the emulated phroms can be updated at any time by inserting a micro-SD card;
3.  Multiphrom attempts to detect other phroms and stops using contended addresses;
4.  Multiphrom plugs into the official cartridge slot in the keyboard (the ashtray), or onto the external rom connector, PL14.

#### Project folders
- **kicad** This holds the schematic and pcb design, as a kicad project;
- **src** This holds the source code, written in C, and a Makefile to build it;
- **docs** This holds documents explaining how to use multiphrom, and the BBC micro speech system;
- **BASIC** This holds several sample programs, written in BBC BASIC.
- **Phroms** This holds some sample phroms for testing.

#### Licence
This project is licensed under creative commons licence [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/).
This means that you are free to use it for any noncommercial purpose, provided you credit the copyright holder, and pass on the same conditions.
If you want to create a commercial product based on Multiphrom, you must obtain a seperate licence from the copyright holder.

The project includes the [sd-reader](http://www.roland-riegel.de/sd-reader/index.html) library by Roland Riegel, which is licenced under GNU LGPL v2.1.
