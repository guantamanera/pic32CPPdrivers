/*------------------------------------------------------------------------------/
/  C++ UART Driver PIC32  aka PIC32C++Driver  r0.001                            /
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



#include "serial.h"


/*Constructor takes an int as argument 0 or 1
 *If argument 0, the UART1 module gets turned on
 *if argument 1, the UART2 module gets configure
 * make sure to call turnON() to actually turn it on
 * you first make sure to set the baudRate. 
 * There is no safety checks to give you compile errors.
 * This class is dangerous since you can call
 * serial serial(20)
 * I am not aware of a PIC32 with 20 uart units. This will
 * compile but it will not work and you might end up debugging for hours.
 * 
 * Example:
 *
 * serial serial1(0);
 * serial1.baudRate(57600);
 * serial1.turnON();
 * serial1.transmit("send some text");
 */
    serial::serial(int in){
        n=in;//serial instance 
        pointerSet();//set up the pointers
    }

void serial::turnON(){   
    //Turns on the serial port
    //line below sets register 2000
    //0000 0000 0000 0000 1000 0000 0000 0000
    *mode|=1<<15;
    
   
    
    //if conditons below chose the port to use for transmit
    //see I/O section of the datasheet 
    //These were chosen because Curisiosity MZ EF has 2 at J5 and J10
    if(n==0){
     uint32_t &txPort=*reinterpret_cast<uint32_t*>(0xBF8015FC);
     readRegister=&txPort;
         txPort=0b0001;
      //just for testing we TX instance 0 will transmit at 2 different ports at the same time
      //I'll clean this later and make it selectable
         
        uint32_t &txPort2=*reinterpret_cast<uint32_t*>(0xBF80154C);
         txPort2=0b0001;
         
         uint32_t &rxPort=*reinterpret_cast<uint32_t*>(0xBF801468);
         rxPort=0b0011;
         
         
         uint32_t &input1=*reinterpret_cast<uint32_t*>(0xBF860310);
         input1=1<<10;
       //  uint32_t &output=*reinterpret_cast<uint32_t*>(0xBF801468);
        // rxPort=0b0011;
         
    }
    if(n==1){
        uint32_t &txPort=*reinterpret_cast<uint32_t*>(0xBF801588);
         txPort=0b0010;
    }
     if(n==2){
        uint32_t &txPort=*reinterpret_cast<uint32_t*>(0xBF801684);
         txPort=0b0001; //this is RG1 port
    }
}
/* Function sets the baudrate of the serial port
 * once the object has been created in main this can be changed
 * object creation:
 * serial serial1;
 * settings the baudrate:  
 * serial1.badRate(115200);
 */
void serial::baudRate(int rate){
    *brg=pbClk / (16 * rate) - 1;
}


/*Function sets up the registers pointing to the proper memory locations.
 Each peripheral repeats every 0x200 by using n we can re-use to create
 more instances of the UART
 */
void serial::pointerSet(){
   
    //block below sets the pointers to the appropiate registers
  //  uint32_t &MODE=*reinterpret_cast<uint32_t*>(baseAddress +n*512); //peripherall repeats at every 0x200
   // uint32_t &STATUS=*reinterpret_cast<uint32_t*>((baseAddress + n*512+16)); //peripherall repeats at every 0x200
  //  uint32_t &TXREG=*reinterpret_cast<uint32_t*>((baseAddress + n*512)+32); //peripherall repeats at every 0x200
  //  uint32_t &RXREG=*reinterpret_cast<uint32_t*>((baseAddress + n*512)+48); //peripherall repeats at every 0x200
   // uint32_t &BRG=*reinterpret_cast<uint32_t*>(baseAddress+n*512+64); //peripherall repeats at every 0x200
    //end setting pointers
    
    //have the dev_regs point at the actual memory
    mode=reinterpret_cast<uint32_t*>(baseAddress +n*512);
    status=reinterpret_cast<uint32_t*>((baseAddress + n*512+16));
    txreg=reinterpret_cast<uint32_t*>((baseAddress + n*512)+32);
    rxreg=reinterpret_cast<uint32_t*>((baseAddress + n*512)+48);
    brg=reinterpret_cast<uint32_t*>(baseAddress+n*512+64);
}

/*Transmits a string. Takes a string as argument
 * if you want to send numbers you might want to convert them first to 
 * a string value
 */
void serial::transmit(string message){
   //bit 10 enables TX pin
      *status |=1<<10;// enable tx
   
   
    //iterate until the buffer has finished transmitting
    for(int i=0;i<message.length();i++)
    {
        while(*status&(1<<9));//checks status of bit 9
        *txreg=message[i];
    }
}


char serial::ReadChar()
{
  *status |=1<<12;// enable rx

   // while(!(*status&(1<<0)));             // Wait for information to be received
     while(!((*status>>0) & 1U));           //same as line above just different way
     
    // char character=*rxreg;
   //  *status &= ~(1UL << 1); //clear buffer;
    // return character;
     return *rxreg;                 //byte full transmit
}

string serial::ReadString()
{  
    string message="";//empty string
    char character;  //char for testing end of the line and adding to string
    while(character!='\r'){
        
        character=ReadChar();
        string transfer(1,character);
        message+=transfer;
    }
    return message;
}

serial::~serial() {
      
}
