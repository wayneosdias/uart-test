/*******************************************************************************
* File Name: Pwm_Clk.h
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

#if !defined(CY_CLOCK_Pwm_Clk_H)
#define CY_CLOCK_Pwm_Clk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Pwm_Clk_StartEx(uint32 alignClkDiv);
#define Pwm_Clk_Start() \
    Pwm_Clk_StartEx(Pwm_Clk__PA_DIV_ID)

#else

void Pwm_Clk_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Pwm_Clk_Stop(void);

void Pwm_Clk_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Pwm_Clk_GetDividerRegister(void);
uint8  Pwm_Clk_GetFractionalDividerRegister(void);

#define Pwm_Clk_Enable()                         Pwm_Clk_Start()
#define Pwm_Clk_Disable()                        Pwm_Clk_Stop()
#define Pwm_Clk_SetDividerRegister(clkDivider, reset)  \
    Pwm_Clk_SetFractionalDividerRegister((clkDivider), 0u)
#define Pwm_Clk_SetDivider(clkDivider)           Pwm_Clk_SetDividerRegister((clkDivider), 1u)
#define Pwm_Clk_SetDividerValue(clkDivider)      Pwm_Clk_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Pwm_Clk_DIV_ID     Pwm_Clk__DIV_ID

#define Pwm_Clk_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Pwm_Clk_CTRL_REG   (*(reg32 *)Pwm_Clk__CTRL_REGISTER)
#define Pwm_Clk_DIV_REG    (*(reg32 *)Pwm_Clk__DIV_REGISTER)

#define Pwm_Clk_CMD_DIV_SHIFT          (0u)
#define Pwm_Clk_CMD_PA_DIV_SHIFT       (8u)
#define Pwm_Clk_CMD_DISABLE_SHIFT      (30u)
#define Pwm_Clk_CMD_ENABLE_SHIFT       (31u)

#define Pwm_Clk_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Pwm_Clk_CMD_DISABLE_SHIFT))
#define Pwm_Clk_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Pwm_Clk_CMD_ENABLE_SHIFT))

#define Pwm_Clk_DIV_FRAC_MASK  (0x000000F8u)
#define Pwm_Clk_DIV_FRAC_SHIFT (3u)
#define Pwm_Clk_DIV_INT_MASK   (0xFFFFFF00u)
#define Pwm_Clk_DIV_INT_SHIFT  (8u)

#else 

#define Pwm_Clk_DIV_REG        (*(reg32 *)Pwm_Clk__REGISTER)
#define Pwm_Clk_ENABLE_REG     Pwm_Clk_DIV_REG
#define Pwm_Clk_DIV_FRAC_MASK  Pwm_Clk__FRAC_MASK
#define Pwm_Clk_DIV_FRAC_SHIFT (16u)
#define Pwm_Clk_DIV_INT_MASK   Pwm_Clk__DIVIDER_MASK
#define Pwm_Clk_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Pwm_Clk_H) */

/* [] END OF FILE */
