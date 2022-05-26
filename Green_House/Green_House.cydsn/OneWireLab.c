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


#define RESET_PULSE 480
#define PRESENCE_PULSE 70
#define END_SLOT 410
#define TIME_SLOT 60
#define RECOVERY_TIME 10
#define WRITE_0_SLOT 60
#define WRITE_1_SLOT 10
#define READ_INIT_DELAY 6
#define READ_VALID_DELAY 6

#define SKIP_ROM 0xCC
#define WRITE_SCRPAD 0x4E
#define RESOL_12BIT 0x7F
#define START_CONVERT 0x44
#define READ_SCRPAD 0xBE


/* Variable diclaration */ 
float32 OneWire1_sum = 0.0;
float32 OneWire2_sum = 0.0;
int OneWire1_cnt = 0;
int OneWire2_cnt = 0;
float temp1_res = 0.0;
float temp2_res = 0.0;
int i;
int16_t temp1x16 ;
int16_t temp2x16 ;
char scratchpad1[10];
char scratchpad2[10];
int temp_lsb1,temp_msb1;
int temp_lsb2,temp_msb2;


void OneWire_start(){
    /*One Wire */
    OW_Reset();
    /* As only single sensor is present in channel-Skip command */
    OW_Write_Byte(SKIP_ROM);
    /*Write Scratch Pad command*/
    OW_Write_Byte(WRITE_SCRPAD);
    
    /* Alarm registers are unused, Writing Random values*/
    /*TH*/
    OW_Write_Byte(0x55);
    /*TL*/
    OW_Write_Byte(0xA2);
    /* Config(resoluion)7F- 12 bit resolution */
    OW_Write_Byte(RESOL_12BIT);
    
/* For the 2nd 1-Wire */
    /*One Wire */
//    OW2_Reset();
//    /* As only single sensor is present in channel-Skip command */
//    OW2_Write_Byte(SKIP_ROM);
//    /*Write Scratch Pad command*/
//    OW2_Write_Byte(WRITE_SCRPAD);
//    
//    /* Alarm registers are unused, Writing Random values*/
//    /*TH*/
//    OW2_Write_Byte(0x55);
//    /*TL*/
//    OW2_Write_Byte(0xA2);
//    /* Config(resoluion)7F- 12 bit resolution */
//    OW2_Write_Byte(RESOL_12BIT);
 
    
}

float OneWire_read_data(){
       OW_Reset();
       /*Skip address check command */
       OW_Write_Byte(SKIP_ROM); 
       /* Start Convert*/
       OW_Write_Byte(START_CONVERT);
    
       /* Wait until conversion completes */
       CyDelayUs(110);
       OW_Reset();
       
       /*Skip address check command */
       OW_Write_Byte(SKIP_ROM);
       /*Read Scratch Pad command*/
       OW_Write_Byte(READ_SCRPAD);
    
/* For the 2nd 1-Wire */
//       OW2_Reset();
//       /*Skip address check command */
//       OW2_Write_Byte(SKIP_ROM); 
//       /* Start Convert*/
//       OW2_Write_Byte(START_CONVERT);
//    
//       /* Wait until conversion completes */
//       CyDelayUs(110);
//       OW2_Reset();
//       
//       /*Skip address check command */
//       OW2_Write_Byte(SKIP_ROM);
//       /*Read Scratch Pad command*/
//       OW2_Write_Byte(READ_SCRPAD);
//  
    
        /*Copy the scratpad from slave*/
        for( i=0;i<9;i++)
        {
        scratchpad1[i] = OW_Read_Byte();
       // scratchpad2[i] = OW2_Read_Byte();
        }
        temp_msb1 = scratchpad1[1] ; /*   S,   S,   S,   S,    S,  2^6,  2^5,  2^4 */
        temp_lsb1 = scratchpad1[0] ; /* 2^3, 2^2, 2^1, 2^0, 2^-1, 2^-2, 2^-3, 2^-4 */
        //temp_msb2 = scratchpad2[1] ; /*   S,   S,   S,   S,    S,  2^6,  2^5,  2^4 */
        //temp_lsb2 = scratchpad2[0] ; /* 2^3, 2^2, 2^1, 2^0, 2^-1, 2^-2, 2^-3, 2^-4 */
        temp1x16 = (temp_msb1 << 8) | temp_lsb1 ; /* make a int16_t from 2 bytes */
       // temp2x16 = (temp_msb2 << 8) | temp_lsb2 ; /* make a int16_t from 2 bytes */
        if (temp1x16 >= 0x800) //temperture is negative 
        {
            /* Tow's complement for reading negative temperatue */
            temp1x16 = (~temp1x16)+1;
            temp1x16 = ((-1)*temp1x16);
        }
//        if (temp2x16 >= 0x800) //temperture is negative 
//        {
//            /* Tow's complement for reading negative temperatue */
//            temp2x16 = (~temp2x16)+1;
//            temp2x16 = ((-1)*temp2x16);
//        }
        /* read the temperature with fraction */
        temp1_res = temp1x16 / 16.0;
        /* Sum of the samples */
        OneWire1_sum += temp1_res;
        /* Number of samples */
        OneWire1_cnt++;
        /* read the temperature with fraction */
        //temp2_res = temp2x16 / 16.0;
        /* Sum of the samples */
        //OneWire2_sum += temp2_res;
        /* Number of samples */
       // OneWire2_cnt++;   
        return OneWire1_sum ;
}
int OW_Reset(void)
{
int presence;
OneWirePin_Write(0); //pull DQ line low
CyDelayUs(RESET_PULSE); // leave it low for 480us
OneWirePin_Write(1); // allow line to return high
CyDelayUs(PRESENCE_PULSE); // wait for presence
presence = OneWirePin_Read(); // get presence signal
CyDelayUs(END_SLOT); // wait for end of timeslot
return presence; // presence signal returned
} // 0=presence, 1 = no part


/* For the 2nd 1-Wire */
/*int OW2_Reset(void)
{
int presence;
OneWire_Pin2_Write(0); //pull DQ line low
CyDelayUs(RESET_PULSE); // leave it low for 480us
OneWire_Pin2_Write(1); // allow line to return high
CyDelayUs(PRESENCE_PULSE); // wait for presence
presence = OneWire_Pin2_Read(); // get presence signal
CyDelayUs(END_SLOT); // wait for end of timeslot
return presence; // presence signal returned
} // 0=presence, 1 = no part
*/

void OW_Write_Bit(int bit)
{   
    
    /* Write is initiated from this step*/
    OneWirePin_Write(0); 
    
    // Write '0' bit
    if(bit==0)
    {
        /* Minimum 60us for logic_0 */
        CyDelayUs(WRITE_0_SLOT);
        // Releases the bus
       OneWirePin_Write(1);
    }
    // Write '1' bit
    else
    {
        /* Release to 1 before 15us for logic_1 */
        CyDelayUs(WRITE_1_SLOT);
        // Releases the bus
        OneWirePin_Write(1);
        // Complete the time slot
        CyDelayUs(50);
       
    }
     
    /* Minimum recovery time 1us between write steps is ncessary, 10us is used */
    CyDelayUs(RECOVERY_TIME);
}

///* For the 2nd 1-Wire */
//void OW2_Write_Bit(int bit)
//{
//    /* Write is initiated from this step*/
//    OneWirePin_Write(0); 
//    
//    // Write '0' bit
//    if(bit==0)
//    {
//        /* Minimum 60us for logic_0 */
//        CyDelayUs(WRITE_0_SLOT);
//        // Releases the bus
//       OneWire_Pin2_Write(1);
//    }
//    // Write '1' bit
//    else
//    {
//        /* Release to 1 before 15us for logic_1 */
//        CyDelayUs(WRITE_1_SLOT);
//        // Releases the bus
//        OneWire_Pin2_Write(1);
//        // Complete the time slot
//        CyDelayUs(50);
//       
//    }
//     
//    /* Minimum recovery time 1us between write steps is ncessary, 10us is used */
//    CyDelayUs(RECOVERY_TIME);
//}




int OW_Read_Bit(void)
{
    int result;
    /* Initiate read by pulling line low */
    OneWirePin_Write(0);
    OneWirePin_Write(1);
    /* Data valid before 15us, should be sampled before that*/
    result = OneWirePin_Read();
    // Complete the time slot and 10us recovery
    CyDelayUs(55);
    // Sample the bit value from the slave
    return result;
}

/* For the 2nd 1-Wire */
//int OW2_Read_Bit(void)
//{
//    int result;
//    /* Initiate read by pulling line low */
//    OneWire_Pin2_Write(0);
//
//    OneWire_Pin2_Write(1);
//    /* Data valid before 15us, should be sampled before that*/
//    result = OneWire_Pin2_Read();
//    // Complete the time slot and 10us recovery
//    CyDelayUs(55);
//    // Sample the bit value from the slave
//    return result;
//}

//-----------------------------------------------------------------------------
// Write 1-Wire data byte
//
void OW_Write_Byte(int data)
{
        int loop;

        // Loop to write each bit in the byte, LS-bit first
        for (loop = 0; loop < 8; loop++)
        {
                OW_Write_Bit(data & 0x01);

                // shift the data byte for the next bit
                data >>= 1;
        }
}

/* For the 2nd 1-Wire */
//void OW2_Write_Byte(int data)
//{
//        int loop;
//
//        // Loop to write each bit in the byte, LS-bit first
//        for (loop = 0; loop < 8; loop++)
//        {
//                OW2_Write_Bit(data & 0x01);
//
//                // shift the data byte for the next bit
//                data >>= 1;
//        }
//}

//-----------------------------------------------------------------------------
// Read 1-Wire data byte and return it
//
int OW_Read_Byte(void)
{
        int loop, result=0;

        for (loop = 0; loop < 8; loop++)
        {
                // shift the result to get it ready for the next bit
                result >>= 1;

                // if result is one, then set MS bit
                if (OW_Read_Bit())
                        result |= 0x80;
        }
        return result;
}

/* For the 2nd 1-Wire */
//int OW2_Read_Byte(void)
//{
//        int loop, result=0;
//
//        for (loop = 0; loop < 8; loop++)
//        {
//                // shift the result to get it ready for the next bit
//                result >>= 1;
//
//                // if result is one, then set MS bit
//                if (OW2_Read_Bit())
//                        result |= 0x80;
//        }
//        return result;
//}

/* [] END OF FILE */
