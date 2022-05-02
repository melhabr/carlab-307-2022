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
#include "project.h"
#include <string.h>
#include <stdio.h>

char buf[50];
int i = 0;

CY_ISR(glove_data) {
    
    while (UART_1_GetRxBufferSize()) {
        buf[i] = UART_1_ReadRxData();
        i++;
    }
    
}
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    // Glove control init
    UART_1_Start();
    PWM_Start();
    PWM_Timer_Start();
    Servo_PWM_Start();
    
    // UART_1_PutString("Hello world!\n\r");
    Receive_Data_Start();
    Receive_Data_SetVector(glove_data);
    uint8 speedVal = 0;
    uint8 directionVal = 127;
    uint8 newSpeed;
    float normalized;
    uint16 position;
    
    for(;;)
    {
  
        if (i > 1) {
            
            
            speedVal = (uint8) buf[0];
            directionVal = (uint8) buf[1];
            
            // change speed and direction
            normalized = (speedVal / 255.0) * 100;
            newSpeed = (uint8) normalized;
            PWM_WriteCompare(newSpeed);

            normalized = (directionVal - 127) / 128.0;
            position = normalized * 6000 + 9600;
            Servo_PWM_WriteCompare(position); 
            
            // reset
            i = 0;
        }
        
    }
}

/* [] END OF FILE */
