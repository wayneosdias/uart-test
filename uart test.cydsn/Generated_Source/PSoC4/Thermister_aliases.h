/*******************************************************************************
* File Name: Thermister.h  
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

#if !defined(CY_PINS_Thermister_ALIASES_H) /* Pins Thermister_ALIASES_H */
#define CY_PINS_Thermister_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Thermister_0			(Thermister__0__PC)
#define Thermister_0_PS		(Thermister__0__PS)
#define Thermister_0_PC		(Thermister__0__PC)
#define Thermister_0_DR		(Thermister__0__DR)
#define Thermister_0_SHIFT	(Thermister__0__SHIFT)
#define Thermister_0_INTR	((uint16)((uint16)0x0003u << (Thermister__0__SHIFT*2u)))

#define Thermister_INTR_ALL	 ((uint16)(Thermister_0_INTR))


#endif /* End Pins Thermister_ALIASES_H */


/* [] END OF FILE */
