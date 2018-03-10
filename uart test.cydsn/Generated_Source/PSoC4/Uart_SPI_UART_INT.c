/***************************************************************************//**
* \file Uart_SPI_UART_INT.c
* \version 3.20
*
* \brief
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in SPI and UART modes.
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

#include "Uart_PVT.h"
#include "Uart_SPI_UART_PVT.h"


#if (Uart_SCB_IRQ_INTERNAL)
/*******************************************************************************
* Function Name: Uart_SPI_UART_ISR
****************************************************************************//**
*
*  Handles the Interrupt Service Routine for the SCB SPI or UART modes.
*
*******************************************************************************/
CY_ISR(Uart_SPI_UART_ISR)
{
#if (Uart_INTERNAL_RX_SW_BUFFER_CONST)
    uint32 locHead;
#endif /* (Uart_INTERNAL_RX_SW_BUFFER_CONST) */

#if (Uart_INTERNAL_TX_SW_BUFFER_CONST)
    uint32 locTail;
#endif /* (Uart_INTERNAL_TX_SW_BUFFER_CONST) */

#ifdef Uart_SPI_UART_ISR_ENTRY_CALLBACK
    Uart_SPI_UART_ISR_EntryCallback();
#endif /* Uart_SPI_UART_ISR_ENTRY_CALLBACK */

    if (NULL != Uart_customIntrHandler)
    {
        Uart_customIntrHandler();
    }

    #if(Uart_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        Uart_ClearSpiExtClkInterruptSource(Uart_INTR_SPI_EC_WAKE_UP);
    }
    #endif

    #if (Uart_CHECK_RX_SW_BUFFER)
    {
        if (Uart_CHECK_INTR_RX_MASKED(Uart_INTR_RX_NOT_EMPTY))
        {
            do
            {
                /* Move local head index */
                locHead = (Uart_rxBufferHead + 1u);

                /* Adjust local head index */
                if (Uart_INTERNAL_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if (locHead == Uart_rxBufferTail)
                {
                    #if (Uart_CHECK_UART_RTS_CONTROL_FLOW)
                    {
                        /* There is no space in the software buffer - disable the
                        * RX Not Empty interrupt source. The data elements are
                        * still being received into the RX FIFO until the RTS signal
                        * stops the transmitter. After the data element is read from the
                        * buffer, the RX Not Empty interrupt source is enabled to
                        * move the next data element in the software buffer.
                        */
                        Uart_INTR_RX_MASK_REG &= ~Uart_INTR_RX_NOT_EMPTY;
                        break;
                    }
                    #else
                    {
                        /* Overflow: through away received data element */
                        (void) Uart_RX_FIFO_RD_REG;
                        Uart_rxBufferOverflow = (uint8) Uart_INTR_RX_OVERFLOW;
                    }
                    #endif
                }
                else
                {
                    /* Store received data */
                    Uart_PutWordInRxBuffer(locHead, Uart_RX_FIFO_RD_REG);

                    /* Move head index */
                    Uart_rxBufferHead = locHead;
                }
            }
            while(0u != Uart_GET_RX_FIFO_ENTRIES);

            Uart_ClearRxInterruptSource(Uart_INTR_RX_NOT_EMPTY);
        }
    }
    #endif


    #if (Uart_CHECK_TX_SW_BUFFER)
    {
        if (Uart_CHECK_INTR_TX_MASKED(Uart_INTR_TX_NOT_FULL))
        {
            do
            {
                /* Check for room in TX software buffer */
                if (Uart_txBufferHead != Uart_txBufferTail)
                {
                    /* Move local tail index */
                    locTail = (Uart_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if (Uart_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    Uart_TX_FIFO_WR_REG = Uart_GetWordFromTxBuffer(locTail);

                    /* Move tail index */
                    Uart_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is empty: complete transfer */
                    Uart_DISABLE_INTR_TX(Uart_INTR_TX_NOT_FULL);
                    break;
                }
            }
            while (Uart_SPI_UART_FIFO_SIZE != Uart_GET_TX_FIFO_ENTRIES);

            Uart_ClearTxInterruptSource(Uart_INTR_TX_NOT_FULL);
        }
    }
    #endif

#ifdef Uart_SPI_UART_ISR_EXIT_CALLBACK
    Uart_SPI_UART_ISR_ExitCallback();
#endif /* Uart_SPI_UART_ISR_EXIT_CALLBACK */

}

#endif /* (Uart_SCB_IRQ_INTERNAL) */


/* [] END OF FILE */
