/*******************************************************************************
* File Name: Adc_Sar_intClock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Adc_Sar_intClock_H)
#define CY_CLOCK_Adc_Sar_intClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Adc_Sar_intClock_StartEx(uint32 alignClkDiv);
#define Adc_Sar_intClock_Start() \
    Adc_Sar_intClock_StartEx(Adc_Sar_intClock__PA_DIV_ID)

#else

void Adc_Sar_intClock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Adc_Sar_intClock_Stop(void);

void Adc_Sar_intClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Adc_Sar_intClock_GetDividerRegister(void);
uint8  Adc_Sar_intClock_GetFractionalDividerRegister(void);

#define Adc_Sar_intClock_Enable()                         Adc_Sar_intClock_Start()
#define Adc_Sar_intClock_Disable()                        Adc_Sar_intClock_Stop()
#define Adc_Sar_intClock_SetDividerRegister(clkDivider, reset)  \
    Adc_Sar_intClock_SetFractionalDividerRegister((clkDivider), 0u)
#define Adc_Sar_intClock_SetDivider(clkDivider)           Adc_Sar_intClock_SetDividerRegister((clkDivider), 1u)
#define Adc_Sar_intClock_SetDividerValue(clkDivider)      Adc_Sar_intClock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Adc_Sar_intClock_DIV_ID     Adc_Sar_intClock__DIV_ID

#define Adc_Sar_intClock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Adc_Sar_intClock_CTRL_REG   (*(reg32 *)Adc_Sar_intClock__CTRL_REGISTER)
#define Adc_Sar_intClock_DIV_REG    (*(reg32 *)Adc_Sar_intClock__DIV_REGISTER)

#define Adc_Sar_intClock_CMD_DIV_SHIFT          (0u)
#define Adc_Sar_intClock_CMD_PA_DIV_SHIFT       (8u)
#define Adc_Sar_intClock_CMD_DISABLE_SHIFT      (30u)
#define Adc_Sar_intClock_CMD_ENABLE_SHIFT       (31u)

#define Adc_Sar_intClock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Adc_Sar_intClock_CMD_DISABLE_SHIFT))
#define Adc_Sar_intClock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Adc_Sar_intClock_CMD_ENABLE_SHIFT))

#define Adc_Sar_intClock_DIV_FRAC_MASK  (0x000000F8u)
#define Adc_Sar_intClock_DIV_FRAC_SHIFT (3u)
#define Adc_Sar_intClock_DIV_INT_MASK   (0xFFFFFF00u)
#define Adc_Sar_intClock_DIV_INT_SHIFT  (8u)

#else 

#define Adc_Sar_intClock_DIV_REG        (*(reg32 *)Adc_Sar_intClock__REGISTER)
#define Adc_Sar_intClock_ENABLE_REG     Adc_Sar_intClock_DIV_REG
#define Adc_Sar_intClock_DIV_FRAC_MASK  Adc_Sar_intClock__FRAC_MASK
#define Adc_Sar_intClock_DIV_FRAC_SHIFT (16u)
#define Adc_Sar_intClock_DIV_INT_MASK   Adc_Sar_intClock__DIVIDER_MASK
#define Adc_Sar_intClock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Adc_Sar_intClock_H) */

/* [] END OF FILE */
