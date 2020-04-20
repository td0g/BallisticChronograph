# BallisticChronograph
Data repository for Arduino-based Ballistic Chronograph.  For the MK2, pleaes see [https://github.com/td0g/ballistic_chronograph_mk2/](https://github.com/td0g/ballistic_chronograph_mk2/)

## ballisticChronoBasic
A very simple ballistic chronograph program.  It reads out the speed over UART Serial to a PC.

You will need to select the BAUD rate (default: 9600), GATE_SEPARATION (default: 500 mm), TIMEOUT (default: 2000 milliseconds), GATE pins (default: A0 and A1), ACTIVE_LOW (default: active low), PULLUP (default: enabled), and LCD (default: enabled).

For the gates, please use the format shown in the code.  GATE_A_PIN and GATE_B_PIN should be PIN_x (eg. PIN_17 or PIN_A0).  GATE_A_PIN2 and GATE_B_PIN2 should be just the number or A-number (eg. 17 or A0).

If your chronograph is an active-low signal (0V is triggered), leave the ACTIVE_LOW alone.  If it is active-high (5V is triggered), comment it out (//ACTIVE_LOW).  If you're not sure, just try both.

If your chronograph needs the Arduino's pullup resistors, leave PULLUP alone.  Otherwise comment it out (//PULLUP).

If you have an LCD display connected, the code will automatically display the speed on the display.  PLEASE BE CAREFUL that the LCD_PINS aren't the same as the GATE_PINS.  If you aren't using an LCD, just comment it out (//LCD) to be safe.

## ballisticChronoLCD
//To Be Updated

## License

Documentation is licensed under a [Creative Commons Attribution 4.0 International License](https://creativecommons.org/licenses/by/4.0/)

Software is licensed under a [GNU GPL v3 License](https://www.gnu.org/licenses/gpl-3.0.txt)
