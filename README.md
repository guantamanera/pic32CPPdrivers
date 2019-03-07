# pic32CPPdrivers
C++ uart driver for pic32

C++ based peripheral drivers for the PIC32 microprocessor. Baremetal Examples

For the example a PIC32MZ2048EFM100 is being used. This CPU is running at 200MHz. If you want to use this code in your project, and are using a different CPU. Then you need to edit line 28 of serial.h and enter your CPU clock speed.

For the PIC32MZ the UART peripheral address starts at 0xBF822000. Check the datasheet of your microprocessor,chapter "universal asynchronous Receiver transmitter" under control registers Edit line 49 of serial.h and enter your microprocessors UART1 address. For PIC32MZ family is 0xBF822000 For PIC32MX1XX/2XX Family is 0xBF806000
