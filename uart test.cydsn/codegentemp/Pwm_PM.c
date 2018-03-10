/*******************************************************************************
* File Name: Pwm_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Pwm.h"

static Pwm_backupStruct Pwm_backup;


/*******************************************************************************
* Function Name: Pwm_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Pwm_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Pwm_SaveConfig(void) 
{

    #if(!Pwm_UsingFixedFunction)
        #if(!Pwm_PWMModeIsCenterAligned)
            Pwm_backup.PWMPeriod = Pwm_ReadPeriod();
        #endif /* (!Pwm_PWMModeIsCenterAligned) */
        Pwm_backup.PWMUdb = Pwm_ReadCounter();
        #if (Pwm_UseStatus)
            Pwm_backup.InterruptMaskValue = Pwm_STATUS_MASK;
        #endif /* (Pwm_UseStatus) */

        #if(Pwm_DeadBandMode == Pwm__B_PWM__DBM_256_CLOCKS || \
            Pwm_DeadBandMode == Pwm__B_PWM__DBM_2_4_CLOCKS)
            Pwm_backup.PWMdeadBandValue = Pwm_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Pwm_KillModeMinTime)
             Pwm_backup.PWMKillCounterPeriod = Pwm_ReadKillTime();
        #endif /* (Pwm_KillModeMinTime) */

        #if(Pwm_UseControl)
            Pwm_backup.PWMControlRegister = Pwm_ReadControlRegister();
        #endif /* (Pwm_UseControl) */
    #endif  /* (!Pwm_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Pwm_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Pwm_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Pwm_RestoreConfig(void) 
{
        #if(!Pwm_UsingFixedFunction)
            #if(!Pwm_PWMModeIsCenterAligned)
                Pwm_WritePeriod(Pwm_backup.PWMPeriod);
            #endif /* (!Pwm_PWMModeIsCenterAligned) */

            Pwm_WriteCounter(Pwm_backup.PWMUdb);

            #if (Pwm_UseStatus)
                Pwm_STATUS_MASK = Pwm_backup.InterruptMaskValue;
            #endif /* (Pwm_UseStatus) */

            #if(Pwm_DeadBandMode == Pwm__B_PWM__DBM_256_CLOCKS || \
                Pwm_DeadBandMode == Pwm__B_PWM__DBM_2_4_CLOCKS)
                Pwm_WriteDeadTime(Pwm_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Pwm_KillModeMinTime)
                Pwm_WriteKillTime(Pwm_backup.PWMKillCounterPeriod);
            #endif /* (Pwm_KillModeMinTime) */

            #if(Pwm_UseControl)
                Pwm_WriteControlRegister(Pwm_backup.PWMControlRegister);
            #endif /* (Pwm_UseControl) */
        #endif  /* (!Pwm_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Pwm_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Pwm_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Pwm_Sleep(void) 
{
    #if(Pwm_UseControl)
        if(Pwm_CTRL_ENABLE == (Pwm_CONTROL & Pwm_CTRL_ENABLE))
        {
            /*Component is enabled */
            Pwm_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Pwm_backup.PWMEnableState = 0u;
        }
    #endif /* (Pwm_UseControl) */

    /* Stop component */
    Pwm_Stop();

    /* Save registers configuration */
    Pwm_SaveConfig();
}


/*******************************************************************************
* Function Name: Pwm_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Pwm_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Pwm_Wakeup(void) 
{
     /* Restore registers values */
    Pwm_RestoreConfig();

    if(Pwm_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Pwm_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
