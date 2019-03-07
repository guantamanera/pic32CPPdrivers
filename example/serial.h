/*------------------------------------------------------------------------------/
/  C++ UART Driver for Curiosity PIC32  aka PIC32C++Driver  r0.001              /
/  Creator Miguel                                                               /
/  Created on February 10, 2019, 11:51 PM                                       /
/-------------------------------------------------------------------------------/
/  This file is part of C++ UART driver for Curiosity PIC32MZ EF. PIC32C++Driver
/
/  PIC32C++Driver is free software: you can redistribute it and/or modify
/  it under the terms of the GNU General Public License as published by
/  the Free Software Foundation, either version 3 of the License, or
/  (at your option) any later version.
/
/  PIC32C++Driver is distributed in the hope that it will be useful,
/  but WITHOUT ANY WARRANTY; without even the implied warranty of
/  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/  GNU General Public License for more details.
/
/  You should have received a copy of the GNU General Public License
/  along with PIC32UARTC++.  If not, see <https://www.gnu.org/licenses/>..
/----------------------------------------------------------------------------*/

#include <cstdlib>
#include <string>
#include <cstdint> //required to do typedef uint32_t

#ifndef SERIAL_H
#define	SERIAL_H
#define SYS_FREQ 200000000 // Running at 200MHz

using namespace std;
//by having typedef we don't have to type volatile everytime we set a register
typedef uint32_t volatile dev_reg;
class serial {
    
public:

    serial(int n);
    void turnON();
    void baudRate(int rate);
    void transmit(string message);
    void pointerSet();
    char ReadChar(void);
   string ReadString();
    virtual ~serial();
    
private:
    //pic32MZ2048EFM100, the UART starts at 0xBF822000, and repeats every 0x200
    //for every other unit of the UART
    //UART1 is at 0xBF822000
    //UART2 is at 0xBF822200 see pointerSet() for implementation
    static const unsigned int baseAddress =(0xBF822000);
    int n;
    int pbClk = SYS_FREQ / 2; //peripheral clock half of system clock
   
    //volatile device registers pointers
    dev_reg *mode;
    dev_reg *status;
    dev_reg *txreg;
    dev_reg *rxreg;
    dev_reg *brg;
    dev_reg *readRegister;
    dev_reg *readRegister2;
    dev_reg *temp;
   
       
};

#endif	/* SERIAL_H */
