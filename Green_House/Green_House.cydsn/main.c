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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#include <OneWireLab.h>

// Define and global variables
# define RTC_slaveAddress 0x68
# define True 1
# define False 0
# define Servo_First_Position 899
# define TRANSBUFFER_SIZE  150
# define ROOM  1
# define SOIL  0

# define MAX_LENGTH              (4u)
# define VALID_DAY_LENGTH        (2u)
# define VALID_MONTH_LENGTH      (2u)
# define VALID_SHORT_YEAR_LENGTH (2u)
# define VALID_LONG_YEAR_LENGTH  (4u)

/* Maximum value of seconds and minutes */
# define MAX_SEC_OR_MIN (60u)

/* Maximum value of hours definition */
# define MAX_HOURS_24H (23UL)

/* Month per year definition */
# define MONTHS_PER_YEAR (12U)

/* Days per week definition */
# define DAYS_PER_WEEK (7u)


# define IS_SEC_VALID(sec) ((sec) <= MAX_SEC_OR_MIN )
# define IS_MIN_VALID(min) ((min) <= MAX_SEC_OR_MIN )
# define IS_HOUR_VALID(hour) ((hour) <= MAX_HOURS_24H)
# define IS_MONTH_VALID(month) (((month > 0u ) && ((month) <= MONTHS_PER_YEAR ) )
# define IS_YEAR_VALID(year) ((year) > 0u )

#define RTC_ACCESS_RETRY         (50u)
#define  RTC_MAX_YEAR            (4u)


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

/*Function for RTC*/

bool ValidateDateTime(RTC_TIME_DATE *datetime);
static void Rtc_PrintDateTime(void);
static void Rtc_SetTime(void);
bool ValidateDateTime(RTC_TIME_DATE *datetime);

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
    
    RTC_TIME_DATE Start;
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();
  
    isr_UART_StartEx(my_isr_UART);
    
    Timer_Start();
    isr_Timer_StartEx(my_isr_TIMER);
    
    PWM_Start();
    isr_PWM_StartEx(my_isr_PWM);
    
    Start.Sec = 55u;
   
    Start.Min = 55u;

    Start.Hour = 13u;

    Start.DayOfMonth = 7u;

    Start.Month = 7u;
   
    Start.Year = 2015u;

    RTC_WriteTime(&Start);
    
    RTC_Start();
    
    RTC_Init();
    
    RTC_Enable();
  
    
    OneWireStart();
    
    /* start Em_EEPROM */
    em_eeprom_status = Em_EEPROM_Init((uint32_t)Em_EEPROM_em_EepromStorage) ;
    
    if (em_eeprom_status != CY_EM_EEPROM_SUCCESS) {
        
        print_em_eeprom_error(em_eeprom_status) ;
    }
    
    // Some Samples
    /*for(p = 0; p < 10; p++) {
        
        CapSense_ScanSensor(0); 
        
        while(CapSense_IsBusy()); // loop until scan is complete
        
        capData += CapSense_ReadSensorRaw(0); 
    }
    baselineData = capData/10; // this is the baseline to be subtracted from the capsense data
    */
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
CY_ISR(my_isr_UART)
{  
    // Terminal Interface Commande
    static uint8 old_char = 0 , last_char = 0;
    uint8 current_char;
    current_char = UART_GetChar();
    
    UART_PutChar(current_char);
    
    if((old_char == 't' || old_char == 'T') && current_char == 13)
    {  
        // Setting the current time for the device
        UART_PutString("\nSetting the current time for the device\r\n");  
        //Rtc_SetTime();
    }
    else if((old_char == 'd' || old_char == 'D') && current_char ==  13)
    {
        // Setting the current day for the device
        UART_PutString("\nSetting the current day for the device\r\n");
        Rtc_SetTime();
    }
    else if((old_char == '?')&& current_char == 13)
    {
        if(last_char == 't' || last_char == 'T' || last_char == 'd' || last_char == 'D')
        {
            // Sending out the current device time
            UART_PutString("\nSending out the current device time\r\n");
            Rtc_PrintDateTime();
        }
       /* else if(old_char == 'd' || old_char == 'D')
        {
            // Sending out the current device day
            UART_PutString("Sending out the current device day\r\n");
        }*/
        else
        {
            // Sending out indentification information 
            UART_PutString("\nGreenHouse controller 0.1, developed by Yameni Gleen and Ndassi Harold\r\n");
        }
    }
     else if((old_char == 'a' || old_char == 'A') && current_char == 13)
    {
        // Sending out all saved data in JSON format: date, time, all temperatures, air humidity, soil moisture, light sensor
        UART_PutString("\nSending all saved data...\r\n");
         Stored = 0;
         print_data_from_eeprom();
    }
     else if((old_char == 'c' || old_char == 'C') && current_char == 13)
    {
        // clearing the device data logging memory
         UART_PutString("\nClearing the logging memory...\r\n");
         Em_EEPROM_Erase();
         UART_PutString("cleared !");
    }
    
    last_char = old_char;
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
    
 /* Parameter is not used, suppress unused argument warning */  
 (void)fd;

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


// RTC 

static void Rtc_PrintDateTime(void)
{
	
    /* Get the current RTC date and time */
    
    RTC_TIME_DATE *dateTime;
    dateTime = RTC_ReadTime();
    
    CyDelay(10000);
	
	/* Display current date and time */
	sprintf(TransmitBuffer,"\rCurrent Time: %u : %u : %u  %u/%u/%u\r\n", 
        
			dateTime->Hour = RTC_ReadHour(), dateTime->Min, dateTime->Sec, \
        
			dateTime->DayOfWeek, dateTime->Month, dateTime->Year);
    
    UART_PutString(TransmitBuffer);
}


static void Rtc_SetTime(void)
{
	/* Variables used to store user input */
	char dateStr[MAX_LENGTH], monthStr[MAX_LENGTH], yearStr[MAX_LENGTH];
    
	char secStr[MAX_LENGTH], minStr[MAX_LENGTH], hourStr[MAX_LENGTH];

	/* Variables used to store date and time information */
	//uint32_t date, month, year, sec, min, hour;

	/* Variable used to store return status of RTC API */
	//cy_en_rtc_status_t  rtcApiStatus;

	uint32_t rtcAccessRetry = RTC_ACCESS_RETRY;

	UART_PutString("\r\nEnter new date (DD MM YY)\r\n");
	scanf("%s %s %s", dateStr, monthStr, yearStr);

	/* validate user input */
	if(strlen(dateStr)<= VALID_DAY_LENGTH && strlen(monthStr)<= VALID_MONTH_LENGTH  && \
	(strlen(yearStr)<= VALID_SHORT_YEAR_LENGTH || strlen(yearStr)== VALID_LONG_YEAR_LENGTH ))
	{
	    UART_PutString("\rEnter new time in 24-hour format (HH MM SS)\r\n");
		sscanf("%s %s %s", hourStr, minStr, secStr);
        
        RTC_TIME_DATE *time;

		/* Convert string input to decimal */
		time-> DayOfWeek = atoi(dateStr);
	    time-> Month  = atoi(monthStr);
		time ->Year   = atoi(yearStr);
		time -> Sec   = atoi(secStr);
		time -> Min   = atoi(minStr);
		time -> Hour  = atoi(hourStr);

		/* If user input 4 digits Year information, set 2 digits Year */
		if(time ->Year > RTC_MAX_YEAR)
		{
			time ->Year = time ->Year % 100u;
		}

		if(ValidateDateTime(time))
		{
			/* Set date and time.
			 * RTC block doesn't allow to access, when synchronizing the user registers
			 * and the internal actual RTC registers. It will return RTC_BUSY value, if
			 * it is not available to update the configuration values. Needs to retry,
			 * if it doesn't return CY_RTC_SUCCESS.
			 */
			do
			{
				RTC_WriteTime(time);
				rtcAccessRetry --;
			} while ((RTC_ReadStatus() != RTC_STATUS_DST) && (rtcAccessRetry != 0));

			if((RTC_ReadStatus() != RTC_STATUS_DST))
			{
				UART_PutString("\r\nFailed to update date and time\r\n");
			}
			else
			{
				UART_PutString("\r\nDate and Time updated !!\r\n");
				//Rtc_PrintDateTime();
			}
		}
		else
		{
			printf("\r\nInvalid values! Please enter the values in specified format\r\n");
			//PrintAvailableCommand();
		}
	}
	else
	{
		printf("\r\nInvalid values! Please enter the values in specified format\r\n");
		//PrintAvailableCommand();
	}

}

bool ValidateDateTime(RTC_TIME_DATE *datetime)
{
	uint8_t daysInMonth;
    
	/* Variable used to store days in months table */
	static uint8_t daysInMonthTable[RTC_MONTHS_IN_YEAR] = {
		RTC_DAYS_IN_JANUARY,
		RTC_DAYS_IN_FEBRUARY,
		RTC_DAYS_IN_MARCH,
		RTC_DAYS_IN_APRIL,
		RTC_DAYS_IN_MAY,
		RTC_DAYS_IN_JUNE,
		RTC_DAYS_IN_JULY,
		RTC_DAYS_IN_AUGUST,
		RTC_DAYS_IN_SEPTEMBER,
		RTC_DAYS_IN_OCTOBER,
		RTC_DAYS_IN_NOVEMBER,
		RTC_DAYS_IN_DECEMBER};

	bool status = IS_SEC_VALID(datetime ->Sec) & IS_MIN_VALID(datetime->Min) &
                  IS_HOUR_VALID(datetime ->Hour) & IS_MONTH_VALID(datetime->Month) &
                  IS_YEAR_VALID(datetime ->Year ) ) ;

	if(status)
	{
		daysInMonth = daysInMonthTable[datetime->Month - 1];

		if(RTC_LEAP_YEAR(datetime->Year) && \
			(datetime->Month == RTC_FEBRUARY))
		{
			daysInMonth++;
		}
		status &= (datetime->DayOfWeek > 0U) && (datetime->DayOfWeek <= daysInMonth);
	}
    
	return status;
}



/* [] END OF FILE */
