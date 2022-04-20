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
    char projectName[50] = "Green House Project \n\r";
    UART_PutString(projectName);
   
    
    for(;;)
    {
        /* Place your application code here. */
       
    }
}

void realTimeClock(void)
{
    uint8 i, result = 0;
    
    // read the rtm module data from the i2c master
  
    I2C_RTC_MasterSendStart(0x68, I2C_RTC_WRITE_XFER_MODE);
    
    I2C_RTC_MasterWriteByte(0x00);
    
    I2C_RTC_MasterSendRestart(0x68, I2C_RTC_WRITE_XFER_MODE);
    
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
        sprintf(testData,"Bit%d : %d\r\n" ,i , rtc_data[i]>>4&0x07);
        UART_PutString(testData);
    }

}


CY_ISR(my_isr_UART)
{
    realTimeClock();
    
    // Terminal Interface Commande
    
    // Clearing the Interrupt bit and the Receive register
    UART_ClearRxBuffer();
    isr_UART_ClearPending();
    
}

/* [] END OF FILE */
