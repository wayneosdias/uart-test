/***************************************************************************//**
* \file Uart_BOOT.c
* \version 3.20
*
* \brief
*  This file provides the source code of the bootloader communication APIs
*  for the SCB Component Unconfigured mode.
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

#include "Uart_BOOT.h"

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (Uart_BTLDR_COMM_ENABLED) && \
                                (Uart_SCB_MODE_UNCONFIG_CONST_CFG)

/*******************************************************************************
* Function Name: Uart_CyBtldrCommStart
****************************************************************************//**
*
*  Starts Uart component. After this function call the component is 
*  ready for communication.
*
*******************************************************************************/
void Uart_CyBtldrCommStart(void)
{
    if (Uart_SCB_MODE_I2C_RUNTM_CFG)
    {
        Uart_I2CCyBtldrCommStart();
    }
    else if (Uart_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        Uart_EzI2CCyBtldrCommStart();
    }
#if (!Uart_CY_SCBIP_V1)
    else if (Uart_SCB_MODE_SPI_RUNTM_CFG)
    {
        Uart_SpiCyBtldrCommStart();
    }
    else if (Uart_SCB_MODE_UART_RUNTM_CFG)
    {
        Uart_UartCyBtldrCommStart();
    }
#endif /* (!Uart_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: Uart_CyBtldrCommStop
****************************************************************************//**
*
*  Stops Uart component.
*
*******************************************************************************/
void Uart_CyBtldrCommStop(void)
{
    if (Uart_SCB_MODE_I2C_RUNTM_CFG)
    {
        Uart_I2CCyBtldrCommStop();
    }
    else if (Uart_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        Uart_EzI2CCyBtldrCommStop();
    }
#if (!Uart_CY_SCBIP_V1)
    else if (Uart_SCB_MODE_SPI_RUNTM_CFG)
    {
        Uart_SpiCyBtldrCommStop();
    }
    else if (Uart_SCB_MODE_UART_RUNTM_CFG)
    {
        Uart_UartCyBtldrCommStop();
    }
#endif /* (!Uart_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: Uart_CyBtldrCommReset
****************************************************************************//**
*
*  Clears Uart component buffers.
*
*******************************************************************************/
void Uart_CyBtldrCommReset(void)
{
    if(Uart_SCB_MODE_I2C_RUNTM_CFG)
    {
        Uart_I2CCyBtldrCommReset();
    }
    else if(Uart_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        Uart_EzI2CCyBtldrCommReset();
    }
#if (!Uart_CY_SCBIP_V1)
    else if(Uart_SCB_MODE_SPI_RUNTM_CFG)
    {
        Uart_SpiCyBtldrCommReset();
    }
    else if(Uart_SCB_MODE_UART_RUNTM_CFG)
    {
        Uart_UartCyBtldrCommReset();
    }
#endif /* (!Uart_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: Uart_CyBtldrCommRead
****************************************************************************//**
*
*  Allows the caller to read data from the bootloader host (the host writes the 
*  data). The function handles polling to allow a block of data to be completely
*  received from the host device.
*
*  \param pData: Pointer to storage for the block of data to be read from the
*   bootloader host.
*  \param size: Number of bytes to be read.
*  \param count: Pointer to the variable to write the number of bytes actually
*   read.
*  \param timeOut: Number of units in 10 ms to wait before returning because of a
*   timeout.
*
* \return
*  \return
*  cystatus: Returns CYRET_SUCCESS if no problem was encountered or returns the
*  value that best describes the problem. For more information refer to 
*  the “Return Codes” section of the System Reference Guide.
*
*******************************************************************************/
cystatus Uart_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    if(Uart_SCB_MODE_I2C_RUNTM_CFG)
    {
        status = Uart_I2CCyBtldrCommRead(pData, size, count, timeOut);
    }
    else if(Uart_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        status = Uart_EzI2CCyBtldrCommRead(pData, size, count, timeOut);
    }
#if (!Uart_CY_SCBIP_V1)
    else if(Uart_SCB_MODE_SPI_RUNTM_CFG)
    {
        status = Uart_SpiCyBtldrCommRead(pData, size, count, timeOut);
    }
    else if(Uart_SCB_MODE_UART_RUNTM_CFG)
    {
        status = Uart_UartCyBtldrCommRead(pData, size, count, timeOut);
    }
#endif /* (!Uart_CY_SCBIP_V1) */
    else
    {
        status = CYRET_INVALID_STATE; /* Unknown mode: return invalid status */
    }

    return(status);
}


/*******************************************************************************
* Function Name: Uart_CyBtldrCommWrite
****************************************************************************//**
*
*  Allows the caller to write data to the bootloader host (the host reads the 
*  data). The function does not use timeout and returns after data has been copied
*  into the slave read buffer. This data available to be read by the bootloader
*  host until following host data write.
*
*  \param pData: Pointer to the block of data to be written to the bootloader host.
*  \param size: Number of bytes to be written.
*  \param count: Pointer to the variable to write the number of bytes actually
*   written.
*  \param timeOut: Number of units in 10 ms to wait before returning because of a
*   timeout.
*
*  \return
*  cystatus: Returns CYRET_SUCCESS if no problem was encountered or returns the
*  value that best describes the problem. For more information refer to 
*  the “Return Codes” section of the System Reference Guide.
*
*******************************************************************************/
cystatus Uart_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    if(Uart_SCB_MODE_I2C_RUNTM_CFG)
    {
        status = Uart_I2CCyBtldrCommWrite(pData, size, count, timeOut);
    }
    else if(Uart_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        status = Uart_EzI2CCyBtldrCommWrite(pData, size, count, timeOut);
    }
#if (!Uart_CY_SCBIP_V1)
    else if(Uart_SCB_MODE_SPI_RUNTM_CFG)
    {
        status = Uart_SpiCyBtldrCommWrite(pData, size, count, timeOut);
    }
    else if(Uart_SCB_MODE_UART_RUNTM_CFG)
    {
        status = Uart_UartCyBtldrCommWrite(pData, size, count, timeOut);
    }
#endif /* (!Uart_CY_SCBIP_V1) */
    else
    {
        status = CYRET_INVALID_STATE; /* Unknown mode: return invalid status */
    }

    return(status);
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Uart_BTLDR_COMM_MODE_ENABLED) */


/* [] END OF FILE */
