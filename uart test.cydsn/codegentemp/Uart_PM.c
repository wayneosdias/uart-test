/***************************************************************************//**
* \file Uart_PM.c
* \version 3.20
*
* \brief
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Uart.h"
#include "Uart_PVT.h"

#if(Uart_SCB_MODE_I2C_INC)
    #include "Uart_I2C_PVT.h"
#endif /* (Uart_SCB_MODE_I2C_INC) */

#if(Uart_SCB_MODE_EZI2C_INC)
    #include "Uart_EZI2C_PVT.h"
#endif /* (Uart_SCB_MODE_EZI2C_INC) */

#if(Uart_SCB_MODE_SPI_INC || Uart_SCB_MODE_UART_INC)
    #include "Uart_SPI_UART_PVT.h"
#endif /* (Uart_SCB_MODE_SPI_INC || Uart_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(Uart_SCB_MODE_UNCONFIG_CONST_CFG || \
   (Uart_SCB_MODE_I2C_CONST_CFG   && (!Uart_I2C_WAKE_ENABLE_CONST))   || \
   (Uart_SCB_MODE_EZI2C_CONST_CFG && (!Uart_EZI2C_WAKE_ENABLE_CONST)) || \
   (Uart_SCB_MODE_SPI_CONST_CFG   && (!Uart_SPI_WAKE_ENABLE_CONST))   || \
   (Uart_SCB_MODE_UART_CONST_CFG  && (!Uart_UART_WAKE_ENABLE_CONST)))

    Uart_BACKUP_STRUCT Uart_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: Uart_Sleep
****************************************************************************//**
*
*  Prepares the Uart component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the Uart_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void Uart_Sleep(void)
{
#if(Uart_SCB_MODE_UNCONFIG_CONST_CFG)

    if(Uart_SCB_WAKE_ENABLE_CHECK)
    {
        if(Uart_SCB_MODE_I2C_RUNTM_CFG)
        {
            Uart_I2CSaveConfig();
        }
        else if(Uart_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            Uart_EzI2CSaveConfig();
        }
    #if(!Uart_CY_SCBIP_V1)
        else if(Uart_SCB_MODE_SPI_RUNTM_CFG)
        {
            Uart_SpiSaveConfig();
        }
        else if(Uart_SCB_MODE_UART_RUNTM_CFG)
        {
            Uart_UartSaveConfig();
        }
    #endif /* (!Uart_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        Uart_backup.enableState = (uint8) Uart_GET_CTRL_ENABLED;

        if(0u != Uart_backup.enableState)
        {
            Uart_Stop();
        }
    }

#else

    #if (Uart_SCB_MODE_I2C_CONST_CFG && Uart_I2C_WAKE_ENABLE_CONST)
        Uart_I2CSaveConfig();

    #elif (Uart_SCB_MODE_EZI2C_CONST_CFG && Uart_EZI2C_WAKE_ENABLE_CONST)
        Uart_EzI2CSaveConfig();

    #elif (Uart_SCB_MODE_SPI_CONST_CFG && Uart_SPI_WAKE_ENABLE_CONST)
        Uart_SpiSaveConfig();

    #elif (Uart_SCB_MODE_UART_CONST_CFG && Uart_UART_WAKE_ENABLE_CONST)
        Uart_UartSaveConfig();

    #else

        Uart_backup.enableState = (uint8) Uart_GET_CTRL_ENABLED;

        if(0u != Uart_backup.enableState)
        {
            Uart_Stop();
        }

    #endif /* defined (Uart_SCB_MODE_I2C_CONST_CFG) && (Uart_I2C_WAKE_ENABLE_CONST) */

#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: Uart_Wakeup
****************************************************************************//**
*
*  Prepares the Uart component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the Uart_Wakeup() function without first calling the 
*   Uart_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void Uart_Wakeup(void)
{
#if(Uart_SCB_MODE_UNCONFIG_CONST_CFG)

    if(Uart_SCB_WAKE_ENABLE_CHECK)
    {
        if(Uart_SCB_MODE_I2C_RUNTM_CFG)
        {
            Uart_I2CRestoreConfig();
        }
        else if(Uart_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            Uart_EzI2CRestoreConfig();
        }
    #if(!Uart_CY_SCBIP_V1)
        else if(Uart_SCB_MODE_SPI_RUNTM_CFG)
        {
            Uart_SpiRestoreConfig();
        }
        else if(Uart_SCB_MODE_UART_RUNTM_CFG)
        {
            Uart_UartRestoreConfig();
        }
    #endif /* (!Uart_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != Uart_backup.enableState)
        {
            Uart_Enable();
        }
    }

#else

    #if (Uart_SCB_MODE_I2C_CONST_CFG  && Uart_I2C_WAKE_ENABLE_CONST)
        Uart_I2CRestoreConfig();

    #elif (Uart_SCB_MODE_EZI2C_CONST_CFG && Uart_EZI2C_WAKE_ENABLE_CONST)
        Uart_EzI2CRestoreConfig();

    #elif (Uart_SCB_MODE_SPI_CONST_CFG && Uart_SPI_WAKE_ENABLE_CONST)
        Uart_SpiRestoreConfig();

    #elif (Uart_SCB_MODE_UART_CONST_CFG && Uart_UART_WAKE_ENABLE_CONST)
        Uart_UartRestoreConfig();

    #else

        if(0u != Uart_backup.enableState)
        {
            Uart_Enable();
        }

    #endif /* (Uart_I2C_WAKE_ENABLE_CONST) */

#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
