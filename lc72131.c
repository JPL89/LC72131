// @File     		
// @Author		JOSIMAR PEREIRA LEITE
// @country		Brazil
// @Date		01/05/2025
//
//
// Copyright (C) 2021  JOSIMAR PEREIRA LEITE
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//

#include "utils.h"
#include "ks0066.h"

#define LC72131_CE_TRIS TRISCbits.RC7
#define LC72131_DI_TRIS TRISCbits.RC6
#define LC72131_CL_TRIS TRISCbits.RC5
#define LC72131_DO_TRIS TRISCbits.RC4

#define LC72131_CE PORTCbits.RC7
#define LC72131_DI PORTCbits.RC6
#define LC72131_CL PORTCbits.RC5
#define LC72131_DO PORTCbits.RC4

#define LC72131_IN1 ((unsigned char)(0x28))
#define LC72131_IN2 ((unsigned char)(0x29))
#define LC72131_OUT ((unsigned char)(0x2A))

//#define LC72131_IN1 ((unsigned char)(0x82))
//#define LC72131_IN2 ((unsigned char)(0x92))
//#define LC72131_OUT ((unsigned char)(0xA2))

#define LC72131_IF 107000000

unsigned long LC72131_RF = 102900000;

void LC72131_write_address(unsigned char data)
{
	for(int i = 0; i < 8; i++)
	{
        if( (data >> i) & 1 ) 
            LC72131_DI = 1; 
        else 
			LC72131_DI = 0; 
		
		LC72131_CL = 1; 
		__asm__ __volatile__("nop");      
        LC72131_CL = 0;
        __asm__ __volatile__("nop");
	}
}

void LC72131_write_data(unsigned long data)
{
	for(int i = 0; i < 24; i++)
	{
        if( (data >> i) & 1 ) 
            LC72131_DI = 1; 
        else 
			LC72131_DI = 0; 
		
		LC72131_CL = 1; 
		__asm__ __volatile__("nop");      
        LC72131_CL = 0;
        __asm__ __volatile__("nop");
	}
}

void LC72131_send_data(unsigned char address, unsigned long data)
{
    LC72131_CE = 0;
    LC72131_write_address( LC72131_IN1 );
    LC72131_CE = 1;
    
    LC72131_write_data( data );
    
    LC72131_CE = 0;
    __asm__ __volatile__("nop");
    LC72131_CE = 1;
}

void LC72131_Init(void)
{
    LC72131_CE_TRIS = 0;
    LC72131_DI_TRIS = 0;
    LC72131_CL_TRIS = 0;
    LC72131_DO_TRIS = 1;
    
    LC72131_CE = 1;
    LC72131_CL = 1;
}

int main(void) 
{    
    // SYSTEM
    Pic18f2520_init();

    // WAIT
    __delay_ms(2000);
    
    // LC72131
    LC72131_Init();
    
    // 102.9 Mhz
    LC72131_RF = 102900000;
    
    LC72131_RF = (LC72131_RF + LC72131_IF);
    
    // R0 = 1, R1 = 1 = 25KHz
    LC72131_RF = ( LC72131_RF / 25000) / 2;
    
    LC72131_send_data( LC72131_OUT, 0x00000800 );
    
    // IFS = 1, DLC = 1, 
    LC72131_send_data( LC72131_IN2, 0x00180000 );
    
    // FREQUENCY, DVS = 1, R0 = 1, R1 = 1
    // XS 0 = 4.5
    // XS 1 = 7.2
    LC72131_send_data( LC72131_IN1, LC72131_RF | 0x003A0000 );
    
    while (1) 
    {
        
    }
}
