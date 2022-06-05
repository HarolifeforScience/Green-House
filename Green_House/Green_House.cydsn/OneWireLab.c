/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <project.h>
#include "OneWireLab.h"
#include "stdio.h"


#define H 480
#define I 70
#define J 410
#define B 64
#define D 10
#define E 10
#define TIME_SLOT 60
#define R_T 10
#define C 60
#define A 10



/* Variable */ 
float32 OneWireSum = 0.0;
int OneWireCnt = 0;
float resultTemp = 0.0;
int i;
int16_t temp16 ;
char scratchpad[10];
int temp_lsb1,temp_msb1;
int temp_lsb2,temp_msb2;


void OneWireStart(){
    /*One Wire */
    OwReset();
    /* As only single sensor is present in channel-Skip command */
    /* Skip Rom */
    OwWriteByte(0xCC);
    /*Write Scratch Pad command*/
    OwWriteByte(0x4E);
    /* Alarm registers are unused, Writing Random values*/
    /*TH*/
    OwWriteByte(0x55);
    /*TL*/
    OwWriteByte(0xA2);
    /* Config(resoluion)7F- 12 bit resolution */
    OwWriteByte(0x7F);
    
}

float OneWireRead_data(){
    
       OwReset();
       /*Skip address check command */
       OwWriteByte(0xCC); 
       /* Start Convert*/
       OwWriteByte(0x44);
       /* Wait until conversion completes */
       CyDelayUs(750);
    
       OwReset();
       
       /*Skip address check command */
       OwWriteByte(0xCC);
    
       /*Read Scratch Pad command*/
       OwWriteByte(0xBE);
    
        /*Copy the scratpad from slave*/
        for( i=0;i<9;i++)
        {
        scratchpad[i] = OwReadByte();
       
        }
        temp_msb1 = scratchpad[1] ; 
        temp_lsb1 = scratchpad[0] ;
        temp16 = (temp_msb1 << 8) | temp_lsb1 ; 
        
      //  if (temp16 >= 0x800) //temperture is negative 
       // {
            /* Tow's complement for reading negative temperatue */
           // temp16 = (~temp16)+1;
            //temp16 = ((-1)*temp16);
       // }

        /* read the temperature with fraction */
        resultTemp = temp16 / 16.0;
        /* Sum of the samples */
        OneWireSum += resultTemp;
        /* Number of samples */
        OneWireCnt++;
       
        return resultTemp;
        
}
int OwReset(void)
{
    
int presence;
    
OneWirePin_Write(0); //pull DQ line low

CyDelayUs(H); // leave it low for 480us

OneWirePin_Write(1); // allow line to return high

CyDelayUs(I); // wait for presence

presence = OneWirePin_Read(); // get presence signal

CyDelayUs(J); // wait for end of timeslot

return presence; // presence signal returned

} // 0=presence, 1 = no part

void OwWriteBit(int b)
{   
    
    /* Write is initiated from this step*/
    //OneWirePin_Write(0); 
    
    // Write '1' bit
    if(b==1)
    {
        // Releases the bus
       OneWirePin_Write(0);
       CyDelayUs(A);
       OneWirePin_Write(1);
       CyDelayUs(B);
    }
    // Write '0' bit
    else
    {
        
        OneWirePin_Write(0);
        CyDelayUs(C);
        OneWirePin_Write(1);
        CyDelayUs(D);
       
    }
     
    /* Minimum recovery time 1us between write steps is ncessary, 10us is used */
    //CyDelayUs(R_T);
}

int OwReadBit(void)
{
    int result;
    /* Initiate read by pulling line low */
    OneWirePin_Write(0);
    CyDelayUs(A);
    OneWirePin_Write(1);
    CyDelayUs(E);
    /* Data valid before 15us, should be sampled before that*/
    result = OneWirePin_Read();
    // Complete the time slot and 10us recovery
    CyDelayUs(50);
    // Sample the bit value from the slave
    return result;
}

//-----------------------------------------------------------------------------
// Write 1-Wire data byte
//
void OwWriteByte(int data)
{
        int loop;

        // Loop to write each bit in the byte, LS-bit first
        for (loop = 0; loop < 8; loop++)
        {
                OwWriteBit(data & 0x01);

                // shift the data byte for the next bit
                data >>= 1;
        }
}
//-----------------------------------------------------------------------------
// Read 1-Wire data byte and return it
//
int OwReadByte(void)
{
        int loop, result=0;

        for (loop = 0; loop < 8; loop++)
        {
                // shift the result to get it ready for the next bit
                result >>= 1;

                // if result is one, then set MS bit
                if (OwReadBit())
                        result |= 0x80;
        }
        return result;
}



/* [] END OF FILE */
