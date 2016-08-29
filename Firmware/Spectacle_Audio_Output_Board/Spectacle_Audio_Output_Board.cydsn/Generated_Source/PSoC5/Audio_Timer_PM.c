/*******************************************************************************
* File Name: Audio_Timer_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Audio_Timer.h"

static Audio_Timer_backupStruct Audio_Timer_backup;


/*******************************************************************************
* Function Name: Audio_Timer_SaveConfig
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
*  Audio_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Audio_Timer_SaveConfig(void) 
{
    #if (!Audio_Timer_UsingFixedFunction)
        Audio_Timer_backup.TimerUdb = Audio_Timer_ReadCounter();
        Audio_Timer_backup.InterruptMaskValue = Audio_Timer_STATUS_MASK;
        #if (Audio_Timer_UsingHWCaptureCounter)
            Audio_Timer_backup.TimerCaptureCounter = Audio_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Audio_Timer_UDB_CONTROL_REG_REMOVED)
            Audio_Timer_backup.TimerControlRegister = Audio_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Audio_Timer_RestoreConfig
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
*  Audio_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Audio_Timer_RestoreConfig(void) 
{   
    #if (!Audio_Timer_UsingFixedFunction)

        Audio_Timer_WriteCounter(Audio_Timer_backup.TimerUdb);
        Audio_Timer_STATUS_MASK =Audio_Timer_backup.InterruptMaskValue;
        #if (Audio_Timer_UsingHWCaptureCounter)
            Audio_Timer_SetCaptureCount(Audio_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Audio_Timer_UDB_CONTROL_REG_REMOVED)
            Audio_Timer_WriteControlRegister(Audio_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Audio_Timer_Sleep
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
*  Audio_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Audio_Timer_Sleep(void) 
{
    #if(!Audio_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Audio_Timer_CTRL_ENABLE == (Audio_Timer_CONTROL & Audio_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Audio_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Audio_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Audio_Timer_Stop();
    Audio_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Audio_Timer_Wakeup
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
*  Audio_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Audio_Timer_Wakeup(void) 
{
    Audio_Timer_RestoreConfig();
    #if(!Audio_Timer_UDB_CONTROL_REG_REMOVED)
        if(Audio_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Audio_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
