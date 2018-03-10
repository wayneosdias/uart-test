/*******************************************************************************
* File Name: Voltage.h  
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

#if !defined(CY_PINS_Voltage_ALIASES_H) /* Pins Voltage_ALIASES_H */
#define CY_PINS_Voltage_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Voltage_0			(Voltage__0__PC)
#define Voltage_0_PS		(Voltage__0__PS)
#define Voltage_0_PC		(Voltage__0__PC)
#define Voltage_0_DR		(Voltage__0__DR)
#define Voltage_0_SHIFT	(Voltage__0__SHIFT)
#define Voltage_0_INTR	((uint16)((uint16)0x0003u << (Voltage__0__SHIFT*2u)))

#define Voltage_INTR_ALL	 ((uint16)(Voltage_0_INTR))


#endif /* End Pins Voltage_ALIASES_H */


/* [] END OF FILE */
