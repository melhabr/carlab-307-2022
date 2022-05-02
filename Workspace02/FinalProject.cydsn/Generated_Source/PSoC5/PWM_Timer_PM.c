/*******************************************************************************
* File Name: PWM_Timer_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "PWM_Timer.h"

static PWM_Timer_backupStruct PWM_Timer_backup;


/*******************************************************************************
* Function Name: PWM_Timer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  PWM_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_Timer_SaveConfig(void) 
{
    #if (!PWM_Timer_UsingFixedFunction)
        PWM_Timer_backup.TimerUdb = PWM_Timer_ReadCounter();
        PWM_Timer_backup.InterruptMaskValue = PWM_Timer_STATUS_MASK;
        #if (PWM_Timer_UsingHWCaptureCounter)
            PWM_Timer_backup.TimerCaptureCounter = PWM_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!PWM_Timer_UDB_CONTROL_REG_REMOVED)
            PWM_Timer_backup.TimerControlRegister = PWM_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: PWM_Timer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  PWM_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Timer_RestoreConfig(void) 
{   
    #if (!PWM_Timer_UsingFixedFunction)

        PWM_Timer_WriteCounter(PWM_Timer_backup.TimerUdb);
        PWM_Timer_STATUS_MASK =PWM_Timer_backup.InterruptMaskValue;
        #if (PWM_Timer_UsingHWCaptureCounter)
            PWM_Timer_SetCaptureCount(PWM_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!PWM_Timer_UDB_CONTROL_REG_REMOVED)
            PWM_Timer_WriteControlRegister(PWM_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: PWM_Timer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  PWM_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_Timer_Sleep(void) 
{
    #if(!PWM_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(PWM_Timer_CTRL_ENABLE == (PWM_Timer_CONTROL & PWM_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            PWM_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            PWM_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    PWM_Timer_Stop();
    PWM_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_Timer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  PWM_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Timer_Wakeup(void) 
{
    PWM_Timer_RestoreConfig();
    #if(!PWM_Timer_UDB_CONTROL_REG_REMOVED)
        if(PWM_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                PWM_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
