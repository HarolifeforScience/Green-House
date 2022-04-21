/* ========================================
 *
 * Green House Project PSoC
 * 
 * Author: Yameni Bakam Gleen Karel & Ndassi Fotso Harold
 *
 * Last version: 18.04.2022
 * 
 * School: Frankfurt University of Applied Sciences
 * ========================================
*/
#include "project.h"
#include <stdio.h>

// Define and global variables
# define RTC_slaveAddress 0x68
# define True 1
# define False 0

// Function prototypes
void realTimeClock(void);

// Interrupts handler prototypes
CY_ISR_PROTO(my_isr_UART);



volatile uint8 rtc_data[7];

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();
    isr_UART_StartEx(my_isr_UART);
    //
    I2C_RTC_Start();
    
    
    // test transmition to putty
    char projectName[50] = "***Green House Project*** \n\r";
    UART_PutString(projectName);
   
    
    for(;;)
    {
        /* Place your application code here. */
       
    }
}

void realTimeClock(void)
{
    uint8 i, result = 0;
    char debug[50];
    
    // read the rtm module data from the i2c master
    do
    {
        result = I2C_RTC_MasterSendStart(RTC_slaveAddress, I2C_RTC_WRITE_XFER_MODE);
        sprintf(debug,"Endless loop: %d\r\n" , result);
        UART_PutString(debug);
    }while(result!=I2C_RTC_MSTR_NO_ERROR);
    
    result = I2C_RTC_MasterWriteByte(0x00);
    
    result = I2C_RTC_MasterSendRestart(RTC_slaveAddress, I2C_RTC_WRITE_XFER_MODE);
    
    for(i=0; i<6; i++)
    {
        rtc_data[i] = I2C_RTC_MasterReadByte(I2C_RTC_ACK_DATA);
    }
    
    rtc_data[6] = I2C_RTC_MasterReadByte(I2C_RTC_NAK_DATA);
    
    I2C_RTC_MasterSendStop();
    
    // send the result to the commad window
    char testData[20];
    for(i=0; i<7; i++)
    {
        sprintf(testData,"Byte %d : %d\r\n" ,i , rtc_data[i]);
        UART_PutString(testData);
    }
    //rtc_data[i]>>4&0x07
}


CY_ISR(my_isr_UART)
{   
    // Testing start
    realTimeClock();
    
    // Testing ends
    
    // Terminal Interface Commande
    static uint8 old_char = 0;
    uint8 current_char;
    current_char = UART_GetChar();
    
    if(current_char == 't' || current_char == 'T')
    {
        // Setting the current time for the device
        UART_PutString("Setting the current time for the device\r\n");  
    }
    else if(current_char == 'd' || current_char == 'D')
    {
        // Setting the current day for the device
        UART_PutString("Setting the current day for the device\r\n");  
    }
    else if(current_char == '?')
    {
        if(old_char == 't' || old_char == 'T')
        {
            // Sending out the current device time
            UART_PutString("Sending out the current device time\r\n");
        }
        else if(old_char == 'd' || old_char == 'D')
        {
            // Sending out the current device day
            UART_PutString("Sending out the current device day\r\n");
        }
        else
        {
            // Sending out indentification information 
            UART_PutString("GreenHouse controller 0.1, developed by Yameni Gleen and Ndassi Harold\r\n");
        }
    }
     else if(current_char == 'a' || current_char == 'A')
    {
        // Sending out all saved data in JSON format: date, time, all temperatures, air humidity, soil moisture, light sensor
        UART_PutString("Sending all saved data...\r\n");
    }
     else if(current_char == 'c' || current_char == 'C')
    {
        // clearing the device data logging memory
        UART_PutString("Clearing the logging memory...\r\n");
    }
    
    old_char = current_char;
    // Clearing the Interrupt bit and the Receive register
    UART_ClearRxBuffer();
    isr_UART_ClearPending();
    
}

/* [] END OF FILE */
