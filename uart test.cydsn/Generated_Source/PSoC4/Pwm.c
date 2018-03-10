/*******************************************************************************
* File Name: Pwm.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Pwm.h"

/* Error message for removed <resource> through optimization */
#ifdef Pwm_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* Pwm_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 Pwm_initVar = 0u;


/*******************************************************************************
* Function Name: Pwm_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Pwm_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Pwm_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(Pwm_initVar == 0u)
    {
        Pwm_Init();
        Pwm_initVar = 1u;
    }
    Pwm_Enable();

}


/*******************************************************************************
* Function Name: Pwm_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  Pwm_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Pwm_Init(void) 
{
    #if (Pwm_UsingFixedFunction || Pwm_UseControl)
        uint8 ctrl;
    #endif /* (Pwm_UsingFixedFunction || Pwm_UseControl) */

    #if(!Pwm_UsingFixedFunction)
        #if(Pwm_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 Pwm_interruptState;
        #endif /* (Pwm_UseStatus) */
    #endif /* (!Pwm_UsingFixedFunction) */

    #if (Pwm_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        Pwm_CONTROL |= Pwm_CFG0_MODE;
        #if (Pwm_DeadBand2_4)
            Pwm_CONTROL |= Pwm_CFG0_DB;
        #endif /* (Pwm_DeadBand2_4) */

        ctrl = Pwm_CONTROL3 & ((uint8 )(~Pwm_CTRL_CMPMODE1_MASK));
        Pwm_CONTROL3 = ctrl | Pwm_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        Pwm_RT1 &= ((uint8)(~Pwm_RT1_MASK));
        Pwm_RT1 |= Pwm_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        Pwm_RT1 &= ((uint8)(~Pwm_SYNCDSI_MASK));
        Pwm_RT1 |= Pwm_SYNCDSI_EN;

    #elif (Pwm_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = Pwm_CONTROL & ((uint8)(~Pwm_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~Pwm_CTRL_CMPMODE1_MASK));
        Pwm_CONTROL = ctrl | Pwm_DEFAULT_COMPARE2_MODE |
                                   Pwm_DEFAULT_COMPARE1_MODE;
    #endif /* (Pwm_UsingFixedFunction) */

    #if (!Pwm_UsingFixedFunction)
        #if (Pwm_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            Pwm_AUX_CONTROLDP0 |= (Pwm_AUX_CTRL_FIFO0_CLR);
        #else /* (Pwm_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            Pwm_AUX_CONTROLDP0 |= (Pwm_AUX_CTRL_FIFO0_CLR);
            Pwm_AUX_CONTROLDP1 |= (Pwm_AUX_CTRL_FIFO0_CLR);
        #endif /* (Pwm_Resolution == 8) */

        Pwm_WriteCounter(Pwm_INIT_PERIOD_VALUE);
    #endif /* (!Pwm_UsingFixedFunction) */

    Pwm_WritePeriod(Pwm_INIT_PERIOD_VALUE);

        #if (Pwm_UseOneCompareMode)
            Pwm_WriteCompare(Pwm_INIT_COMPARE_VALUE1);
        #else
            Pwm_WriteCompare1(Pwm_INIT_COMPARE_VALUE1);
            Pwm_WriteCompare2(Pwm_INIT_COMPARE_VALUE2);
        #endif /* (Pwm_UseOneCompareMode) */

        #if (Pwm_KillModeMinTime)
            Pwm_WriteKillTime(Pwm_MinimumKillTime);
        #endif /* (Pwm_KillModeMinTime) */

        #if (Pwm_DeadBandUsed)
            Pwm_WriteDeadTime(Pwm_INIT_DEAD_TIME);
        #endif /* (Pwm_DeadBandUsed) */

    #if (Pwm_UseStatus || Pwm_UsingFixedFunction)
        Pwm_SetInterruptMode(Pwm_INIT_INTERRUPTS_MODE);
    #endif /* (Pwm_UseStatus || Pwm_UsingFixedFunction) */

    #if (Pwm_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        Pwm_GLOBAL_ENABLE |= Pwm_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        Pwm_CONTROL2 |= Pwm_CTRL2_IRQ_SEL;
    #else
        #if(Pwm_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Pwm_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Pwm_STATUS_AUX_CTRL |= Pwm_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(Pwm_interruptState);

            /* Clear the FIFO to enable the Pwm_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            Pwm_ClearFIFO();
        #endif /* (Pwm_UseStatus) */
    #endif /* (Pwm_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Pwm_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void Pwm_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Pwm_UsingFixedFunction)
        Pwm_GLOBAL_ENABLE |= Pwm_BLOCK_EN_MASK;
        Pwm_GLOBAL_STBY_ENABLE |= Pwm_BLOCK_STBY_EN_MASK;
    #endif /* (Pwm_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (Pwm_UseControl || Pwm_UsingFixedFunction)
        Pwm_CONTROL |= Pwm_CTRL_ENABLE;
    #endif /* (Pwm_UseControl || Pwm_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Pwm_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void Pwm_Stop(void) 
{
    #if (Pwm_UseControl || Pwm_UsingFixedFunction)
        Pwm_CONTROL &= ((uint8)(~Pwm_CTRL_ENABLE));
    #endif /* (Pwm_UseControl || Pwm_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (Pwm_UsingFixedFunction)
        Pwm_GLOBAL_ENABLE &= ((uint8)(~Pwm_BLOCK_EN_MASK));
        Pwm_GLOBAL_STBY_ENABLE &= ((uint8)(~Pwm_BLOCK_STBY_EN_MASK));
    #endif /* (Pwm_UsingFixedFunction) */
}

#if (Pwm_UseOneCompareMode)
    #if (Pwm_CompareMode1SW)


        /*******************************************************************************
        * Function Name: Pwm_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Pwm_SetCompareMode(uint8 comparemode) 
        {
            #if(Pwm_UsingFixedFunction)

                #if(0 != Pwm_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << Pwm_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != Pwm_CTRL_CMPMODE1_SHIFT) */

                Pwm_CONTROL3 &= ((uint8)(~Pwm_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                Pwm_CONTROL3 |= comparemodemasked;

            #elif (Pwm_UseControl)

                #if(0 != Pwm_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << Pwm_CTRL_CMPMODE1_SHIFT)) &
                                                Pwm_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & Pwm_CTRL_CMPMODE1_MASK;
                #endif /* (0 != Pwm_CTRL_CMPMODE1_SHIFT) */

                #if(0 != Pwm_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << Pwm_CTRL_CMPMODE2_SHIFT)) &
                                               Pwm_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & Pwm_CTRL_CMPMODE2_MASK;
                #endif /* (0 != Pwm_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                Pwm_CONTROL &= ((uint8)(~(Pwm_CTRL_CMPMODE1_MASK |
                                            Pwm_CTRL_CMPMODE2_MASK)));
                Pwm_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (Pwm_UsingFixedFunction) */
        }
    #endif /* Pwm_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (Pwm_CompareMode1SW)


        /*******************************************************************************
        * Function Name: Pwm_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Pwm_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != Pwm_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << Pwm_CTRL_CMPMODE1_SHIFT)) &
                                           Pwm_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & Pwm_CTRL_CMPMODE1_MASK;
            #endif /* (0 != Pwm_CTRL_CMPMODE1_SHIFT) */

            #if (Pwm_UseControl)
                Pwm_CONTROL &= ((uint8)(~Pwm_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                Pwm_CONTROL |= comparemodemasked;
            #endif /* (Pwm_UseControl) */
        }
    #endif /* Pwm_CompareMode1SW */

#if (Pwm_CompareMode2SW)


    /*******************************************************************************
    * Function Name: Pwm_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Pwm_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != Pwm_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << Pwm_CTRL_CMPMODE2_SHIFT)) &
                                                 Pwm_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & Pwm_CTRL_CMPMODE2_MASK;
        #endif /* (0 != Pwm_CTRL_CMPMODE2_SHIFT) */

        #if (Pwm_UseControl)
            Pwm_CONTROL &= ((uint8)(~Pwm_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            Pwm_CONTROL |= comparemodemasked;
        #endif /* (Pwm_UseControl) */
    }
    #endif /*Pwm_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!Pwm_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Pwm_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void Pwm_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(Pwm_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: Pwm_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint8 Pwm_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(Pwm_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(Pwm_CAPTURE_LSB_PTR));
    }

    #if (Pwm_UseStatus)


        /*******************************************************************************
        * Function Name: Pwm_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Pwm_ClearFIFO(void) 
        {
            while(0u != (Pwm_ReadStatusRegister() & Pwm_STATUS_FIFONEMPTY))
            {
                (void)Pwm_ReadCapture();
            }
        }

    #endif /* Pwm_UseStatus */

#endif /* !Pwm_UsingFixedFunction */


/*******************************************************************************
* Function Name: Pwm_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void Pwm_WritePeriod(uint8 period) 
{
    #if(Pwm_UsingFixedFunction)
        CY_SET_REG16(Pwm_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(Pwm_PERIOD_LSB_PTR, period);
    #endif /* (Pwm_UsingFixedFunction) */
}

#if (Pwm_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Pwm_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void Pwm_WriteCompare(uint8 compare) \
                                       
    {
        #if(Pwm_UsingFixedFunction)
            CY_SET_REG16(Pwm_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(Pwm_COMPARE1_LSB_PTR, compare);
        #endif /* (Pwm_UsingFixedFunction) */

        #if (Pwm_PWMMode == Pwm__B_PWM__DITHER)
            #if(Pwm_UsingFixedFunction)
                CY_SET_REG16(Pwm_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(Pwm_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (Pwm_UsingFixedFunction) */
        #endif /* (Pwm_PWMMode == Pwm__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: Pwm_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Pwm_WriteCompare1(uint8 compare) \
                                        
    {
        #if(Pwm_UsingFixedFunction)
            CY_SET_REG16(Pwm_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(Pwm_COMPARE1_LSB_PTR, compare);
        #endif /* (Pwm_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: Pwm_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Pwm_WriteCompare2(uint8 compare) \
                                        
    {
        #if(Pwm_UsingFixedFunction)
            CY_SET_REG16(Pwm_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(Pwm_COMPARE2_LSB_PTR, compare);
        #endif /* (Pwm_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (Pwm_DeadBandUsed)


    /*******************************************************************************
    * Function Name: Pwm_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Pwm_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!Pwm_DeadBand2_4)
            CY_SET_REG8(Pwm_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            Pwm_DEADBAND_COUNT &= ((uint8)(~Pwm_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(Pwm_DEADBAND_COUNT_SHIFT)
                Pwm_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << Pwm_DEADBAND_COUNT_SHIFT)) &
                                                    Pwm_DEADBAND_COUNT_MASK;
            #else
                Pwm_DEADBAND_COUNT |= deadtime & Pwm_DEADBAND_COUNT_MASK;
            #endif /* (Pwm_DEADBAND_COUNT_SHIFT) */

        #endif /* (!Pwm_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: Pwm_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 Pwm_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!Pwm_DeadBand2_4)
            return (CY_GET_REG8(Pwm_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(Pwm_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(Pwm_DEADBAND_COUNT & Pwm_DEADBAND_COUNT_MASK)) >>
                                                                           Pwm_DEADBAND_COUNT_SHIFT));
            #else
                return (Pwm_DEADBAND_COUNT & Pwm_DEADBAND_COUNT_MASK);
            #endif /* (Pwm_DEADBAND_COUNT_SHIFT) */
        #endif /* (!Pwm_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (Pwm_UseStatus || Pwm_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Pwm_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Pwm_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(Pwm_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: Pwm_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 Pwm_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(Pwm_STATUS_PTR));
    }

#endif /* (Pwm_UseStatus || Pwm_UsingFixedFunction) */


#if (Pwm_UseControl)


    /*******************************************************************************
    * Function Name: Pwm_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 Pwm_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(Pwm_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: Pwm_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Pwm_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(Pwm_CONTROL_PTR, control);
    }

#endif /* (Pwm_UseControl) */


#if (!Pwm_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Pwm_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint8 Pwm_ReadCapture(void) 
    {
        return (CY_GET_REG8(Pwm_CAPTURE_LSB_PTR));
    }

#endif /* (!Pwm_UsingFixedFunction) */


#if (Pwm_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Pwm_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint8 Pwm_ReadCompare(void) 
    {
        #if(Pwm_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(Pwm_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(Pwm_COMPARE1_LSB_PTR));
        #endif /* (Pwm_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: Pwm_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 Pwm_ReadCompare1(void) 
    {
        return (CY_GET_REG8(Pwm_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: Pwm_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 Pwm_ReadCompare2(void) 
    {
        return (CY_GET_REG8(Pwm_COMPARE2_LSB_PTR));
    }

#endif /* (Pwm_UseOneCompareMode) */


/*******************************************************************************
* Function Name: Pwm_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint8 Pwm_ReadPeriod(void) 
{
    #if(Pwm_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Pwm_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(Pwm_PERIOD_LSB_PTR));
    #endif /* (Pwm_UsingFixedFunction) */
}

#if ( Pwm_KillModeMinTime)


    /*******************************************************************************
    * Function Name: Pwm_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Pwm_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(Pwm_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: Pwm_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 Pwm_ReadKillTime(void) 
    {
        return (CY_GET_REG8(Pwm_KILLMODEMINTIME_PTR));
    }

#endif /* ( Pwm_KillModeMinTime) */

/* [] END OF FILE */
