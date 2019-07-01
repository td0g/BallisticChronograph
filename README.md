# BallisticChronograph
Data used for Arduino-based Ballistic Chronograph

## ballisticChronoBasic
A very simple ballistic chronograph program.  It reads out the speed over UART Serial to a PC.
You will need to select the BAUD rate (default: 9600), GATE_SEPARATION (default: 500 mm), GATE pins (default: A0 and A1), timeout (default: 2000 milliseconds), and ACTIVE_LOW (default: active low).
For the gates, please use the format shown in the code.  GATE_A_PIN and GATE_B_PIN should be PIN_x (eg. PIN_17 or PIN_A0).  GATE_A_PIN2 and GATE_B_PIN2 should be just the number or A-number (eg. 17 or A0).
If your chronograph is an active-low signal (0V is triggered), leave the ACTIVE_LOW alone.  If it is active-high (5V is triggered), comment it out (//ACTIVE_LOW).  If you're not sure, just try both.

## ballisticChronoComplete
//To Be Updated

## _2016-12-15_BallisticChronograph_V3
Depricated, please do not use except for reference.
