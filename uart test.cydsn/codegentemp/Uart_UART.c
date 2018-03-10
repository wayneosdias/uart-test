/***************************************************************************//**
* \file Uart_UART.c
* \version 3.20
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  UART mode.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Uart_PVT.h"
#include "Uart_SPI_UART_PVT.h"


#if (Uart_UART_WAKE_ENABLE_CONST && Uart_UART_RX_WAKEUP_IRQ)
    /**
    * \addtogroup group_globals
    * \{
    */
    /** This global variable determines whether to enable Skip Start
    * functionality when Uart_Sleep() function is called:
    * 0 – disable, other values – enable. Default value is 1.
    * It is only available when Enable wakeup from Deep Sleep Mode is enabled.
    */
    uint8 Uart_skipStart = 1u;
    /** \} globals */
#endif /* (Uart_UART_WAKE_ENABLE_CONST && Uart_UART_RX_WAKEUP_IRQ) */

#if(Uart_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    const Uart_UART_INIT_STRUCT Uart_configUart =
    {
        Uart_UART_SUB_MODE,
        Uart_UART_DIRECTION,
        Uart_UART_DATA_BITS_NUM,
        Uart_UART_PARITY_TYPE,
        Uart_UART_STOP_BITS_NUM,
        Uart_UART_OVS_FACTOR,
        Uart_UART_IRDA_LOW_POWER,
        Uart_UART_MEDIAN_FILTER_ENABLE,
        Uart_UART_RETRY_ON_NACK,
        Uart_UART_IRDA_POLARITY,
        Uart_UART_DROP_ON_PARITY_ERR,
        Uart_UART_DROP_ON_FRAME_ERR,
        Uart_UART_WAKE_ENABLE,
        0u,
        NULL,
        0u,
        NULL,
        Uart_UART_MP_MODE_ENABLE,
        Uart_UART_MP_ACCEPT_ADDRESS,
        Uart_UART_MP_RX_ADDRESS,
        Uart_UART_MP_RX_ADDRESS_MASK,
        (uint32) Uart_SCB_IRQ_INTERNAL,
        Uart_UART_INTR_RX_MASK,
        Uart_UART_RX_TRIGGER_LEVEL,
        Uart_UART_INTR_TX_MASK,
        Uart_UART_TX_TRIGGER_LEVEL,
        (uint8) Uart_UART_BYTE_MODE_ENABLE,
        (uint8) Uart_UART_CTS_ENABLE,
        (uint8) Uart_UART_CTS_POLARITY,
        (uint8) Uart_UART_RTS_POLARITY,
        (uint8) Uart_UART_RTS_FIFO_LEVEL
    };


    /*******************************************************************************
    * Function Name: Uart_UartInit
    ****************************************************************************//**
    *
    *  Configures the Uart for UART operation.
    *
    *  This function is intended specifically to be used when the Uart
    *  configuration is set to “Unconfigured Uart” in the customizer.
    *  After initializing the Uart in UART mode using this function,
    *  the component can be enabled using the Uart_Start() or
    * Uart_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration
    *  settings. This structure contains the same information that would otherwise
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of
    *   fields. These fields match the selections available in the customizer.
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void Uart_UartInit(const Uart_UART_INIT_STRUCT *config)
    {
        uint32 pinsConfig;

        if (NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Get direction to configure UART pins: TX, RX or TX+RX */
            pinsConfig  = config->direction;

        #if !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1)
            /* Add RTS and CTS pins to configure */
            pinsConfig |= (0u != config->rtsRxFifoLevel) ? (Uart_UART_RTS_PIN_ENABLE) : (0u);
            pinsConfig |= (0u != config->enableCts)      ? (Uart_UART_CTS_PIN_ENABLE) : (0u);
        #endif /* !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1) */

            /* Configure pins */
            Uart_SetPins(Uart_SCB_MODE_UART, config->mode, pinsConfig);

            /* Store internal configuration */
            Uart_scbMode       = (uint8) Uart_SCB_MODE_UART;
            Uart_scbEnableWake = (uint8) config->enableWake;
            Uart_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            Uart_rxBuffer      =         config->rxBuffer;
            Uart_rxDataBits    = (uint8) config->dataBits;
            Uart_rxBufferSize  = (uint8) config->rxBufferSize;

            /* Set TX direction internal variables */
            Uart_txBuffer      =         config->txBuffer;
            Uart_txDataBits    = (uint8) config->dataBits;
            Uart_txBufferSize  = (uint8) config->txBufferSize;

            /* Configure UART interface */
            if(Uart_UART_MODE_IRDA == config->mode)
            {
                /* OVS settings: IrDA */
                Uart_CTRL_REG  = ((0u != config->enableIrdaLowPower) ?
                                                (Uart_UART_GET_CTRL_OVS_IRDA_LP(config->oversample)) :
                                                (Uart_CTRL_OVS_IRDA_OVS16));
            }
            else
            {
                /* OVS settings: UART and SmartCard */
                Uart_CTRL_REG  = Uart_GET_CTRL_OVS(config->oversample);
            }

            Uart_CTRL_REG     |= Uart_GET_CTRL_BYTE_MODE  (config->enableByteMode)      |
                                             Uart_GET_CTRL_ADDR_ACCEPT(config->multiprocAcceptAddr) |
                                             Uart_CTRL_UART;

            /* Configure sub-mode: UART, SmartCard or IrDA */
            Uart_UART_CTRL_REG = Uart_GET_UART_CTRL_MODE(config->mode);

            /* Configure RX direction */
            Uart_UART_RX_CTRL_REG = Uart_GET_UART_RX_CTRL_MODE(config->stopBits)              |
                                        Uart_GET_UART_RX_CTRL_POLARITY(config->enableInvertedRx)          |
                                        Uart_GET_UART_RX_CTRL_MP_MODE(config->enableMultiproc)            |
                                        Uart_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(config->dropOnParityErr) |
                                        Uart_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(config->dropOnFrameErr);

            if(Uart_UART_PARITY_NONE != config->parity)
            {
               Uart_UART_RX_CTRL_REG |= Uart_GET_UART_RX_CTRL_PARITY(config->parity) |
                                                    Uart_UART_RX_CTRL_PARITY_ENABLED;
            }

            Uart_RX_CTRL_REG      = Uart_GET_RX_CTRL_DATA_WIDTH(config->dataBits)       |
                                                Uart_GET_RX_CTRL_MEDIAN(config->enableMedianFilter) |
                                                Uart_GET_UART_RX_CTRL_ENABLED(config->direction);

            Uart_RX_FIFO_CTRL_REG = Uart_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure MP address */
            Uart_RX_MATCH_REG     = Uart_GET_RX_MATCH_ADDR(config->multiprocAddr) |
                                                Uart_GET_RX_MATCH_MASK(config->multiprocAddrMask);

            /* Configure RX direction */
            Uart_UART_TX_CTRL_REG = Uart_GET_UART_TX_CTRL_MODE(config->stopBits) |
                                                Uart_GET_UART_TX_CTRL_RETRY_NACK(config->enableRetryNack);

            if(Uart_UART_PARITY_NONE != config->parity)
            {
               Uart_UART_TX_CTRL_REG |= Uart_GET_UART_TX_CTRL_PARITY(config->parity) |
                                                    Uart_UART_TX_CTRL_PARITY_ENABLED;
            }

            Uart_TX_CTRL_REG      = Uart_GET_TX_CTRL_DATA_WIDTH(config->dataBits)    |
                                                Uart_GET_UART_TX_CTRL_ENABLED(config->direction);

            Uart_TX_FIFO_CTRL_REG = Uart_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

        #if !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1)
            Uart_UART_FLOW_CTRL_REG = Uart_GET_UART_FLOW_CTRL_CTS_ENABLE(config->enableCts) | \
                                            Uart_GET_UART_FLOW_CTRL_CTS_POLARITY (config->ctsPolarity)  | \
                                            Uart_GET_UART_FLOW_CTRL_RTS_POLARITY (config->rtsPolarity)  | \
                                            Uart_GET_UART_FLOW_CTRL_TRIGGER_LEVEL(config->rtsRxFifoLevel);
        #endif /* !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1) */

            /* Configure interrupt with UART handler but do not enable it */
            CyIntDisable    (Uart_ISR_NUMBER);
            CyIntSetPriority(Uart_ISR_NUMBER, Uart_ISR_PRIORITY);
            (void) CyIntSetVector(Uart_ISR_NUMBER, &Uart_SPI_UART_ISR);

            /* Configure WAKE interrupt */
        #if(Uart_UART_RX_WAKEUP_IRQ)
            CyIntDisable    (Uart_RX_WAKE_ISR_NUMBER);
            CyIntSetPriority(Uart_RX_WAKE_ISR_NUMBER, Uart_RX_WAKE_ISR_PRIORITY);
            (void) CyIntSetVector(Uart_RX_WAKE_ISR_NUMBER, &Uart_UART_WAKEUP_ISR);
        #endif /* (Uart_UART_RX_WAKEUP_IRQ) */

            /* Configure interrupt sources */
            Uart_INTR_I2C_EC_MASK_REG = Uart_NO_INTR_SOURCES;
            Uart_INTR_SPI_EC_MASK_REG = Uart_NO_INTR_SOURCES;
            Uart_INTR_SLAVE_MASK_REG  = Uart_NO_INTR_SOURCES;
            Uart_INTR_MASTER_MASK_REG = Uart_NO_INTR_SOURCES;
            Uart_INTR_RX_MASK_REG     = config->rxInterruptMask;
            Uart_INTR_TX_MASK_REG     = config->txInterruptMask;
        
            /* Configure TX interrupt sources to restore. */
            Uart_IntrTxMask = LO16(Uart_INTR_TX_MASK_REG);

            /* Clear RX buffer indexes */
            Uart_rxBufferHead     = 0u;
            Uart_rxBufferTail     = 0u;
            Uart_rxBufferOverflow = 0u;

            /* Clear TX buffer indexes */
            Uart_txBufferHead = 0u;
            Uart_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: Uart_UartInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the UART operation.
    *
    *******************************************************************************/
    void Uart_UartInit(void)
    {
        /* Configure UART interface */
        Uart_CTRL_REG = Uart_UART_DEFAULT_CTRL;

        /* Configure sub-mode: UART, SmartCard or IrDA */
        Uart_UART_CTRL_REG = Uart_UART_DEFAULT_UART_CTRL;

        /* Configure RX direction */
        Uart_UART_RX_CTRL_REG = Uart_UART_DEFAULT_UART_RX_CTRL;
        Uart_RX_CTRL_REG      = Uart_UART_DEFAULT_RX_CTRL;
        Uart_RX_FIFO_CTRL_REG = Uart_UART_DEFAULT_RX_FIFO_CTRL;
        Uart_RX_MATCH_REG     = Uart_UART_DEFAULT_RX_MATCH_REG;

        /* Configure TX direction */
        Uart_UART_TX_CTRL_REG = Uart_UART_DEFAULT_UART_TX_CTRL;
        Uart_TX_CTRL_REG      = Uart_UART_DEFAULT_TX_CTRL;
        Uart_TX_FIFO_CTRL_REG = Uart_UART_DEFAULT_TX_FIFO_CTRL;

    #if !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1)
        Uart_UART_FLOW_CTRL_REG = Uart_UART_DEFAULT_FLOW_CTRL;
    #endif /* !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1) */

        /* Configure interrupt with UART handler but do not enable it */
    #if(Uart_SCB_IRQ_INTERNAL)
        CyIntDisable    (Uart_ISR_NUMBER);
        CyIntSetPriority(Uart_ISR_NUMBER, Uart_ISR_PRIORITY);
        (void) CyIntSetVector(Uart_ISR_NUMBER, &Uart_SPI_UART_ISR);
    #endif /* (Uart_SCB_IRQ_INTERNAL) */

        /* Configure WAKE interrupt */
    #if(Uart_UART_RX_WAKEUP_IRQ)
        CyIntDisable    (Uart_RX_WAKE_ISR_NUMBER);
        CyIntSetPriority(Uart_RX_WAKE_ISR_NUMBER, Uart_RX_WAKE_ISR_PRIORITY);
        (void) CyIntSetVector(Uart_RX_WAKE_ISR_NUMBER, &Uart_UART_WAKEUP_ISR);
    #endif /* (Uart_UART_RX_WAKEUP_IRQ) */

        /* Configure interrupt sources */
        Uart_INTR_I2C_EC_MASK_REG = Uart_UART_DEFAULT_INTR_I2C_EC_MASK;
        Uart_INTR_SPI_EC_MASK_REG = Uart_UART_DEFAULT_INTR_SPI_EC_MASK;
        Uart_INTR_SLAVE_MASK_REG  = Uart_UART_DEFAULT_INTR_SLAVE_MASK;
        Uart_INTR_MASTER_MASK_REG = Uart_UART_DEFAULT_INTR_MASTER_MASK;
        Uart_INTR_RX_MASK_REG     = Uart_UART_DEFAULT_INTR_RX_MASK;
        Uart_INTR_TX_MASK_REG     = Uart_UART_DEFAULT_INTR_TX_MASK;
    
        /* Configure TX interrupt sources to restore. */
        Uart_IntrTxMask = LO16(Uart_INTR_TX_MASK_REG);

    #if(Uart_INTERNAL_RX_SW_BUFFER_CONST)
        Uart_rxBufferHead     = 0u;
        Uart_rxBufferTail     = 0u;
        Uart_rxBufferOverflow = 0u;
    #endif /* (Uart_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(Uart_INTERNAL_TX_SW_BUFFER_CONST)
        Uart_txBufferHead = 0u;
        Uart_txBufferTail = 0u;
    #endif /* (Uart_INTERNAL_TX_SW_BUFFER_CONST) */
    }
#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: Uart_UartPostEnable
****************************************************************************//**
*
*  Restores HSIOM settings for the UART output pins (TX and/or RTS) to be
*  controlled by the SCB UART.
*
*******************************************************************************/
void Uart_UartPostEnable(void)
{
#if (Uart_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (Uart_TX_SCL_MISO_PIN)
        if (Uart_CHECK_TX_SCL_MISO_PIN_USED)
        {
            /* Set SCB UART to drive the output pin */
            Uart_SET_HSIOM_SEL(Uart_TX_SCL_MISO_HSIOM_REG, Uart_TX_SCL_MISO_HSIOM_MASK,
                                           Uart_TX_SCL_MISO_HSIOM_POS, Uart_TX_SCL_MISO_HSIOM_SEL_UART);
        }
    #endif /* (Uart_TX_SCL_MISO_PIN_PIN) */

    #if !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1)
        #if (Uart_RTS_SS0_PIN)
            if (Uart_CHECK_RTS_SS0_PIN_USED)
            {
                /* Set SCB UART to drive the output pin */
                Uart_SET_HSIOM_SEL(Uart_RTS_SS0_HSIOM_REG, Uart_RTS_SS0_HSIOM_MASK,
                                               Uart_RTS_SS0_HSIOM_POS, Uart_RTS_SS0_HSIOM_SEL_UART);
            }
        #endif /* (Uart_RTS_SS0_PIN) */
    #endif /* !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1) */

#else
    #if (Uart_UART_TX_PIN)
         /* Set SCB UART to drive the output pin */
        Uart_SET_HSIOM_SEL(Uart_TX_HSIOM_REG, Uart_TX_HSIOM_MASK,
                                       Uart_TX_HSIOM_POS, Uart_TX_HSIOM_SEL_UART);
    #endif /* (Uart_UART_TX_PIN) */

    #if (Uart_UART_RTS_PIN)
        /* Set SCB UART to drive the output pin */
        Uart_SET_HSIOM_SEL(Uart_RTS_HSIOM_REG, Uart_RTS_HSIOM_MASK,
                                       Uart_RTS_HSIOM_POS, Uart_RTS_HSIOM_SEL_UART);
    #endif /* (Uart_UART_RTS_PIN) */
#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Restore TX interrupt sources. */
    Uart_SetTxInterruptMode(Uart_IntrTxMask);
}


/*******************************************************************************
* Function Name: Uart_UartStop
****************************************************************************//**
*
*  Changes the HSIOM settings for the UART output pins (TX and/or RTS) to keep
*  them inactive after the block is disabled. The output pins are controlled by
*  the GPIO data register. Also, the function disables the skip start feature
*  to not cause it to trigger after the component is enabled.
*
*******************************************************************************/
void Uart_UartStop(void)
{
#if(Uart_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (Uart_TX_SCL_MISO_PIN)
        if (Uart_CHECK_TX_SCL_MISO_PIN_USED)
        {
            /* Set GPIO to drive output pin */
            Uart_SET_HSIOM_SEL(Uart_TX_SCL_MISO_HSIOM_REG, Uart_TX_SCL_MISO_HSIOM_MASK,
                                           Uart_TX_SCL_MISO_HSIOM_POS, Uart_TX_SCL_MISO_HSIOM_SEL_GPIO);
        }
    #endif /* (Uart_TX_SCL_MISO_PIN_PIN) */

    #if !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1)
        #if (Uart_RTS_SS0_PIN)
            if (Uart_CHECK_RTS_SS0_PIN_USED)
            {
                /* Set output pin state after block is disabled */
                Uart_uart_rts_spi_ss0_Write(Uart_GET_UART_RTS_INACTIVE);

                /* Set GPIO to drive output pin */
                Uart_SET_HSIOM_SEL(Uart_RTS_SS0_HSIOM_REG, Uart_RTS_SS0_HSIOM_MASK,
                                               Uart_RTS_SS0_HSIOM_POS, Uart_RTS_SS0_HSIOM_SEL_GPIO);
            }
        #endif /* (Uart_RTS_SS0_PIN) */
    #endif /* !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1) */

#else
    #if (Uart_UART_TX_PIN)
        /* Set GPIO to drive output pin */
        Uart_SET_HSIOM_SEL(Uart_TX_HSIOM_REG, Uart_TX_HSIOM_MASK,
                                       Uart_TX_HSIOM_POS, Uart_TX_HSIOM_SEL_GPIO);
    #endif /* (Uart_UART_TX_PIN) */

    #if (Uart_UART_RTS_PIN)
        /* Set output pin state after block is disabled */
        Uart_rts_Write(Uart_GET_UART_RTS_INACTIVE);

        /* Set GPIO to drive output pin */
        Uart_SET_HSIOM_SEL(Uart_RTS_HSIOM_REG, Uart_RTS_HSIOM_MASK,
                                       Uart_RTS_HSIOM_POS, Uart_RTS_HSIOM_SEL_GPIO);
    #endif /* (Uart_UART_RTS_PIN) */

#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (Uart_UART_WAKE_ENABLE_CONST)
    /* Disable skip start feature used for wakeup */
    Uart_UART_RX_CTRL_REG &= (uint32) ~Uart_UART_RX_CTRL_SKIP_START;
#endif /* (Uart_UART_WAKE_ENABLE_CONST) */

    /* Store TX interrupt sources (exclude level triggered). */
    Uart_IntrTxMask = LO16(Uart_GetTxInterruptMode() & Uart_INTR_UART_TX_RESTORE);
}


/*******************************************************************************
* Function Name: Uart_UartSetRxAddress
****************************************************************************//**
*
*  Sets the hardware detectable receiver address for the UART in the
*  Multiprocessor mode.
*
*  \param address: Address for hardware address detection.
*
*******************************************************************************/
void Uart_UartSetRxAddress(uint32 address)
{
     uint32 matchReg;

    matchReg = Uart_RX_MATCH_REG;

    matchReg &= ((uint32) ~Uart_RX_MATCH_ADDR_MASK); /* Clear address bits */
    matchReg |= ((uint32)  (address & Uart_RX_MATCH_ADDR_MASK)); /* Set address  */

    Uart_RX_MATCH_REG = matchReg;
}


/*******************************************************************************
* Function Name: Uart_UartSetRxAddressMask
****************************************************************************//**
*
*  Sets the hardware address mask for the UART in the Multiprocessor mode.
*
*  \param addressMask: Address mask.
*   - Bit value 0 – excludes bit from address comparison.
*   - Bit value 1 – the bit needs to match with the corresponding bit
*     of the address.
*
*******************************************************************************/
void Uart_UartSetRxAddressMask(uint32 addressMask)
{
    uint32 matchReg;

    matchReg = Uart_RX_MATCH_REG;

    matchReg &= ((uint32) ~Uart_RX_MATCH_MASK_MASK); /* Clear address mask bits */
    matchReg |= ((uint32) (addressMask << Uart_RX_MATCH_MASK_POS));

    Uart_RX_MATCH_REG = matchReg;
}


#if(Uart_UART_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: Uart_UartGetChar
    ****************************************************************************//**
    *
    *  Retrieves next data element from receive buffer.
    *  This function is designed for ASCII characters and returns a char where
    *  1 to 255 are valid characters and 0 indicates an error occurred or no data
    *  is present.
    *  - RX software buffer is disabled: Returns data element retrieved from RX
    *    FIFO.
    *  - RX software buffer is enabled: Returns data element from the software
    *    receive buffer.
    *
    *  \return
    *   Next data element from the receive buffer. ASCII character values from
    *   1 to 255 are valid. A returned zero signifies an error condition or no
    *   data available.
    *
    *  \sideeffect
    *   The errors bits may not correspond with reading characters due to
    *   RX FIFO and software buffer usage.
    *   RX software buffer is enabled: The internal software buffer overflow
    *   is not treated as an error condition.
    *   Check Uart_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 Uart_UartGetChar(void)
    {
        uint32 rxData = 0u;

        /* Reads data only if there is data to read */
        if (0u != Uart_SpiUartGetRxBufferSize())
        {
            rxData = Uart_SpiUartReadRxData();
        }

        if (Uart_CHECK_INTR_RX(Uart_INTR_RX_ERR))
        {
            rxData = 0u; /* Error occurred: returns zero */
            Uart_ClearRxInterruptSource(Uart_INTR_RX_ERR);
        }

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: Uart_UartGetByte
    ****************************************************************************//**
    *
    *  Retrieves the next data element from the receive buffer, returns the
    *  received byte and error condition.
    *   - The RX software buffer is disabled: returns the data element retrieved
    *     from the RX FIFO. Undefined data will be returned if the RX FIFO is
    *     empty.
    *   - The RX software buffer is enabled: returns data element from the
    *     software receive buffer.
    *
    *  \return
    *   Bits 7-0 contain the next data element from the receive buffer and
    *   other bits contain the error condition.
    *   - Uart_UART_RX_OVERFLOW - Attempt to write to a full
    *     receiver FIFO.
    *   - Uart_UART_RX_UNDERFLOW	Attempt to read from an empty
    *     receiver FIFO.
    *   - Uart_UART_RX_FRAME_ERROR - UART framing error detected.
    *   - Uart_UART_RX_PARITY_ERROR - UART parity error detected.
    *
    *  \sideeffect
    *   The errors bits may not correspond with reading characters due to
    *   RX FIFO and software buffer usage.
    *   RX software buffer is enabled: The internal software buffer overflow
    *   is not treated as an error condition.
    *   Check Uart_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 Uart_UartGetByte(void)
    {
        uint32 rxData;
        uint32 tmpStatus;

        #if (Uart_CHECK_RX_SW_BUFFER)
        {
            Uart_DisableInt();
        }
        #endif

        if (0u != Uart_SpiUartGetRxBufferSize())
        {
            /* Enables interrupt to receive more bytes: at least one byte is in
            * buffer.
            */
            #if (Uart_CHECK_RX_SW_BUFFER)
            {
                Uart_EnableInt();
            }
            #endif

            /* Get received byte */
            rxData = Uart_SpiUartReadRxData();
        }
        else
        {
            /* Reads a byte directly from RX FIFO: underflow is raised in the
            * case of empty. Otherwise the first received byte will be read.
            */
            rxData = Uart_RX_FIFO_RD_REG;


            /* Enables interrupt to receive more bytes. */
            #if (Uart_CHECK_RX_SW_BUFFER)
            {

                /* The byte has been read from RX FIFO. Clear RX interrupt to
                * not involve interrupt handler when RX FIFO is empty.
                */
                Uart_ClearRxInterruptSource(Uart_INTR_RX_NOT_EMPTY);

                Uart_EnableInt();
            }
            #endif
        }

        /* Get and clear RX error mask */
        tmpStatus = (Uart_GetRxInterruptSource() & Uart_INTR_RX_ERR);
        Uart_ClearRxInterruptSource(Uart_INTR_RX_ERR);

        /* Puts together data and error status:
        * MP mode and accept address: 9th bit is set to notify mark.
        */
        rxData |= ((uint32) (tmpStatus << 8u));

        return (rxData);
    }


    #if !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: Uart_UartSetRtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of RTS output signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param polarity: Active polarity of RTS output signal.
        *   - Uart_UART_RTS_ACTIVE_LOW  - RTS signal is active low.
        *   - Uart_UART_RTS_ACTIVE_HIGH - RTS signal is active high.
        *
        *******************************************************************************/
        void Uart_UartSetRtsPolarity(uint32 polarity)
        {
            if(0u != polarity)
            {
                Uart_UART_FLOW_CTRL_REG |= (uint32)  Uart_UART_FLOW_CTRL_RTS_POLARITY;
            }
            else
            {
                Uart_UART_FLOW_CTRL_REG &= (uint32) ~Uart_UART_FLOW_CTRL_RTS_POLARITY;
            }
        }


        /*******************************************************************************
        * Function Name: Uart_UartSetRtsFifoLevel
        ****************************************************************************//**
        *
        *  Sets level in the RX FIFO for RTS signal activation.
        *  While the RX FIFO has fewer entries than the RX FIFO level the RTS signal
        *  remains active, otherwise the RTS signal becomes inactive.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param level: Level in the RX FIFO for RTS signal activation.
        *   The range of valid level values is between 0 and RX FIFO depth - 1.
        *   Setting level value to 0 disables RTS signal activation.
        *
        *******************************************************************************/
        void Uart_UartSetRtsFifoLevel(uint32 level)
        {
            uint32 uartFlowCtrl;

            uartFlowCtrl = Uart_UART_FLOW_CTRL_REG;

            uartFlowCtrl &= ((uint32) ~Uart_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
            uartFlowCtrl |= ((uint32) (Uart_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK & level));

            Uart_UART_FLOW_CTRL_REG = uartFlowCtrl;
        }
    #endif /* !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1) */

#endif /* (Uart_UART_RX_DIRECTION) */


#if(Uart_UART_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: Uart_UartPutString
    ****************************************************************************//**
    *
    *  Places a NULL terminated string in the transmit buffer to be sent at the
    *  next available bus time.
    *  This function is blocking and waits until there is a space available to put
    *  requested data in transmit buffer.
    *
    *  \param string: pointer to the null terminated string array to be placed in the
    *   transmit buffer.
    *
    *******************************************************************************/
    void Uart_UartPutString(const char8 string[])
    {
        uint32 bufIndex;

        bufIndex = 0u;

        /* Blocks the control flow until all data has been sent */
        while(string[bufIndex] != ((char8) 0))
        {
            Uart_UartPutChar((uint32) string[bufIndex]);
            bufIndex++;
        }
    }


    /*******************************************************************************
    * Function Name: Uart_UartPutCRLF
    ****************************************************************************//**
    *
    *  Places byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) in the transmit buffer.
    *  This function is blocking and waits until there is a space available to put
    *  all requested data in transmit buffer.
    *
    *  \param txDataByte: the data to be transmitted.
    *
    *******************************************************************************/
    void Uart_UartPutCRLF(uint32 txDataByte)
    {
        Uart_UartPutChar(txDataByte);  /* Blocks control flow until all data has been sent */
        Uart_UartPutChar(0x0Du);       /* Blocks control flow until all data has been sent */
        Uart_UartPutChar(0x0Au);       /* Blocks control flow until all data has been sent */
    }


    #if !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: UartSCB_UartEnableCts
        ****************************************************************************//**
        *
        *  Enables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void Uart_UartEnableCts(void)
        {
            Uart_UART_FLOW_CTRL_REG |= (uint32)  Uart_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: Uart_UartDisableCts
        ****************************************************************************//**
        *
        *  Disables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void Uart_UartDisableCts(void)
        {
            Uart_UART_FLOW_CTRL_REG &= (uint32) ~Uart_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: Uart_UartSetCtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of CTS input signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param polarity: Active polarity of CTS output signal.
        *   - Uart_UART_CTS_ACTIVE_LOW  - CTS signal is active low.
        *   - Uart_UART_CTS_ACTIVE_HIGH - CTS signal is active high.
        *
        *******************************************************************************/
        void Uart_UartSetCtsPolarity(uint32 polarity)
        {
            if (0u != polarity)
            {
                Uart_UART_FLOW_CTRL_REG |= (uint32)  Uart_UART_FLOW_CTRL_CTS_POLARITY;
            }
            else
            {
                Uart_UART_FLOW_CTRL_REG &= (uint32) ~Uart_UART_FLOW_CTRL_CTS_POLARITY;
            }
        }
    #endif /* !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1) */

#endif /* (Uart_UART_TX_DIRECTION) */


#if (Uart_UART_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: Uart_UartSaveConfig
    ****************************************************************************//**
    *
    *  Clears and enables an interrupt on a falling edge of the Rx input. The GPIO
    *  interrupt does not track in the active mode, therefore requires to be 
    *  cleared by this API.
    *
    *******************************************************************************/
    void Uart_UartSaveConfig(void)
    {
    #if (Uart_UART_RX_WAKEUP_IRQ)
        /* Set SKIP_START if requested (set by default). */
        if (0u != Uart_skipStart)
        {
            Uart_UART_RX_CTRL_REG |= (uint32)  Uart_UART_RX_CTRL_SKIP_START;
        }
        else
        {
            Uart_UART_RX_CTRL_REG &= (uint32) ~Uart_UART_RX_CTRL_SKIP_START;
        }
        
        /* Clear RX GPIO interrupt status and pending interrupt in NVIC because
        * falling edge on RX line occurs while UART communication in active mode.
        * Enable interrupt: next interrupt trigger should wakeup device.
        */
        Uart_CLEAR_UART_RX_WAKE_INTR;
        Uart_RxWakeClearPendingInt();
        Uart_RxWakeEnableInt();
    #endif /* (Uart_UART_RX_WAKEUP_IRQ) */
    }


    /*******************************************************************************
    * Function Name: Uart_UartRestoreConfig
    ****************************************************************************//**
    *
    *  Disables the RX GPIO interrupt. Until this function is called the interrupt
    *  remains active and triggers on every falling edge of the UART RX line.
    *
    *******************************************************************************/
    void Uart_UartRestoreConfig(void)
    {
    #if (Uart_UART_RX_WAKEUP_IRQ)
        /* Disable interrupt: no more triggers in active mode */
        Uart_RxWakeDisableInt();
    #endif /* (Uart_UART_RX_WAKEUP_IRQ) */
    }


    #if (Uart_UART_RX_WAKEUP_IRQ)
        /*******************************************************************************
        * Function Name: Uart_UART_WAKEUP_ISR
        ****************************************************************************//**
        *
        *  Handles the Interrupt Service Routine for the SCB UART mode GPIO wakeup
        *  event. This event is configured to trigger on a falling edge of the RX line.
        *
        *******************************************************************************/
        CY_ISR(Uart_UART_WAKEUP_ISR)
        {
        #ifdef Uart_UART_WAKEUP_ISR_ENTRY_CALLBACK
            Uart_UART_WAKEUP_ISR_EntryCallback();
        #endif /* Uart_UART_WAKEUP_ISR_ENTRY_CALLBACK */

            Uart_CLEAR_UART_RX_WAKE_INTR;

        #ifdef Uart_UART_WAKEUP_ISR_EXIT_CALLBACK
            Uart_UART_WAKEUP_ISR_ExitCallback();
        #endif /* Uart_UART_WAKEUP_ISR_EXIT_CALLBACK */
        }
    #endif /* (Uart_UART_RX_WAKEUP_IRQ) */
#endif /* (Uart_UART_RX_WAKEUP_IRQ) */


/* [] END OF FILE */
