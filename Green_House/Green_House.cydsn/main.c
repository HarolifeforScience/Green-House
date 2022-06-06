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
#define TRANSBUFFER_SIZE  150
# define ROOM  1
# define SOIL  0

// Function prototypes
void read_Room_Temp(void);
void window_position_control(void);
void heating_control(void);
void real_Time_Clock(void);
uint8 get_soil_temp(void);
uint16 get_CapData(void);


// Interrupts handler prototypes
CY_ISR_PROTO(my_isr_UART);
CY_ISR_PROTO(my_isr_TIMER);
CY_ISR_PROTO(my_isr_PWM);


/* variable declaration for Em_EEPROM */

char    str[64] ;
const uint8_t Em_EEPROM_em_EepromStorage[Em_EEPROM_PHYSICAL_SIZE]
__ALIGNED(CY_FLASH_SIZEOF_ROW) = {0u};
uint8 Stored;

cy_en_em_eeprom_status_t em_eeprom_status ;
uint32_t em_eeprom_write_address = 0 ;
uint32_t em_eeprom_read_address = 0 ;

/*function for Em_EEprom*/

void save_str_to_eeprom(char *str, uint len);
int get_str_from_eeprom(uint32_t address, char *str);
void print_data_from_eeprom(void);
void print_em_eeprom_error(cy_en_em_eeprom_status_t status);
int write(int file, char *ptr, int len);
int read (int fd, const void *Ch, size_t count);

/* the length of the string to be saved */
 uint len = 0 ;


volatile uint8 rtc_data[7];
volatile uint8 room_temp = 0;
volatile uint8 window_position_index = 0;
volatile uint16 timerCount = 0;
volatile uint16 window_position[6] = {899, 1319, 1739, 2159, 2579, 2999};

/* variables declaration for soil measurement */

uint16 capData = 0; // variable to read the sensor values

uint16 baselineData; // sensor baseline

uint8  p;        // loop counter

/* our Transmit Buffer */

char TransmitBuffer[TRANSBUFFER_SIZE];


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
    
    OneWireStart();
    
    /* start Em_EEPROM */
    em_eeprom_status = Em_EEPROM_Init((uint32_t)Em_EEPROM_em_EepromStorage) ;
    
    if (em_eeprom_status != CY_EM_EEPROM_SUCCESS) {
        
        print_em_eeprom_error(em_eeprom_status) ;
    }
    
    // Some Samples
    for(p = 0; p < 10; p++) {
        
        CapSense_ScanSensor(0); 
        
        while(CapSense_IsBusy()); // loop until scan is complete
        
        capData += CapSense_ReadSensorRaw(0); 
    }
    baselineData = capData/10; // this is the baseline to be subtracted from the capsense data
    
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
    //  Read, Calculate and convert the tempearture
    room_temp = OneWireRead_data(SOIL);
    
    window_position_control();
    
    heating_control();
    
}

/* get Soil Temperature from OnewirePin2*/

uint8 get_soil_temp(){
    
    int soil_temp;
    
    soil_temp = OneWireRead_data(ROOM);
    
    return soil_temp;
}

uint16 get_CapData(void) {
    
    // start CapSense scanning of sensor number 0
    CapSense_ScanSensor(0); 
    // loop here until scan is complete
    while(CapSense_IsBusy()); 
    // read the CapSense value of sensor number 0
    capData = CapSense_ReadSensorRaw(0);  // read the CapSense value of sensor number 0
    return capData;
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
   // real_Time_Clock();
  room_temp = OneWireRead_data(SOIL);
  char testData[32];
    
  sprintf(testData," Temp One wire = %u \r\n" ,room_temp);
    
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
         Stored = 0;
         print_data_from_eeprom();
    }
     else if(current_char == 'c' || current_char == 'C')
    {
        // clearing the device data logging memory
         UART_PutString("Clearing the logging memory...\r\n");
         Em_EEPROM_Erase();
         UART_PutString("cleared !");
    }
    
    old_char = current_char;
    // Clearing the Interrupt bit and the Receive register
    UART_ClearRxBuffer();
    isr_UART_ClearPending();
    
}

CY_ISR(my_isr_TIMER)
{
    timerCount = timerCount + 1;
    read_Room_Temp();               // Read the temperature every second
    
    if ( timerCount == 10 ) {
        
        len = sprintf(TransmitBuffer,"{  \" ROOM TEMP\": %u , \"SOIL TEMP\": %u , \"SOIL MOISTURE\": %d }\r\n", room_temp,get_soil_temp(),get_CapData());
        UART_PutString(TransmitBuffer);
        save_str_to_eeprom(TransmitBuffer, len+1) ;
        
        timerCount = 0;
    }
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
void save_str_to_eeprom(char *str, uint len)
{   
     if ((em_eeprom_write_address+len) >= Em_EEPROM_PHYSICAL_SIZE) {
              UART_PutString("Write: Em_EEPROM size exceeded!\n\r") ;
            return ;
     }
    /* write data from address 0 */
    Em_EEPROM_Write(em_eeprom_write_address, str, len) ;
    em_eeprom_write_address += len ;
}

/* write to EEPROM */
int get_str_from_eeprom(uint32_t address, char *str)
{
    int i = 0 ;
    uint8_t data ;
    do {
        if ((address + i) >= Em_EEPROM_PHYSICAL_SIZE) {
            
            UART_PutString("Write: Em_EEPROM size excceeded!\n\r") ;
            
            break ;
        }
        Em_EEPROM_Read(address + i, &data, 1) ;
        
        if (data) {
            
            str[i++] = data ;
        }
        
    } while(data);
    str[i] = 0 ;
    return( i ) ;
}

/* read from EEPROM */
void print_data_from_eeprom(void)
{
    uint address = 0 ;
    do {
        
        len = get_str_from_eeprom(address, str) ;
        
        if (len) {
            
            UART_PutString(str) ;
        }
        address += len+1 ;
        
        if (address >= Em_EEPROM_PHYSICAL_SIZE) {
            
            UART_PutString("Read: Em_EEPROM size excceeded!\n\r") ;
            
            break ;
        }
        
    } while(len) ;
    
   Stored = 1 ;
}

/* print error status */

void print_em_eeprom_error(cy_en_em_eeprom_status_t status)
{
    switch(status) {
    case CY_EM_EEPROM_SUCCESS:      UART_PutString("No Error\n") ; break ;
    
    case CY_EM_EEPROM_BAD_PARAM:    UART_PutString("The input parameter is invalid\n") ; break ;
    
    case CY_EM_EEPROM_BAD_CHECKSUM: UART_PutString("The data in EEPROM is correupted\n") ; break ;
    
    case CY_EM_EEPROM_BAD_DATA:     UART_PutString("Failed to place the EEPROM in flash\n") ; break ;
    
    case CY_EM_EEPROM_WRITE_FAIL:   UART_PutString("Write to EEPROM failed\n") ; break ;
    
    default:
    
        sprintf(str, "EEPROM Unknown Error %d\n", status) ; 
        
        UART_PutString(str) ;
        
        break ;
    }
}

//Replace the gcc default connectors for stdin and stdout  
int write(int file, char *ptr, int len) {  
    
    int i;  
    
    file = file;  
    
    for (i = 0; i < len; i++) {  
        
        UART_PutChar(*ptr++);  
        
        }  
    
    return len;  
  }  
  
  
int read (int fd, const void *Ch, size_t count) {  
  size_t CharCnt = 0x00;  
    
  (void)fd;/* Parameter is not used, suppress unused argument warning */  

  for (;count > 0x00; --count) {  
    
    /* Save character received by UARTx device into the receive buffer */  
    
    while(!(*(uint8_t*)Ch = (unsigned char)UART_GetChar()));  
    
    CharCnt++;   /* Increase char counter */  
    
    /* Stop reading if CR (Ox0D) character is received */  
    
    if (*(uint8_t*)Ch == 0x0DU) { /* New line character (CR) received ? */  
        
      *(uint8_t*)Ch = '\n';  /* Yes, convert LF to '\n' char. */  
    
      break; /* Stop loop and return received char(s) */  
    
    }  
    Ch++; /* Increase buffer pointer */  
  }  
  //return 1; /* WRONG! */  
  return CharCnt;  
}  




/* [] END OF FILE */
