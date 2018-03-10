/***************************************************************************//**
* \file .h
* \version 3.20
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_Uart_H)
#define CY_SCB_PVT_Uart_H

#include "Uart.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define Uart_SetI2CExtClkInterruptMode(interruptMask) Uart_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define Uart_ClearI2CExtClkInterruptSource(interruptMask) Uart_CLEAR_INTR_I2C_EC(interruptMask)
#define Uart_GetI2CExtClkInterruptSource()                (Uart_INTR_I2C_EC_REG)
#define Uart_GetI2CExtClkInterruptMode()                  (Uart_INTR_I2C_EC_MASK_REG)
#define Uart_GetI2CExtClkInterruptSourceMasked()          (Uart_INTR_I2C_EC_MASKED_REG)

#if (!Uart_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define Uart_SetSpiExtClkInterruptMode(interruptMask) \
                                                                Uart_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define Uart_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                Uart_CLEAR_INTR_SPI_EC(interruptMask)
    #define Uart_GetExtSpiClkInterruptSource()                 (Uart_INTR_SPI_EC_REG)
    #define Uart_GetExtSpiClkInterruptMode()                   (Uart_INTR_SPI_EC_MASK_REG)
    #define Uart_GetExtSpiClkInterruptSourceMasked()           (Uart_INTR_SPI_EC_MASKED_REG)
#endif /* (!Uart_CY_SCBIP_V1) */

#if(Uart_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void Uart_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (Uart_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_Uart_CUSTOM_INTR_HANDLER)
    extern cyisraddress Uart_customIntrHandler;
#endif /* !defined (CY_REMOVE_Uart_CUSTOM_INTR_HANDLER) */
#endif /* (Uart_SCB_IRQ_INTERNAL) */

extern Uart_BACKUP_STRUCT Uart_backup;

#if(Uart_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 Uart_scbMode;
    extern uint8 Uart_scbEnableWake;
    extern uint8 Uart_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 Uart_mode;
    extern uint8 Uart_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * Uart_rxBuffer;
    extern uint8   Uart_rxDataBits;
    extern uint32  Uart_rxBufferSize;

    extern volatile uint8 * Uart_txBuffer;
    extern uint8   Uart_txDataBits;
    extern uint32  Uart_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 Uart_numberOfAddr;
    extern uint8 Uart_subAddrSize;
#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (Uart_SCB_MODE_I2C_CONST_CFG || \
        Uart_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 Uart_IntrTxMask;
#endif /* (! (Uart_SCB_MODE_I2C_CONST_CFG || \
              Uart_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(Uart_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define Uart_SCB_MODE_I2C_RUNTM_CFG     (Uart_SCB_MODE_I2C      == Uart_scbMode)
    #define Uart_SCB_MODE_SPI_RUNTM_CFG     (Uart_SCB_MODE_SPI      == Uart_scbMode)
    #define Uart_SCB_MODE_UART_RUNTM_CFG    (Uart_SCB_MODE_UART     == Uart_scbMode)
    #define Uart_SCB_MODE_EZI2C_RUNTM_CFG   (Uart_SCB_MODE_EZI2C    == Uart_scbMode)
    #define Uart_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (Uart_SCB_MODE_UNCONFIG == Uart_scbMode)

    /* Defines wakeup enable */
    #define Uart_SCB_WAKE_ENABLE_CHECK       (0u != Uart_scbEnableWake)
#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!Uart_CY_SCBIP_V1)
    #define Uart_SCB_PINS_NUMBER    (7u)
#else
    #define Uart_SCB_PINS_NUMBER    (2u)
#endif /* (!Uart_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_Uart_H) */


/* [] END OF FILE */
