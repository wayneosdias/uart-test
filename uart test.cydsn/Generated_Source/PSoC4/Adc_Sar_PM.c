/*******************************************************************************
* File Name: Adc_Sar_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Adc_Sar.h"


/***************************************
* Local data allocation
***************************************/

static Adc_Sar_BACKUP_STRUCT  Adc_Sar_backup =
{
    Adc_Sar_DISABLED,
    0u    
};


/*******************************************************************************
* Function Name: Adc_Sar_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Adc_Sar_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: Adc_Sar_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Adc_Sar_RestoreConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: Adc_Sar_Sleep
********************************************************************************
*
* Summary:
*  Stops the ADC operation and saves the configuration registers and component
*  enable state. Should be called just prior to entering sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Adc_Sar_backup - modified.
*
*******************************************************************************/
void Adc_Sar_Sleep(void)
{
    /* During deepsleep/ hibernate mode keep SARMUX active, i.e. do not open
    *   all switches (disconnect), to be used for ADFT
    */
    Adc_Sar_backup.dftRegVal = Adc_Sar_SAR_DFT_CTRL_REG & (uint32)~Adc_Sar_ADFT_OVERRIDE;
    Adc_Sar_SAR_DFT_CTRL_REG |= Adc_Sar_ADFT_OVERRIDE;
    if((Adc_Sar_SAR_CTRL_REG  & Adc_Sar_ENABLE) != 0u)
    {
        if((Adc_Sar_SAR_SAMPLE_CTRL_REG & Adc_Sar_CONTINUOUS_EN) != 0u)
        {
            Adc_Sar_backup.enableState = Adc_Sar_ENABLED | Adc_Sar_STARTED;
        }
        else
        {
            Adc_Sar_backup.enableState = Adc_Sar_ENABLED;
        }
        Adc_Sar_StopConvert();
        Adc_Sar_Stop();
        
        /* Disable the SAR internal pump before entering the chip low power mode */
        if((Adc_Sar_SAR_CTRL_REG & Adc_Sar_BOOSTPUMP_EN) != 0u)
        {
            Adc_Sar_SAR_CTRL_REG &= (uint32)~Adc_Sar_BOOSTPUMP_EN;
            Adc_Sar_backup.enableState |= Adc_Sar_BOOSTPUMP_ENABLED;
        }
    }
    else
    {
        Adc_Sar_backup.enableState = Adc_Sar_DISABLED;
    }
}


/*******************************************************************************
* Function Name: Adc_Sar_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component enable state and configuration registers.
*  This should be called just after awaking from sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Adc_Sar_backup - used.
*
*******************************************************************************/
void Adc_Sar_Wakeup(void)
{
    Adc_Sar_SAR_DFT_CTRL_REG = Adc_Sar_backup.dftRegVal;
    if(Adc_Sar_backup.enableState != Adc_Sar_DISABLED)
    {
        /* Enable the SAR internal pump  */
        if((Adc_Sar_backup.enableState & Adc_Sar_BOOSTPUMP_ENABLED) != 0u)
        {
            Adc_Sar_SAR_CTRL_REG |= Adc_Sar_BOOSTPUMP_EN;
        }
        Adc_Sar_Enable();
        if((Adc_Sar_backup.enableState & Adc_Sar_STARTED) != 0u)
        {
            Adc_Sar_StartConvert();
        }
    }
}
/* [] END OF FILE */
