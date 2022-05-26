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
#include <OneWireLab.h>

// Define and global variables
# define RTC_slaveAddress 0x68
# define True 1
# define False 0
# define Servo_First_Position 899

// Function prototypes
void read_Room_Temp(void);
void window_position_control(void);
void heating_control(void);
void real_Time_Clock(void);

// Interrupts handler prototypes
CY_ISR_PROTO(my_isr_UART);
CY_ISR_PROTO(my_isr_TIMER);
CY_ISR_PROTO(my_isr_PWM);


volatile uint8 rtc_data[7];
volatile uint8 room_temp = 0;
volatile uint8 window_position_index = 0;
volatile uint16 window_position[6] = {899, 1319, 1739, 2159, 2579, 2999};
volatile uint16 OneWireVal = 0 ;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();
    isr_UART_StartEx(my_isr_UART);
    
    Timer_Start();
    isr_Timer_StartEx(my_isr_TIMER);
    
    PWM_Start();
    isr_PWM_StartEx(my_isr_PWM);
    
    //
    I2C_RTC_Start();
    
    OneWire_start();
    
    // test transmition to putty
    char projectName[50] = "***Green House Project*** \n\r";
    UART_PutString(projectName);
   
    
    for(;;)
    {
        /* Place your application code here. */
       
    }
}

void heating_control(void)
{
    if (room_temp < 5)
    {
        LED_Heating_Write(True);    // Turns on extra heating if temperature falls below +5C
    }
    else if (room_temp > 10)
    {
        LED_Heating_Write(False);   //  Turns off extra heating if temperature rises above +10C
    }
}

void window_position_control(void)
{
    if (room_temp>=25)
    {
        window_position_index = 5;    // Fully opened
    }
    else if (room_temp>=24)
    {
        window_position_index = 4;
    }
    else if (room_temp>=23)
    {
        window_position_index = 3;
    }
    else if (room_temp>=22)
    {
        window_position_index = 2;
    }
    else if (room_temp>=21)
    {
        window_position_index = 1;
    }
    else
    {
        window_position_index = 0;    // Fully closed
    }
}
void read_Room_Temp(void)
{
    
    
    // ***To-Do**
    //  Read, Calculate and convert the tempearture
    //
    
    window_position_control();
    
    heating_control();
    
}


void real_Time_Clock(void)
{
    uint8 i, result = 0;
    char debug[50];
    
    // read the real time clock module data from the i2c master
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
   // real_Time_Clock();
    OneWireVal = OneWire_read_data();
    char testData[20];
    
    sprintf(testData," Temp One wire = %u \r\n" ,OneWireVal);
    
    UART_PutString(testData);
    
    
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

CY_ISR(my_isr_TIMER)
{
    read_Room_Temp();               // Read the temperature every second
    
    Timer_ReadStatusRegister();     // Clear the interrupt flag
}

CY_ISR(my_isr_PWM)
{
    static uint16 actualPosition = Servo_First_Position;    // actual position of the Servo Motor
    
    if (actualPosition > window_position[window_position_index]) actualPosition -= 1; // smooth the servo movement by gradualy incrementing/decrementing the position
    if (actualPosition < window_position[window_position_index]) actualPosition += 1;
    
    PWM_WriteCompare(actualPosition);   // Move the Servo to the desired position
    
    PWM_ReadStatusRegister();           // Clear the interrupt flag
}

/* [] END OF FILE */
