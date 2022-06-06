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
// Onewire 
void OneWireStart(void);
int OneWireRead_data(int args);
int OwReset(void);
void OwWriteBit(int b);
int OwReadBit(void);
void OwWriteByte(int data);
int OwReadByte(void);
// OneWire 2
int Ow2Reset(void);
void Ow2WriteBit(int b);
int Ow2ReadBit(void);
void Ow2WriteByte(int data);
int Ow2ReadByte(void);

/* [] END OF FILE */
