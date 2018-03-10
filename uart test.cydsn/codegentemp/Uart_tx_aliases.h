/*******************************************************************************
* File Name: Uart_tx.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Uart_tx_ALIASES_H) /* Pins Uart_tx_ALIASES_H */
#define CY_PINS_Uart_tx_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Uart_tx_0			(Uart_tx__0__PC)
#define Uart_tx_0_PS		(Uart_tx__0__PS)
#define Uart_tx_0_PC		(Uart_tx__0__PC)
#define Uart_tx_0_DR		(Uart_tx__0__DR)
#define Uart_tx_0_SHIFT	(Uart_tx__0__SHIFT)
#define Uart_tx_0_INTR	((uint16)((uint16)0x0003u << (Uart_tx__0__SHIFT*2u)))

#define Uart_tx_INTR_ALL	 ((uint16)(Uart_tx_0_INTR))


#endif /* End Pins Uart_tx_ALIASES_H */


/* [] END OF FILE */
