/*******************************************************************************
* File Name: Receive_Data.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_Receive_Data_H)
#define CY_ISR_Receive_Data_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Receive_Data_Start(void);
void Receive_Data_StartEx(cyisraddress address);
void Receive_Data_Stop(void);

CY_ISR_PROTO(Receive_Data_Interrupt);

void Receive_Data_SetVector(cyisraddress address);
cyisraddress Receive_Data_GetVector(void);

void Receive_Data_SetPriority(uint8 priority);
uint8 Receive_Data_GetPriority(void);

void Receive_Data_Enable(void);
uint8 Receive_Data_GetState(void);
void Receive_Data_Disable(void);

void Receive_Data_SetPending(void);
void Receive_Data_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Receive_Data ISR. */
#define Receive_Data_INTC_VECTOR            ((reg32 *) Receive_Data__INTC_VECT)

/* Address of the Receive_Data ISR priority. */
#define Receive_Data_INTC_PRIOR             ((reg8 *) Receive_Data__INTC_PRIOR_REG)

/* Priority of the Receive_Data interrupt. */
#define Receive_Data_INTC_PRIOR_NUMBER      Receive_Data__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Receive_Data interrupt. */
#define Receive_Data_INTC_SET_EN            ((reg32 *) Receive_Data__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Receive_Data interrupt. */
#define Receive_Data_INTC_CLR_EN            ((reg32 *) Receive_Data__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Receive_Data interrupt state to pending. */
#define Receive_Data_INTC_SET_PD            ((reg32 *) Receive_Data__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Receive_Data interrupt. */
#define Receive_Data_INTC_CLR_PD            ((reg32 *) Receive_Data__INTC_CLR_PD_REG)


#endif /* CY_ISR_Receive_Data_H */


/* [] END OF FILE */
