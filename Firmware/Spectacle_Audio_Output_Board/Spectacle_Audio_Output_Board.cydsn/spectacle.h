/****************************************************************************
spectacle.h
Definitions for various Spectacle-related constants
Mike Hord @ SparkFun Electronics
24 Jan 2017
https://github.com/sparkfun/Spectacle_Audio_Output_Board

This file contains a number of constants regarding I2C addresses and board ID.
There is a spectacle.h file in every Spectacle hardware project.

Development environment specifics:
Developed in PSoC Creator 4.0
Board V10

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!
****************************************************************************/
    
#ifndef __spectacle_h__
#define __spectacle_h__

#define BOARD_ID        0x01 // unique identifier for this type of spectacle 
                             // board. The MSb signifies whether this is an
                             // input type board (1) or output type (0)
#define BOARD_ID_REG    0xff // I2C register holding the board ID.
#define CONFIGURED_REG  0xfd // register set by director when it has finished
                             // setting the I2C address for this board into
                             // the appropriate register, which is...
#define I2C_ADDR_REG    0xfe // ...this register.
#endif

