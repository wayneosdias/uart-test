/***************************************************************************//**
* \file Uart_SPI_UART_PVT.h
* \version 3.20
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
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

#if !defined(CY_SCB_SPI_UART_PVT_Uart_H)
#define CY_SCB_SPI_UART_PVT_Uart_H

#include "Uart_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if (Uart_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  Uart_rxBufferHead;
    extern volatile uint32  Uart_rxBufferTail;
    
    /**
    * \addtogroup group_globals
    * @{
    */
    
    /** Sets when internal software receive buffer overflow
     *  was occurred.
    */  
    extern volatile uint8   Uart_rxBufferOverflow;
    /** @} globals */
#endif /* (Uart_INTERNAL_RX_SW_BUFFER_CONST) */

#if (Uart_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  Uart_txBufferHead;
    extern volatile uint32  Uart_txBufferTail;
#endif /* (Uart_INTERNAL_TX_SW_BUFFER_CONST) */

#if (Uart_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 Uart_rxBufferInternal[Uart_INTERNAL_RX_BUFFER_SIZE];
#endif /* (Uart_INTERNAL_RX_SW_BUFFER) */

#if (Uart_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 Uart_txBufferInternal[Uart_TX_BUFFER_SIZE];
#endif /* (Uart_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

void Uart_SpiPostEnable(void);
void Uart_SpiStop(void);

#if (Uart_SCB_MODE_SPI_CONST_CFG)
    void Uart_SpiInit(void);
#endif /* (Uart_SCB_MODE_SPI_CONST_CFG) */

#if (Uart_SPI_WAKE_ENABLE_CONST)
    void Uart_SpiSaveConfig(void);
    void Uart_SpiRestoreConfig(void);
#endif /* (Uart_SPI_WAKE_ENABLE_CONST) */

void Uart_UartPostEnable(void);
void Uart_UartStop(void);

#if (Uart_SCB_MODE_UART_CONST_CFG)
    void Uart_UartInit(void);
#endif /* (Uart_SCB_MODE_UART_CONST_CFG) */

#if (Uart_UART_WAKE_ENABLE_CONST)
    void Uart_UartSaveConfig(void);
    void Uart_UartRestoreConfig(void);
#endif /* (Uart_UART_WAKE_ENABLE_CONST) */


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in Uart_SetPins() */
#define Uart_UART_RX_PIN_ENABLE    (Uart_UART_RX)
#define Uart_UART_TX_PIN_ENABLE    (Uart_UART_TX)

/* UART RTS and CTS position to be used in  Uart_SetPins() */
#define Uart_UART_RTS_PIN_ENABLE    (0x10u)
#define Uart_UART_CTS_PIN_ENABLE    (0x20u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Interrupt processing */
#define Uart_SpiUartEnableIntRx(intSourceMask)  Uart_SetRxInterruptMode(intSourceMask)
#define Uart_SpiUartEnableIntTx(intSourceMask)  Uart_SetTxInterruptMode(intSourceMask)
uint32  Uart_SpiUartDisableIntRx(void);
uint32  Uart_SpiUartDisableIntTx(void);


#endif /* (CY_SCB_SPI_UART_PVT_Uart_H) */


/* [] END OF FILE */
