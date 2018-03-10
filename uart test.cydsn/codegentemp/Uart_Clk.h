/*******************************************************************************
* File Name: Uart_Clk.h
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

#if !defined(CY_CLOCK_Uart_Clk_H)
#define CY_CLOCK_Uart_Clk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Uart_Clk_StartEx(uint32 alignClkDiv);
#define Uart_Clk_Start() \
    Uart_Clk_StartEx(Uart_Clk__PA_DIV_ID)

#else

void Uart_Clk_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Uart_Clk_Stop(void);

void Uart_Clk_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Uart_Clk_GetDividerRegister(void);
uint8  Uart_Clk_GetFractionalDividerRegister(void);

#define Uart_Clk_Enable()                         Uart_Clk_Start()
#define Uart_Clk_Disable()                        Uart_Clk_Stop()
#define Uart_Clk_SetDividerRegister(clkDivider, reset)  \
    Uart_Clk_SetFractionalDividerRegister((clkDivider), 0u)
#define Uart_Clk_SetDivider(clkDivider)           Uart_Clk_SetDividerRegister((clkDivider), 1u)
#define Uart_Clk_SetDividerValue(clkDivider)      Uart_Clk_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Uart_Clk_DIV_ID     Uart_Clk__DIV_ID

#define Uart_Clk_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Uart_Clk_CTRL_REG   (*(reg32 *)Uart_Clk__CTRL_REGISTER)
#define Uart_Clk_DIV_REG    (*(reg32 *)Uart_Clk__DIV_REGISTER)

#define Uart_Clk_CMD_DIV_SHIFT          (0u)
#define Uart_Clk_CMD_PA_DIV_SHIFT       (8u)
#define Uart_Clk_CMD_DISABLE_SHIFT      (30u)
#define Uart_Clk_CMD_ENABLE_SHIFT       (31u)

#define Uart_Clk_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Uart_Clk_CMD_DISABLE_SHIFT))
#define Uart_Clk_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Uart_Clk_CMD_ENABLE_SHIFT))

#define Uart_Clk_DIV_FRAC_MASK  (0x000000F8u)
#define Uart_Clk_DIV_FRAC_SHIFT (3u)
#define Uart_Clk_DIV_INT_MASK   (0xFFFFFF00u)
#define Uart_Clk_DIV_INT_SHIFT  (8u)

#else 

#define Uart_Clk_DIV_REG        (*(reg32 *)Uart_Clk__REGISTER)
#define Uart_Clk_ENABLE_REG     Uart_Clk_DIV_REG
#define Uart_Clk_DIV_FRAC_MASK  Uart_Clk__FRAC_MASK
#define Uart_Clk_DIV_FRAC_SHIFT (16u)
#define Uart_Clk_DIV_INT_MASK   Uart_Clk__DIVIDER_MASK
#define Uart_Clk_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Uart_Clk_H) */

/* [] END OF FILE */
