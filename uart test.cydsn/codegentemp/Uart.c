/***************************************************************************//**
* \file Uart.c
* \version 3.20
*
* \brief
*  This file provides the source code to the API for the SCB Component.
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

#if (Uart_SCB_MODE_I2C_INC)
    #include "Uart_I2C_PVT.h"
#endif /* (Uart_SCB_MODE_I2C_INC) */

#if (Uart_SCB_MODE_EZI2C_INC)
    #include "Uart_EZI2C_PVT.h"
#endif /* (Uart_SCB_MODE_EZI2C_INC) */

#if (Uart_SCB_MODE_SPI_INC || Uart_SCB_MODE_UART_INC)
    #include "Uart_SPI_UART_PVT.h"
#endif /* (Uart_SCB_MODE_SPI_INC || Uart_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if (Uart_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 Uart_scbMode = Uart_SCB_MODE_UNCONFIG;
    uint8 Uart_scbEnableWake;
    uint8 Uart_scbEnableIntr;

    /* I2C configuration variables */
    uint8 Uart_mode;
    uint8 Uart_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * Uart_rxBuffer;
    uint8  Uart_rxDataBits;
    uint32 Uart_rxBufferSize;

    volatile uint8 * Uart_txBuffer;
    uint8  Uart_txDataBits;
    uint32 Uart_txBufferSize;

    /* EZI2C configuration variables */
    uint8 Uart_numberOfAddr;
    uint8 Uart_subAddrSize;
#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/
/**
* \addtogroup group_general
* \{
*/

/** Uart_initVar indicates whether the Uart 
*  component has been initialized. The variable is initialized to 0 
*  and set to 1 the first time SCB_Start() is called. This allows 
*  the component to restart without reinitialization after the first 
*  call to the Uart_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  Uart_Init() function can be called before the 
*  Uart_Start() or Uart_Enable() function.
*/
uint8 Uart_initVar = 0u;


#if (! (Uart_SCB_MODE_I2C_CONST_CFG || \
        Uart_SCB_MODE_EZI2C_CONST_CFG))
    /** This global variable stores TX interrupt sources after 
    * Uart_Stop() is called. Only these TX interrupt sources 
    * will be restored on a subsequent Uart_Enable() call.
    */
    uint16 Uart_IntrTxMask = 0u;
#endif /* (! (Uart_SCB_MODE_I2C_CONST_CFG || \
              Uart_SCB_MODE_EZI2C_CONST_CFG)) */
/** \} globals */

#if (Uart_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_Uart_CUSTOM_INTR_HANDLER)
    void (*Uart_customIntrHandler)(void) = NULL;
#endif /* !defined (CY_REMOVE_Uart_CUSTOM_INTR_HANDLER) */
#endif /* (Uart_SCB_IRQ_INTERNAL) */


/***************************************
*    Private Function Prototypes
***************************************/

static void Uart_ScbEnableIntr(void);
static void Uart_ScbModeStop(void);
static void Uart_ScbModePostEnable(void);


/*******************************************************************************
* Function Name: Uart_Init
****************************************************************************//**
*
*  Initializes the Uart component to operate in one of the selected
*  configurations: I2C, SPI, UART or EZI2C.
*  When the configuration is set to "Unconfigured SCB", this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  Uart_I2CInit, Uart_SpiInit, 
*  Uart_UartInit or Uart_EzI2CInit.
*
*******************************************************************************/
void Uart_Init(void)
{
#if (Uart_SCB_MODE_UNCONFIG_CONST_CFG)
    if (Uart_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        Uart_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif (Uart_SCB_MODE_I2C_CONST_CFG)
    Uart_I2CInit();

#elif (Uart_SCB_MODE_SPI_CONST_CFG)
    Uart_SpiInit();

#elif (Uart_SCB_MODE_UART_CONST_CFG)
    Uart_UartInit();

#elif (Uart_SCB_MODE_EZI2C_CONST_CFG)
    Uart_EzI2CInit();

#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: Uart_Enable
****************************************************************************//**
*
*  Enables Uart component operation: activates the hardware and 
*  internal interrupt. It also restores TX interrupt sources disabled after the 
*  Uart_Stop() function was called (note that level-triggered TX 
*  interrupt sources remain disabled to not cause code lock-up).
*  For I2C and EZI2C modes the interrupt is internal and mandatory for 
*  operation. For SPI and UART modes the interrupt can be configured as none, 
*  internal or external.
*  The Uart configuration should be not changed when the component
*  is enabled. Any configuration changes should be made after disabling the 
*  component.
*  When configuration is set to “Unconfigured Uart”, the component 
*  must first be initialized to operate in one of the following configurations: 
*  I2C, SPI, UART or EZ I2C, using the mode-specific initialization API. 
*  Otherwise this function does not enable the component.
*
*******************************************************************************/
void Uart_Enable(void)
{
#if (Uart_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if (!Uart_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        Uart_CTRL_REG |= Uart_CTRL_ENABLED;

        Uart_ScbEnableIntr();

        /* Call PostEnable function specific to current operation mode */
        Uart_ScbModePostEnable();
    }
#else
    Uart_CTRL_REG |= Uart_CTRL_ENABLED;

    Uart_ScbEnableIntr();

    /* Call PostEnable function specific to current operation mode */
    Uart_ScbModePostEnable();
#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: Uart_Start
****************************************************************************//**
*
*  Invokes Uart_Init() and Uart_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to "Unconfigured SCB", the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZI2C. Otherwise this function does not enable the component.
*
* \globalvars
*  Uart_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void Uart_Start(void)
{
    if (0u == Uart_initVar)
    {
        Uart_Init();
        Uart_initVar = 1u; /* Component was initialized */
    }

    Uart_Enable();
}


/*******************************************************************************
* Function Name: Uart_Stop
****************************************************************************//**
*
*  Disables the Uart component: disable the hardware and internal 
*  interrupt. It also disables all TX interrupt sources so as not to cause an 
*  unexpected interrupt trigger because after the component is enabled, the 
*  TX FIFO is empty.
*  Refer to the function Uart_Enable() for the interrupt 
*  configuration details.
*  This function disables the SCB component without checking to see if 
*  communication is in progress. Before calling this function it may be 
*  necessary to check the status of communication to make sure communication 
*  is complete. If this is not done then communication could be stopped mid 
*  byte and corrupted data could result.
*
*******************************************************************************/
void Uart_Stop(void)
{
#if (Uart_SCB_IRQ_INTERNAL)
    Uart_DisableInt();
#endif /* (Uart_SCB_IRQ_INTERNAL) */

    /* Call Stop function specific to current operation mode */
    Uart_ScbModeStop();

    /* Disable SCB IP */
    Uart_CTRL_REG &= (uint32) ~Uart_CTRL_ENABLED;

    /* Disable all TX interrupt sources so as not to cause an unexpected
    * interrupt trigger after the component will be enabled because the 
    * TX FIFO is empty.
    * For SCB IP v0, it is critical as it does not mask-out interrupt
    * sources when it is disabled. This can cause a code lock-up in the
    * interrupt handler because TX FIFO cannot be loaded after the block
    * is disabled.
    */
    Uart_SetTxInterruptMode(Uart_NO_INTR_SOURCES);

#if (Uart_SCB_IRQ_INTERNAL)
    Uart_ClearPendingInt();
#endif /* (Uart_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: Uart_SetRxFifoLevel
****************************************************************************//**
*
*  Sets level in the RX FIFO to generate a RX level interrupt.
*  When the RX FIFO has more entries than the RX FIFO level an RX level
*  interrupt request is generated.
*
*  \param level: Level in the RX FIFO to generate RX level interrupt.
*   The range of valid level values is between 0 and RX FIFO depth - 1.
*
*******************************************************************************/
void Uart_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = Uart_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~Uart_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (Uart_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    Uart_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: Uart_SetTxFifoLevel
****************************************************************************//**
*
*  Sets level in the TX FIFO to generate a TX level interrupt.
*  When the TX FIFO has less entries than the TX FIFO level an TX level
*  interrupt request is generated.
*
*  \param level: Level in the TX FIFO to generate TX level interrupt.
*   The range of valid level values is between 0 and TX FIFO depth - 1.
*
*******************************************************************************/
void Uart_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = Uart_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~Uart_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (Uart_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    Uart_TX_FIFO_CTRL_REG = txFifoCtrl;
}


#if (Uart_SCB_IRQ_INTERNAL)
    /*******************************************************************************
    * Function Name: Uart_SetCustomInterruptHandler
    ****************************************************************************//**
    *
    *  Registers a function to be called by the internal interrupt handler.
    *  First the function that is registered is called, then the internal interrupt
    *  handler performs any operation such as software buffer management functions
    *  before the interrupt returns.  It is the user's responsibility not to break
    *  the software buffer operations. Only one custom handler is supported, which
    *  is the function provided by the most recent call.
    *  At the initialization time no custom handler is registered.
    *
    *  \param func: Pointer to the function to register.
    *        The value NULL indicates to remove the current custom interrupt
    *        handler.
    *
    *******************************************************************************/
    void Uart_SetCustomInterruptHandler(void (*func)(void))
    {
    #if !defined (CY_REMOVE_Uart_CUSTOM_INTR_HANDLER)
        Uart_customIntrHandler = func; /* Register interrupt handler */
    #else
        if (NULL != func)
        {
            /* Suppress compiler warning */
        }
    #endif /* !defined (CY_REMOVE_Uart_CUSTOM_INTR_HANDLER) */
    }
#endif /* (Uart_SCB_IRQ_INTERNAL) */


/*******************************************************************************
* Function Name: Uart_ScbModeEnableIntr
****************************************************************************//**
*
*  Enables an interrupt for a specific mode.
*
*******************************************************************************/
static void Uart_ScbEnableIntr(void)
{
#if (Uart_SCB_IRQ_INTERNAL)
    /* Enable interrupt in NVIC */
    #if (Uart_SCB_MODE_UNCONFIG_CONST_CFG)
        if (0u != Uart_scbEnableIntr)
        {
            Uart_EnableInt();
        }

    #else
        Uart_EnableInt();

    #endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (Uart_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: Uart_ScbModePostEnable
****************************************************************************//**
*
*  Calls the PostEnable function for a specific operation mode.
*
*******************************************************************************/
static void Uart_ScbModePostEnable(void)
{
#if (Uart_SCB_MODE_UNCONFIG_CONST_CFG)
#if (!Uart_CY_SCBIP_V1)
    if (Uart_SCB_MODE_SPI_RUNTM_CFG)
    {
        Uart_SpiPostEnable();
    }
    else if (Uart_SCB_MODE_UART_RUNTM_CFG)
    {
        Uart_UartPostEnable();
    }
    else
    {
        /* Unknown mode: do nothing */
    }
#endif /* (!Uart_CY_SCBIP_V1) */

#elif (Uart_SCB_MODE_SPI_CONST_CFG)
    Uart_SpiPostEnable();

#elif (Uart_SCB_MODE_UART_CONST_CFG)
    Uart_UartPostEnable();

#else
    /* Unknown mode: do nothing */
#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: Uart_ScbModeStop
****************************************************************************//**
*
*  Calls the Stop function for a specific operation mode.
*
*******************************************************************************/
static void Uart_ScbModeStop(void)
{
#if (Uart_SCB_MODE_UNCONFIG_CONST_CFG)
    if (Uart_SCB_MODE_I2C_RUNTM_CFG)
    {
        Uart_I2CStop();
    }
    else if (Uart_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        Uart_EzI2CStop();
    }
#if (!Uart_CY_SCBIP_V1)
    else if (Uart_SCB_MODE_SPI_RUNTM_CFG)
    {
        Uart_SpiStop();
    }
    else if (Uart_SCB_MODE_UART_RUNTM_CFG)
    {
        Uart_UartStop();
    }
#endif /* (!Uart_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
#elif (Uart_SCB_MODE_I2C_CONST_CFG)
    Uart_I2CStop();

#elif (Uart_SCB_MODE_EZI2C_CONST_CFG)
    Uart_EzI2CStop();

#elif (Uart_SCB_MODE_SPI_CONST_CFG)
    Uart_SpiStop();

#elif (Uart_SCB_MODE_UART_CONST_CFG)
    Uart_UartStop();

#else
    /* Unknown mode: do nothing */
#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (Uart_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: Uart_SetPins
    ****************************************************************************//**
    *
    *  Sets the pins settings accordingly to the selected operation mode.
    *  Only available in the Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when a specific mode of operation
    *  is selected in design time.
    *
    *  \param mode:      Mode of SCB operation.
    *  \param subMode:   Sub-mode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  \param uartEnableMask: enables TX or RX direction and RTS and CTS signals.
    *
    *******************************************************************************/
    void Uart_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 pinsDm[Uart_SCB_PINS_NUMBER];
        uint32 i;
        
    #if (!Uart_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!Uart_CY_SCBIP_V1) */
        
        uint32 hsiomSel[Uart_SCB_PINS_NUMBER] = 
        {
            Uart_RX_SDA_MOSI_HSIOM_SEL_GPIO,
            Uart_TX_SCL_MISO_HSIOM_SEL_GPIO,
            0u,
            0u,
            0u,
            0u,
            0u,
        };

    #if (Uart_CY_SCBIP_V1)
        /* Supress compiler warning. */
        if ((0u == subMode) || (0u == uartEnableMask))
        {
        }
    #endif /* (Uart_CY_SCBIP_V1) */

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for (i = 0u; i < Uart_SCB_PINS_NUMBER; i++)
        {
            pinsDm[i] = Uart_PIN_DM_ALG_HIZ;
        }

        if ((Uart_SCB_MODE_I2C   == mode) ||
            (Uart_SCB_MODE_EZI2C == mode))
        {
        #if (Uart_RX_SDA_MOSI_PIN)
            hsiomSel[Uart_RX_SDA_MOSI_PIN_INDEX] = Uart_RX_SDA_MOSI_HSIOM_SEL_I2C;
            pinsDm  [Uart_RX_SDA_MOSI_PIN_INDEX] = Uart_PIN_DM_OD_LO;
        #elif (Uart_RX_WAKE_SDA_MOSI_PIN)
            hsiomSel[Uart_RX_WAKE_SDA_MOSI_PIN_INDEX] = Uart_RX_WAKE_SDA_MOSI_HSIOM_SEL_I2C;
            pinsDm  [Uart_RX_WAKE_SDA_MOSI_PIN_INDEX] = Uart_PIN_DM_OD_LO;
        #else
        #endif /* (Uart_RX_SDA_MOSI_PIN) */
        
        #if (Uart_TX_SCL_MISO_PIN)
            hsiomSel[Uart_TX_SCL_MISO_PIN_INDEX] = Uart_TX_SCL_MISO_HSIOM_SEL_I2C;
            pinsDm  [Uart_TX_SCL_MISO_PIN_INDEX] = Uart_PIN_DM_OD_LO;
        #endif /* (Uart_TX_SCL_MISO_PIN) */
        }
    #if (!Uart_CY_SCBIP_V1)
        else if (Uart_SCB_MODE_SPI == mode)
        {
        #if (Uart_RX_SDA_MOSI_PIN)
            hsiomSel[Uart_RX_SDA_MOSI_PIN_INDEX] = Uart_RX_SDA_MOSI_HSIOM_SEL_SPI;
        #elif (Uart_RX_WAKE_SDA_MOSI_PIN)
            hsiomSel[Uart_RX_WAKE_SDA_MOSI_PIN_INDEX] = Uart_RX_WAKE_SDA_MOSI_HSIOM_SEL_SPI;
        #else
        #endif /* (Uart_RX_SDA_MOSI_PIN) */
        
        #if (Uart_TX_SCL_MISO_PIN)
            hsiomSel[Uart_TX_SCL_MISO_PIN_INDEX] = Uart_TX_SCL_MISO_HSIOM_SEL_SPI;
        #endif /* (Uart_TX_SCL_MISO_PIN) */
        
        #if (Uart_CTS_SCLK_PIN)
            hsiomSel[Uart_CTS_SCLK_PIN_INDEX] = Uart_CTS_SCLK_HSIOM_SEL_SPI;
        #endif /* (Uart_CTS_SCLK_PIN) */

            if (Uart_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[Uart_RX_SDA_MOSI_PIN_INDEX] = Uart_PIN_DM_DIG_HIZ;
                pinsDm[Uart_TX_SCL_MISO_PIN_INDEX] = Uart_PIN_DM_STRONG;
                pinsDm[Uart_CTS_SCLK_PIN_INDEX] = Uart_PIN_DM_DIG_HIZ;

            #if (Uart_RTS_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[Uart_RTS_SS0_PIN_INDEX] = Uart_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm  [Uart_RTS_SS0_PIN_INDEX] = Uart_PIN_DM_DIG_HIZ;
            #endif /* (Uart_RTS_SS0_PIN) */

            #if (Uart_TX_SCL_MISO_PIN)
                /* Disable input buffer */
                 pinsInBuf |= Uart_TX_SCL_MISO_PIN_MASK;
            #endif /* (Uart_TX_SCL_MISO_PIN) */
            }
            else 
            {
                /* (Master) */
                pinsDm[Uart_RX_SDA_MOSI_PIN_INDEX] = Uart_PIN_DM_STRONG;
                pinsDm[Uart_TX_SCL_MISO_PIN_INDEX] = Uart_PIN_DM_DIG_HIZ;
                pinsDm[Uart_CTS_SCLK_PIN_INDEX] = Uart_PIN_DM_STRONG;

            #if (Uart_RTS_SS0_PIN)
                hsiomSel [Uart_RTS_SS0_PIN_INDEX] = Uart_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm   [Uart_RTS_SS0_PIN_INDEX] = Uart_PIN_DM_STRONG;
                pinsInBuf |= Uart_RTS_SS0_PIN_MASK;
            #endif /* (Uart_RTS_SS0_PIN) */

            #if (Uart_SS1_PIN)
                hsiomSel [Uart_SS1_PIN_INDEX] = Uart_SS1_HSIOM_SEL_SPI;
                pinsDm   [Uart_SS1_PIN_INDEX] = Uart_PIN_DM_STRONG;
                pinsInBuf |= Uart_SS1_PIN_MASK;
            #endif /* (Uart_SS1_PIN) */

            #if (Uart_SS2_PIN)
                hsiomSel [Uart_SS2_PIN_INDEX] = Uart_SS2_HSIOM_SEL_SPI;
                pinsDm   [Uart_SS2_PIN_INDEX] = Uart_PIN_DM_STRONG;
                pinsInBuf |= Uart_SS2_PIN_MASK;
            #endif /* (Uart_SS2_PIN) */

            #if (Uart_SS3_PIN)
                hsiomSel [Uart_SS3_PIN_INDEX] = Uart_SS3_HSIOM_SEL_SPI;
                pinsDm   [Uart_SS3_PIN_INDEX] = Uart_PIN_DM_STRONG;
                pinsInBuf |= Uart_SS3_PIN_MASK;
            #endif /* (Uart_SS3_PIN) */

                /* Disable input buffers */
            #if (Uart_RX_SDA_MOSI_PIN)
                pinsInBuf |= Uart_RX_SDA_MOSI_PIN_MASK;
            #elif (Uart_RX_WAKE_SDA_MOSI_PIN)
                pinsInBuf |= Uart_RX_WAKE_SDA_MOSI_PIN_MASK;
            #else
            #endif /* (Uart_RX_SDA_MOSI_PIN) */

            #if (Uart_CTS_SCLK_PIN)
                pinsInBuf |= Uart_CTS_SCLK_PIN_MASK;
            #endif /* (Uart_CTS_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if (Uart_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
            #if (Uart_TX_SCL_MISO_PIN)
                hsiomSel[Uart_TX_SCL_MISO_PIN_INDEX] = Uart_TX_SCL_MISO_HSIOM_SEL_UART;
                pinsDm  [Uart_TX_SCL_MISO_PIN_INDEX] = Uart_PIN_DM_OD_LO;
            #endif /* (Uart_TX_SCL_MISO_PIN) */
            }
            else /* Standard or IrDA */
            {
                if (0u != (Uart_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                #if (Uart_RX_SDA_MOSI_PIN)
                    hsiomSel[Uart_RX_SDA_MOSI_PIN_INDEX] = Uart_RX_SDA_MOSI_HSIOM_SEL_UART;
                    pinsDm  [Uart_RX_SDA_MOSI_PIN_INDEX] = Uart_PIN_DM_DIG_HIZ;
                #elif (Uart_RX_WAKE_SDA_MOSI_PIN)
                    hsiomSel[Uart_RX_WAKE_SDA_MOSI_PIN_INDEX] = Uart_RX_WAKE_SDA_MOSI_HSIOM_SEL_UART;
                    pinsDm  [Uart_RX_WAKE_SDA_MOSI_PIN_INDEX] = Uart_PIN_DM_DIG_HIZ;
                #else
                #endif /* (Uart_RX_SDA_MOSI_PIN) */
                }

                if (0u != (Uart_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                #if (Uart_TX_SCL_MISO_PIN)
                    hsiomSel[Uart_TX_SCL_MISO_PIN_INDEX] = Uart_TX_SCL_MISO_HSIOM_SEL_UART;
                    pinsDm  [Uart_TX_SCL_MISO_PIN_INDEX] = Uart_PIN_DM_STRONG;
                    
                    /* Disable input buffer */
                    pinsInBuf |= Uart_TX_SCL_MISO_PIN_MASK;
                #endif /* (Uart_TX_SCL_MISO_PIN) */
                }

            #if !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1)
                if (Uart_UART_MODE_STD == subMode)
                {
                    if (0u != (Uart_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                    #if (Uart_CTS_SCLK_PIN)
                        hsiomSel[Uart_CTS_SCLK_PIN_INDEX] = Uart_CTS_SCLK_HSIOM_SEL_UART;
                        pinsDm  [Uart_CTS_SCLK_PIN_INDEX] = Uart_PIN_DM_DIG_HIZ;
                    #endif /* (Uart_CTS_SCLK_PIN) */
                    }

                    if (0u != (Uart_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                    #if (Uart_RTS_SS0_PIN)
                        hsiomSel[Uart_RTS_SS0_PIN_INDEX] = Uart_RTS_SS0_HSIOM_SEL_UART;
                        pinsDm  [Uart_RTS_SS0_PIN_INDEX] = Uart_PIN_DM_STRONG;
                        
                        /* Disable input buffer */
                        pinsInBuf |= Uart_RTS_SS0_PIN_MASK;
                    #endif /* (Uart_RTS_SS0_PIN) */
                    }
                }
            #endif /* !(Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1) */
            }
        }
    #endif /* (!Uart_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if (Uart_RX_SDA_MOSI_PIN)
        Uart_SET_HSIOM_SEL(Uart_RX_SDA_MOSI_HSIOM_REG,
                                       Uart_RX_SDA_MOSI_HSIOM_MASK,
                                       Uart_RX_SDA_MOSI_HSIOM_POS,
                                        hsiomSel[Uart_RX_SDA_MOSI_PIN_INDEX]);

        Uart_uart_rx_i2c_sda_spi_mosi_SetDriveMode((uint8) pinsDm[Uart_RX_SDA_MOSI_PIN_INDEX]);

        #if (!Uart_CY_SCBIP_V1)
            Uart_SET_INP_DIS(Uart_uart_rx_i2c_sda_spi_mosi_INP_DIS,
                                         Uart_uart_rx_i2c_sda_spi_mosi_MASK,
                                         (0u != (pinsInBuf & Uart_RX_SDA_MOSI_PIN_MASK)));
        #endif /* (!Uart_CY_SCBIP_V1) */
    
    #elif (Uart_RX_WAKE_SDA_MOSI_PIN)
        Uart_SET_HSIOM_SEL(Uart_RX_WAKE_SDA_MOSI_HSIOM_REG,
                                       Uart_RX_WAKE_SDA_MOSI_HSIOM_MASK,
                                       Uart_RX_WAKE_SDA_MOSI_HSIOM_POS,
                                       hsiomSel[Uart_RX_WAKE_SDA_MOSI_PIN_INDEX]);

        Uart_uart_rx_wake_i2c_sda_spi_mosi_SetDriveMode((uint8)
                                                               pinsDm[Uart_RX_WAKE_SDA_MOSI_PIN_INDEX]);

        Uart_SET_INP_DIS(Uart_uart_rx_wake_i2c_sda_spi_mosi_INP_DIS,
                                     Uart_uart_rx_wake_i2c_sda_spi_mosi_MASK,
                                     (0u != (pinsInBuf & Uart_RX_WAKE_SDA_MOSI_PIN_MASK)));

         /* Set interrupt on falling edge */
        Uart_SET_INCFG_TYPE(Uart_RX_WAKE_SDA_MOSI_INTCFG_REG,
                                        Uart_RX_WAKE_SDA_MOSI_INTCFG_TYPE_MASK,
                                        Uart_RX_WAKE_SDA_MOSI_INTCFG_TYPE_POS,
                                        Uart_INTCFG_TYPE_FALLING_EDGE);
    #else
    #endif /* (Uart_RX_WAKE_SDA_MOSI_PIN) */

    #if (Uart_TX_SCL_MISO_PIN)
        Uart_SET_HSIOM_SEL(Uart_TX_SCL_MISO_HSIOM_REG,
                                       Uart_TX_SCL_MISO_HSIOM_MASK,
                                       Uart_TX_SCL_MISO_HSIOM_POS,
                                        hsiomSel[Uart_TX_SCL_MISO_PIN_INDEX]);

        Uart_uart_tx_i2c_scl_spi_miso_SetDriveMode((uint8) pinsDm[Uart_TX_SCL_MISO_PIN_INDEX]);

    #if (!Uart_CY_SCBIP_V1)
        Uart_SET_INP_DIS(Uart_uart_tx_i2c_scl_spi_miso_INP_DIS,
                                     Uart_uart_tx_i2c_scl_spi_miso_MASK,
                                    (0u != (pinsInBuf & Uart_TX_SCL_MISO_PIN_MASK)));
    #endif /* (!Uart_CY_SCBIP_V1) */
    #endif /* (Uart_RX_SDA_MOSI_PIN) */

    #if (Uart_CTS_SCLK_PIN)
        Uart_SET_HSIOM_SEL(Uart_CTS_SCLK_HSIOM_REG,
                                       Uart_CTS_SCLK_HSIOM_MASK,
                                       Uart_CTS_SCLK_HSIOM_POS,
                                       hsiomSel[Uart_CTS_SCLK_PIN_INDEX]);

        Uart_uart_cts_spi_sclk_SetDriveMode((uint8) pinsDm[Uart_CTS_SCLK_PIN_INDEX]);

        Uart_SET_INP_DIS(Uart_uart_cts_spi_sclk_INP_DIS,
                                     Uart_uart_cts_spi_sclk_MASK,
                                     (0u != (pinsInBuf & Uart_CTS_SCLK_PIN_MASK)));
    #endif /* (Uart_CTS_SCLK_PIN) */

    #if (Uart_RTS_SS0_PIN)
        Uart_SET_HSIOM_SEL(Uart_RTS_SS0_HSIOM_REG,
                                       Uart_RTS_SS0_HSIOM_MASK,
                                       Uart_RTS_SS0_HSIOM_POS,
                                       hsiomSel[Uart_RTS_SS0_PIN_INDEX]);

        Uart_uart_rts_spi_ss0_SetDriveMode((uint8) pinsDm[Uart_RTS_SS0_PIN_INDEX]);

        Uart_SET_INP_DIS(Uart_uart_rts_spi_ss0_INP_DIS,
                                     Uart_uart_rts_spi_ss0_MASK,
                                     (0u != (pinsInBuf & Uart_RTS_SS0_PIN_MASK)));
    #endif /* (Uart_RTS_SS0_PIN) */

    #if (Uart_SS1_PIN)
        Uart_SET_HSIOM_SEL(Uart_SS1_HSIOM_REG,
                                       Uart_SS1_HSIOM_MASK,
                                       Uart_SS1_HSIOM_POS,
                                       hsiomSel[Uart_SS1_PIN_INDEX]);

        Uart_spi_ss1_SetDriveMode((uint8) pinsDm[Uart_SS1_PIN_INDEX]);

        Uart_SET_INP_DIS(Uart_spi_ss1_INP_DIS,
                                     Uart_spi_ss1_MASK,
                                     (0u != (pinsInBuf & Uart_SS1_PIN_MASK)));
    #endif /* (Uart_SS1_PIN) */

    #if (Uart_SS2_PIN)
        Uart_SET_HSIOM_SEL(Uart_SS2_HSIOM_REG,
                                       Uart_SS2_HSIOM_MASK,
                                       Uart_SS2_HSIOM_POS,
                                       hsiomSel[Uart_SS2_PIN_INDEX]);

        Uart_spi_ss2_SetDriveMode((uint8) pinsDm[Uart_SS2_PIN_INDEX]);

        Uart_SET_INP_DIS(Uart_spi_ss2_INP_DIS,
                                     Uart_spi_ss2_MASK,
                                     (0u != (pinsInBuf & Uart_SS2_PIN_MASK)));
    #endif /* (Uart_SS2_PIN) */

    #if (Uart_SS3_PIN)
        Uart_SET_HSIOM_SEL(Uart_SS3_HSIOM_REG,
                                       Uart_SS3_HSIOM_MASK,
                                       Uart_SS3_HSIOM_POS,
                                       hsiomSel[Uart_SS3_PIN_INDEX]);

        Uart_spi_ss3_SetDriveMode((uint8) pinsDm[Uart_SS3_PIN_INDEX]);

        Uart_SET_INP_DIS(Uart_spi_ss3_INP_DIS,
                                     Uart_spi_ss3_MASK,
                                     (0u != (pinsInBuf & Uart_SS3_PIN_MASK)));
    #endif /* (Uart_SS3_PIN) */
    }

#endif /* (Uart_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: Uart_I2CSlaveNackGeneration
    ****************************************************************************//**
    *
    *  Sets command to generate NACK to the address or data.
    *
    *******************************************************************************/
    void Uart_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (Uart_CTRL_REG & Uart_CTRL_EC_AM_MODE)) &&
            (0u == (Uart_I2C_CTRL_REG & Uart_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            Uart_CTRL_REG &= ~Uart_CTRL_EC_AM_MODE;
            Uart_CTRL_REG |=  Uart_CTRL_EC_AM_MODE;
        }

        Uart_I2C_SLAVE_CMD_REG = Uart_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (Uart_CY_SCBIP_V0 || Uart_CY_SCBIP_V1) */


/* [] END OF FILE */
