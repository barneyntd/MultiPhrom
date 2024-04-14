Phroms
======

This folder contains sample phroms in the format used by MultiPhrom.
Put them on a micro-SD card, plug it into Multiphrom, and wait for the LED to go out.

> *I do not own the copyright on some of these files!
I have included them here for testing, but they should not be included in distributions.
If you are the copyright holder, please let me know what you would like me to do about your data*

1. **PH\_EFR** This is VM71003A, the speaking clock rom.
2. **PH\_F0R** This is WM61002, the phrom included in the US version if the model B.
3. **PH\_FEN** This is a phrom created using PHSAVE, containing BASIC files for the \*ROM filing system.
4. **PH\_FFN** This is CM62024, the Kenneth Kendal phrom included in the BBC speech upgrade.

The filename controls how MultiPhrom treats the files:
* Any filename not of the form "PH\_hhr" where hh is a hex value between C0 and FF,
and r is N or R, is ignored.
* The two characters hh are the slot number in which the phrom will be installed.
Phrom data is not slot specific, so phroms can be installed in any slot.
* The character r controls whether the data will be bit-reversed. Rom dumps are usually
bit reversed; home created data is usually not.

