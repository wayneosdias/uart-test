/*******************************************************************************
* File Name: Adc_Sar.h
* Version 2.50
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Sequencing Successive Approximation ADC Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_ADC_SAR_SEQ_Adc_Sar_H)
#define CY_ADC_SAR_SEQ_Adc_Sar_H

#include "cytypes.h"
#include "CyLib.h"


/***************************************
*      Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint32 dftRegVal;
} Adc_Sar_BACKUP_STRUCT;


/**************************************
*    Enumerated Types and Parameters
**************************************/

/*  Clock Source setting constants */
#define Adc_Sar__EXTERNAL 0
#define Adc_Sar__INTERNAL 1

/*  Sample Mode setting constants */
#define Adc_Sar__FREERUNNING 0
#define Adc_Sar__HARDWARESOC 1

/*  Reference type setting constants */
#define Adc_Sar__VDDA_2 0
#define Adc_Sar__VDDA 1
#define Adc_Sar__INTERNAL1024 2
#define Adc_Sar__INTERNAL1024BYPASSED 3
#define Adc_Sar__INTERNALVREF 4
#define Adc_Sar__INTERNALVREFBYPASSED 5
#define Adc_Sar__VDDA_2BYPASSED 6
#define Adc_Sar__EXTERNALVREF 7

/* Input buffer gain setting constants */
#define Adc_Sar__DISABLED 0
#define Adc_Sar__ONE 1
#define Adc_Sar__TWO 2
#define Adc_Sar__FOUR 3
#define Adc_Sar__EIGHT 4
#define Adc_Sar__SIXTEEN 5

/* Negative input setting sonstants in single ended mode */
#define Adc_Sar__VSS 0
#define Adc_Sar__VREF 1
#define Adc_Sar__OTHER 2

/* Compare mode setting constants:
*    Mode0 - Disable
*    Mode1 - Result < Low_Limit
*    Mode2 - Low_Limit <= Result < High_Limit
*    Mode3 - High_Limit <= Result
*    Mode4 - (Result < Low_Limit) or (High_Limit <= Result)
*/
#define Adc_Sar__MODE0 0
#define Adc_Sar__MODE1 1
#define Adc_Sar__MODE2 2
#define Adc_Sar__MODE3 3

#define Adc_Sar__RES8 0
#define Adc_Sar__RES10 1

#define Adc_Sar__RIGHT 0
#define Adc_Sar__LEFT 1

#define Adc_Sar__FSIGNED 1
#define Adc_Sar__FUNSIGNED 0

#define Adc_Sar__ACCUMULATE 0
#define Adc_Sar__FIXEDRESOLUTION 1



/***************************************
*   Conditional Compilation Parameters
****************************************/ 

#define Adc_Sar_CY_SAR_IP_VER0               	(0u)
#define Adc_Sar_CY_SAR_IP_VER1               	(1u)

#if (CY_PSOC4_4100 || CY_PSOC4_4200)
    #define Adc_Sar_CY_SAR_IP_VER              (Adc_Sar_CY_SAR_IP_VER0)
#else /* Other devices */
    #define Adc_Sar_CY_SAR_IP_VER              (Adc_Sar_CY_SAR_IP_VER1)
#endif  /* (CY_PSOC4_4100 || CY_PSOC4_4200) */


/***************************************
*    Initial Parameter Constants
***************************************/
#define Adc_Sar_DEFAULT_SAMPLE_MODE_SEL        (0u)
#define Adc_Sar_DEFAULT_VREF_SEL               (2u)
#define Adc_Sar_DEFAULT_NEG_INPUT_SEL          (0u)
#define Adc_Sar_DEFAULT_ALT_RESOLUTION_SEL     (0u)
#define Adc_Sar_DEFAULT_JUSTIFICATION_SEL      (0u)
#define Adc_Sar_DEFAULT_DIFF_RESULT_FORMAT_SEL (1u)
#define Adc_Sar_DEFAULT_SE_RESULT_FORMAT_SEL   (1u)
#define Adc_Sar_DEFAULT_CLOCK_SOURCE           (1u)
#define Adc_Sar_DEFAULT_VREF_MV_VALUE          (1024)
#define Adc_Sar_DEFAULT_BUFFER_GAIN            (0u)
#define Adc_Sar_DEFAULT_AVG_SAMPLES_NUM        (0u)
#define Adc_Sar_DEFAULT_AVG_SAMPLES_DIV        (0u < 4u) ? (int16)(0x100u >> (7u - 0u)) : (int16)(0x100u >> 4u)
#define Adc_Sar_DEFAULT_AVG_MODE               (1u)
#define Adc_Sar_MAX_RESOLUTION                 (12u)
#define Adc_Sar_DEFAULT_LOW_LIMIT              (0u)
#define Adc_Sar_DEFAULT_HIGH_LIMIT             (2047u)
#define Adc_Sar_DEFAULT_COMPARE_MODE           (0u)
#define Adc_Sar_DEFAULT_ACLKS_NUM              (2u)
#define Adc_Sar_DEFAULT_BCLKS_NUM              (2u)
#define Adc_Sar_DEFAULT_CCLKS_NUM              (2u)
#define Adc_Sar_DEFAULT_DCLKS_NUM              (2u)
#define Adc_Sar_TOTAL_CHANNELS_NUM             (2u)
#define Adc_Sar_SEQUENCED_CHANNELS_NUM         (2u)
#define Adc_Sar_DEFAULT_EN_CHANNELS            (3u)
#define Adc_Sar_NOMINAL_CLOCK_FREQ             (1200000)
#define Adc_Sar_INJ_CHANNEL_ENABLED            (0u)
#define Adc_Sar_IRQ_REMOVE                     (0u)

/* Determines whether the configuration contains external negative input. */
#define Adc_Sar_SINGLE_PRESENT                 (0u)
#define Adc_Sar_CHANNELS_MODE                  (0u)
#define Adc_Sar_MAX_CHANNELS_EN_MASK           (0xffffu >> (16u - Adc_Sar_SEQUENCED_CHANNELS_NUM))


/***************************************
*        Function Prototypes
***************************************/

void Adc_Sar_Start(void);
void Adc_Sar_Stop(void);
void Adc_Sar_Init(void);
void Adc_Sar_Enable(void);
void Adc_Sar_StartConvert(void);
void Adc_Sar_StopConvert(void);
uint32 Adc_Sar_IsEndConversion(uint32 retMode);
int16 Adc_Sar_GetResult16(uint32 chan);
void Adc_Sar_SetChanMask(uint32 mask);
void Adc_Sar_SetLowLimit(uint32 lowLimit);
void Adc_Sar_SetHighLimit(uint32 highLimit);
void Adc_Sar_SetLimitMask(uint32 mask);
void Adc_Sar_SetSatMask(uint32 mask);
void Adc_Sar_SetOffset(uint32 chan, int16 offset);
void Adc_Sar_SetGain(uint32 chan, int32 adcGain);
#if(Adc_Sar_INJ_CHANNEL_ENABLED)
    void Adc_Sar_EnableInjection(void);
#endif /* Adc_Sar_INJ_CHANNEL_ENABLED */
#if(Adc_Sar_DEFAULT_JUSTIFICATION_SEL == Adc_Sar__RIGHT)
    int16 Adc_Sar_CountsTo_mVolts(uint32 chan, int16 adcCounts);
    int32 Adc_Sar_CountsTo_uVolts(uint32 chan, int16 adcCounts);
    float32 Adc_Sar_CountsTo_Volts(uint32 chan, int16 adcCounts);
#endif /* End Adc_Sar_DEFAULT_JUSTIFICATION_SEL == Adc_Sar__RIGHT */
void Adc_Sar_Sleep(void);
void Adc_Sar_Wakeup(void);
void Adc_Sar_SaveConfig(void);
void Adc_Sar_RestoreConfig(void);

CY_ISR_PROTO( Adc_Sar_ISR );


/**************************************
*           API Constants
**************************************/
/* Constants for Sleep mode states */
#define Adc_Sar_DISABLED                   (0x00u)
#define Adc_Sar_ENABLED                    (0x01u)
#define Adc_Sar_STARTED                    (0x02u)
#define Adc_Sar_BOOSTPUMP_ENABLED          (0x04u)

/*   Constants for IsEndConversion() "retMode" parameter  */
#define Adc_Sar_RETURN_STATUS              (0x01u)
#define Adc_Sar_WAIT_FOR_RESULT            (0x02u)
#define Adc_Sar_RETURN_STATUS_INJ          (0x04u)
#define Adc_Sar_WAIT_FOR_RESULT_INJ        (0x08u)

#define Adc_Sar_MAX_FREQUENCY              (18000000)       /*18Mhz*/

#define Adc_Sar_RESOLUTION_12              (12u)
#define Adc_Sar_RESOLUTION_10              (10u)
#define Adc_Sar_RESOLUTION_8               (8u)

#define Adc_Sar_10US_DELAY                 (10u)

#define Adc_Sar_10V_COUNTS                 (10.0F)
#define Adc_Sar_10MV_COUNTS                (10000)
#define Adc_Sar_10UV_COUNTS                (10000000L)


/***************************************
* Global variables external identifier
***************************************/

extern uint8 Adc_Sar_initVar;
extern volatile int16 Adc_Sar_offset[Adc_Sar_TOTAL_CHANNELS_NUM];
extern volatile int32 Adc_Sar_countsPer10Volt[Adc_Sar_TOTAL_CHANNELS_NUM];


/***************************************
*              Registers
***************************************/

#define Adc_Sar_SAR_CTRL_REG                (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CTRL )
#define Adc_Sar_SAR_CTRL_PTR                ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CTRL )

#define Adc_Sar_SAR_SAMPLE_CTRL_REG         (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SAMPLE_CTRL )
#define Adc_Sar_SAR_SAMPLE_CTRL_PTR         ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SAMPLE_CTRL )

#define Adc_Sar_SAR_SAMPLE_TIME01_REG       (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SAMPLE_TIME01 )
#define Adc_Sar_SAR_SAMPLE_TIME01_PTR       ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SAMPLE_TIME01 )

#define Adc_Sar_SAR_SAMPLE_TIME23_REG       (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SAMPLE_TIME23 )
#define Adc_Sar_SAR_SAMPLE_TIME23_PTR       ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SAMPLE_TIME23 )

#define Adc_Sar_SAR_RANGE_THRES_REG         (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_RANGE_THRES )
#define Adc_Sar_SAR_RANGE_THRES_PTR         ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_RANGE_THRES )

#define Adc_Sar_SAR_RANGE_COND_REG          (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_RANGE_COND )
#define Adc_Sar_SAR_RANGE_COND_PTR          ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_RANGE_COND )

#define Adc_Sar_SAR_CHAN_EN_REG             (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_EN )
#define Adc_Sar_SAR_CHAN_EN_PTR             ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_EN )

#define Adc_Sar_SAR_START_CTRL_REG          (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_START_CTRL )
#define Adc_Sar_SAR_START_CTRL_PTR          ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_START_CTRL )

#define Adc_Sar_SAR_DFT_CTRL_REG            (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_DFT_CTRL )
#define Adc_Sar_SAR_DFT_CTRL_PTR            ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_DFT_CTRL )

#define Adc_Sar_SAR_CHAN_CONFIG_REG         (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_CONFIG00 )
#define Adc_Sar_SAR_CHAN_CONFIG_PTR         ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_CONFIG00 )
#define Adc_Sar_SAR_CHAN_CONFIG_IND         Adc_Sar_cy_psoc4_sar__SAR_CHAN_CONFIG00

#define Adc_Sar_SAR_CHAN_WORK_REG           (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_WORK00 )
#define Adc_Sar_SAR_CHAN_WORK_PTR           ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_WORK00 )

#define Adc_Sar_SAR_CHAN_RESULT_REG         (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define Adc_Sar_SAR_CHAN_RESULT_PTR         ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define Adc_Sar_SAR_CHAN_RESULT_IND         Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT00

#define Adc_Sar_SAR_CHAN0_RESULT_REG         (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define Adc_Sar_SAR_CHAN0_RESULT_PTR         ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT00 )

#define Adc_Sar_SAR_CHAN1_RESULT_REG         (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT01 )
#define Adc_Sar_SAR_CHAN1_RESULT_PTR         ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT01 )

#define Adc_Sar_SAR_CHAN2_RESULT_REG         (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT02 )
#define Adc_Sar_SAR_CHAN2_RESULT_PTR         ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT02 )

#define Adc_Sar_SAR_CHAN3_RESULT_REG         (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT03 )
#define Adc_Sar_SAR_CHAN3_RESULT_PTR         ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT03 )

#define Adc_Sar_SAR_CHAN4_RESULT_REG         (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT04 )
#define Adc_Sar_SAR_CHAN4_RESULT_PTR         ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT04 )

#define Adc_Sar_SAR_CHAN5_RESULT_REG         (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT05 )
#define Adc_Sar_SAR_CHAN5_RESULT_PTR         ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT05 )

#define Adc_Sar_SAR_CHAN6_RESULT_REG         (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT06 )
#define Adc_Sar_SAR_CHAN6_RESULT_PTR         ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT06 )

#define Adc_Sar_SAR_CHAN7_RESULT_REG         (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT07 )
#define Adc_Sar_SAR_CHAN7_RESULT_PTR         ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT07 )

#if(Adc_Sar_CY_SAR_IP_VER != Adc_Sar_CY_SAR_IP_VER0)
    #define Adc_Sar_SAR_CHAN8_RESULT_REG     (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT08 )
    #define Adc_Sar_SAR_CHAN8_RESULT_PTR     ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT08 )

    #define Adc_Sar_SAR_CHAN9_RESULT_REG     (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT09 )
    #define Adc_Sar_SAR_CHAN9_RESULT_PTR     ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT09 )

    #define Adc_Sar_SAR_CHAN10_RESULT_REG    (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT10 )
    #define Adc_Sar_SAR_CHAN10_RESULT_PTR    ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT10 )

    #define Adc_Sar_SAR_CHAN11_RESULT_REG    (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT11 )
    #define Adc_Sar_SAR_CHAN11_RESULT_PTR    ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT11 )

    #define Adc_Sar_SAR_CHAN12_RESULT_REG    (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT12 )
    #define Adc_Sar_SAR_CHAN12_RESULT_PTR    ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT12 )

    #define Adc_Sar_SAR_CHAN13_RESULT_REG    (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT13 )
    #define Adc_Sar_SAR_CHAN13_RESULT_PTR    ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT13 )

    #define Adc_Sar_SAR_CHAN14_RESULT_REG    (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT14 )
    #define Adc_Sar_SAR_CHAN14_RESULT_PTR    ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT14 )

    #define Adc_Sar_SAR_CHAN15_RESULT_REG    (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT15 )
    #define Adc_Sar_SAR_CHAN15_RESULT_PTR    ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT15 )
#endif /* (Adc_Sar_CY_SAR_IP_VER != Adc_Sar_CY_SAR_IP_VER0) */

#define Adc_Sar_SAR_CHAN_WORK_VALID_REG     (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_WORK_VALID)
#define Adc_Sar_SAR_CHAN_WORK_VALID_PTR     ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_WORK_VALID)

#define Adc_Sar_SAR_CHAN_RESULT_VALID_REG  ( *(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT_VALID )
#define Adc_Sar_SAR_CHAN_RESULT_VALID_PTR  ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_CHAN_RESULT_VALID )

#define Adc_Sar_SAR_STATUS_REG              (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_STATUS )
#define Adc_Sar_SAR_STATUS_PTR              ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_STATUS )

#define Adc_Sar_SAR_AVG_START_REG           (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_AVG_STAT )
#define Adc_Sar_SAR_AVG_START_PTR           ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_AVG_STAT )

#define Adc_Sar_SAR_INTR_REG                (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_INTR )
#define Adc_Sar_SAR_INTR_PTR                ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_INTR )

#define Adc_Sar_SAR_INTR_SET_REG            (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_INTR_SET )
#define Adc_Sar_SAR_INTR_SET_PTR            ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_INTR_SET )

#define Adc_Sar_SAR_INTR_MASK_REG           (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_INTR_MASK )
#define Adc_Sar_SAR_INTR_MASK_PTR           ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_INTR_MASK )

#define Adc_Sar_SAR_INTR_MASKED_REG         (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_INTR_MASKED )
#define Adc_Sar_SAR_INTR_MASKED_PTR         ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_INTR_MASKED )

#define Adc_Sar_SAR_SATURATE_INTR_REG       (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SATURATE_INTR )
#define Adc_Sar_SAR_SATURATE_INTR_PTR       ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SATURATE_INTR )

#define Adc_Sar_SAR_SATURATE_INTR_SET_REG   (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SATURATE_INTR_SET )
#define Adc_Sar_SAR_SATURATE_INTR_SET_PTR   ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SATURATE_INTR_SET )

#define Adc_Sar_SAR_SATURATE_INTR_MASK_REG (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SATURATE_INTR_MASK )
#define Adc_Sar_SAR_SATURATE_INTR_MASK_PTR ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SATURATE_INTR_MASK )

#define Adc_Sar_SAR_SATURATE_INTR_MASKED_REG \
                                                 (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SATURATE_INTR_MASKED )
#define Adc_Sar_SAR_SATURATE_INTR_MASKED_PTR \
                                                 ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_SATURATE_INTR_MASKED )

#define Adc_Sar_SAR_RANGE_INTR_REG          (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_RANGE_INTR )
#define Adc_Sar_SAR_RANGE_INTR_PTR          ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_RANGE_INTR )

#define Adc_Sar_SAR_RANGE_INTR_SET_REG      (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_RANGE_INTR_SET )
#define Adc_Sar_SAR_RANGE_INTR_SET_PTR      ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_RANGE_INTR_SET )

#define Adc_Sar_SAR_RANGE_INTR_MASK_REG     (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_RANGE_INTR_MASK )
#define Adc_Sar_SAR_RANGE_INTR_MASK_PTR     ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_RANGE_INTR_MASK )

#define Adc_Sar_SAR_RANGE_INTR_MASKED_REG   (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_RANGE_INTR_MASKED )
#define Adc_Sar_SAR_RANGE_INTR_MASKED_PTR   ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_RANGE_INTR_MASKED )

#define Adc_Sar_SAR_INTR_CAUSE_REG          (*(reg32 *) Adc_Sar_cy_psoc4_sar__SAR_INTR_CAUSE )
#define Adc_Sar_SAR_INTR_CAUSE_PTR          ( (reg32 *) Adc_Sar_cy_psoc4_sar__SAR_INTR_CAUSE )

#if(Adc_Sar_INJ_CHANNEL_ENABLED)
    #define Adc_Sar_SAR_INJ_CHAN_CONFIG_REG \
                                                 (*(reg32 *) Adc_Sar_cy_psoc4_sarmux_8__SAR_INJ_CHAN_CONFIG )
    #define Adc_Sar_SAR_INJ_CHAN_CONFIG_PTR    \
                                                 ( (reg32 *) Adc_Sar_cy_psoc4_sarmux_8__SAR_INJ_CHAN_CONFIG )

    #define Adc_Sar_SAR_INJ_RESULT_REG    (*(reg32 *) Adc_Sar_cy_psoc4_sarmux_8__SAR_INJ_RESULT )
    #define Adc_Sar_SAR_INJ_RESULT_PTR    ( (reg32 *) Adc_Sar_cy_psoc4_sarmux_8__SAR_INJ_RESULT )
#endif /* Adc_Sar_INJ_CHANNEL_ENABLED */
    
#define Adc_Sar_MUX_SWITCH0_REG           (*(reg32 *)  Adc_Sar_cy_psoc4_sar__SAR_MUX_SWITCH0 )
#define Adc_Sar_MUX_SWITCH0_PTR           ( (reg32 *)  Adc_Sar_cy_psoc4_sar__SAR_MUX_SWITCH0 )

#define Adc_Sar_MUX_SWITCH_HW_CTRL_REG    (*(reg32 *)  Adc_Sar_cy_psoc4_sar__SAR_MUX_SWITCH_HW_CTRL )
#define Adc_Sar_MUX_SWITCH_HW_CTRL_PTR    ( (reg32 *)  Adc_Sar_cy_psoc4_sar__SAR_MUX_SWITCH_HW_CTRL )

#define Adc_Sar_PUMP_CTRL_REG             (*(reg32 *)  Adc_Sar_cy_psoc4_sar__SAR_PUMP_CTRL )
#define Adc_Sar_PUMP_CTRL_PTR             ( (reg32 *)  Adc_Sar_cy_psoc4_sar__SAR_PUMP_CTRL )

#define Adc_Sar_ANA_TRIM_REG              (*(reg32 *)  Adc_Sar_cy_psoc4_sar__SAR_ANA_TRIM )
#define Adc_Sar_ANA_TRIM_PTR              ( (reg32 *)  Adc_Sar_cy_psoc4_sar__SAR_ANA_TRIM )

#define Adc_Sar_WOUNDING_REG              (*(reg32 *)  Adc_Sar_cy_psoc4_sar__SAR_WOUNDING )
#define Adc_Sar_WOUNDING_PTR              ( (reg32 *)  Adc_Sar_cy_psoc4_sar__SAR_WOUNDING )


/**************************************
*       Register Constants
**************************************/
#define Adc_Sar_INTC_NUMBER                (Adc_Sar_IRQ__INTC_NUMBER)
#define Adc_Sar_INTC_PRIOR_NUMBER          (Adc_Sar_IRQ__INTC_PRIOR_NUM)

/* defines for CTRL register */
#define Adc_Sar_VREF_INTERNAL1024          (0x00000040Lu)
#define Adc_Sar_VREF_EXTERNAL              (0x00000050Lu)
#define Adc_Sar_VREF_VDDA_2                (0x00000060Lu)
#define Adc_Sar_VREF_VDDA                  (0x00000070Lu)
#define Adc_Sar_VREF_INTERNAL1024BYPASSED  (0x000000C0Lu)
#define Adc_Sar_VREF_VDDA_2BYPASSED        (0x000000E0Lu)
#define Adc_Sar_VREF_INTERNALVREF          (0x00000040Lu)
#define Adc_Sar_VREF_INTERNALVREFBYPASSED  (0x000000C0Lu)

#define Adc_Sar_NEG_VSSA_KELVIN            (0x00000000Lu)
#define Adc_Sar_NEG_VSSA                   (0x00000200Lu)
#define Adc_Sar_NEG_VREF                   (0x00000E00Lu)
#if(Adc_Sar_TOTAL_CHANNELS_NUM > 1u)
    #define Adc_Sar_NEG_OTHER              (uint16)((uint16)Adc_Sar_cy_psoc4_sarmux_8__VNEG0 << 9u)
#else
    #define Adc_Sar_NEG_OTHER              (0)
#endif /* Adc_Sar_TOTAL_CHANNELS_NUM > 1u */

#define Adc_Sar_SAR_HW_CTRL_NEGVREF        (0x00002000Lu)

#define Adc_Sar_BOOSTPUMP_EN               (0x00100000Lu)

#define Adc_Sar_NORMAL_PWR                 (0x00000000Lu)
#define Adc_Sar_HALF_PWR                   (0x01000000Lu)
#define Adc_Sar_MORE_PWR                   (0x02000000Lu)
#define Adc_Sar_QUARTER_PWR                (0x03000000Lu)
#define Adc_Sar_DEEPSLEEP_ON               (0x08000000Lu)

#define Adc_Sar_DSI_SYNC_CONFIG            (0x10000000Lu)
#define Adc_Sar_DSI_MODE                   (0x20000000Lu)
#define Adc_Sar_SWITCH_DISABLE             (0x40000000Lu)
#define Adc_Sar_ENABLE                     (0x80000000Lu)

/* defines for STATUS register */
#define Adc_Sar_STATUS_BUSY                (0x80000000Lu)

/* defines for SAMPLE_CTRL register */
#define Adc_Sar_ALT_RESOLUTION_10BIT       (0x00000001Lu)
#define Adc_Sar_ALT_RESOLUTION_8BIT        (0x00000000Lu)

#define Adc_Sar_DATA_ALIGN_LEFT            (0x00000002Lu)
#define Adc_Sar_DATA_ALIGN_RIGHT           (0x00000000Lu)

#define Adc_Sar_SE_SIGNED_RESULT           (0x00000004Lu)
#define Adc_Sar_SE_UNSIGNED_RESULT         (0x00000000Lu)

#define Adc_Sar_DIFF_SIGNED_RESULT         (0x00000008Lu)
#define Adc_Sar_DIFF_UNSIGNED_RESULT       (0x00000000Lu)

#define Adc_Sar_AVG_CNT_OFFSET             (4u)
#define Adc_Sar_AVG_CNT_MASK               (0x00000070Lu)
#define Adc_Sar_AVG_SHIFT                  (0x00000080Lu)

#define Adc_Sar_CONTINUOUS_EN              (0x00010000Lu)
#define Adc_Sar_DSI_TRIGGER_EN             (0x00020000Lu)
#define Adc_Sar_DSI_TRIGGER_LEVEL          (0x00040000Lu)
#define Adc_Sar_DSI_SYNC_TRIGGER           (0x00080000Lu)
#define Adc_Sar_EOS_DSI_OUT_EN             (0x80000000Lu)

/* defines for SAMPLE_TIME01 / SAMPLE_TIME23 registers */
#define Adc_Sar_SAMPLE_TIME13_OFFSET       (16u)
#define Adc_Sar_SAMPLE_TIME02_MASK         (0x000003FFLu)
#define Adc_Sar_SAMPLE_TIME13_MASK         (0x03FF0000Lu)

/* defines for RANGE_THRES registers */
#define Adc_Sar_RANGE_HIGH_OFFSET          (16u)
#define Adc_Sar_RANGE_HIGH_MASK            (0xFFFF0000Lu)
#define Adc_Sar_RANGE_LOW_MASK             (0x0000FFFFLu)

/* defines for RANGE_COND register */
/* Compare mode setting constants:
*    BELOW   - Result < Low_Limit
*    INSIDE  - Low_Limit <= Result < High_Limit
*    ABOVE   - High_Limit <= Result
*    OUTSIDE - (Result < Low_Limit) or (High_Limit <= Result)
*/
#define Adc_Sar_CMP_MODE_BELOW             (0x00000000Lu)
#define Adc_Sar_CMP_MODE_INSIDE            (0x40000000Lu)
#define Adc_Sar_CMP_MODE_ABOVE             (0x80000000Lu)
#define Adc_Sar_CMP_MODE_OUTSIDE           (0xC0000000Lu)
#define Adc_Sar_CMP_OFFSET                 (30u)

/* defines for _START_CTRL register */
#define Adc_Sar_FW_TRIGGER                 (0x00000001Lu)

/* defines for DFT_CTRL register */
#define Adc_Sar_DLY_INC                    (0x00000001Lu)
#define Adc_Sar_HIZ                        (0x00000002Lu)
#define Adc_Sar_DFT_INC_MASK               (0x000F0000Lu)
#define Adc_Sar_DFT_OUTC_MASK              (0x00700000Lu)
#define Adc_Sar_SEL_CSEL_DFT_MASK          (0x0F000000Lu)

/* configuration for clock speed > 9 Mhz based on
* characterization results
*/
#define Adc_Sar_SEL_CSEL_DFT_CHAR          (0x03000000Lu)
#define Adc_Sar_EN_CSEL_DFT                (0x10000000Lu)
#define Adc_Sar_DCEN                       (0x20000000Lu)
#define Adc_Sar_ADFT_OVERRIDE              (0x80000000Lu)

/* defines for CHAN_CONFIG / DIE_CHAN_CONFIG register
*  and channelsConfig parameter
*/
#define Adc_Sar_SARMUX_VIRT_SELECT         (0x00000070Lu)
#define Adc_Sar_DIFFERENTIAL_EN            (0x00000100Lu)
#define Adc_Sar_ALT_RESOLUTION_ON          (0x00000200Lu)
#define Adc_Sar_AVERAGING_EN               (0x00000400Lu)

#define Adc_Sar_SAMPLE_TIME_SEL_SHIFT      (12u)
#define Adc_Sar_SAMPLE_TIME_SEL_MASK       (0x00003000Lu)

#define Adc_Sar_CHANNEL_CONFIG_MASK        (0x00003700Lu)

/* for CHAN_CONFIG only */
#define Adc_Sar_DSI_OUT_EN                 (0x80000000Lu)

/* for INJ_CHAN_CONFIG only */
#define Adc_Sar_INJ_TAILGATING             (0x40000000Lu)
#define Adc_Sar_INJ_CHAN_EN                (0x80000000Lu)

/* defines for CHAN_WORK register */
#define Adc_Sar_SAR_WRK_MAX_12BIT          (0x00001000Lu)
#define Adc_Sar_SAR_WRK_MAX_10BIT          (0x00000400Lu)
#define Adc_Sar_SAR_WRK_MAX_8BIT           (0x00000100Lu)

/* defines for CHAN_RESULT register */
#define Adc_Sar_RESULT_MASK                (0x0000FFFFLu)
#define Adc_Sar_SATURATE_INTR_MIR          (0x20000000Lu)
#define Adc_Sar_RANGE_INTR_MIR             (0x40000000Lu)
#define Adc_Sar_CHAN_RESULT_VALID_MIR      (0x80000000Lu)

/* defines for INTR_MASK register */
#define Adc_Sar_EOS_MASK                   (0x00000001Lu)
#define Adc_Sar_OVERFLOW_MASK              (0x00000002Lu)
#define Adc_Sar_FW_COLLISION_MASK          (0x00000004Lu)
#define Adc_Sar_DSI_COLLISION_MASK         (0x00000008Lu)
#define Adc_Sar_INJ_EOC_MASK               (0x00000010Lu)
#define Adc_Sar_INJ_SATURATE_MASK          (0x00000020Lu)
#define Adc_Sar_INJ_RANGE_MASK             (0x00000040Lu)
#define Adc_Sar_INJ_COLLISION_MASK         (0x00000080Lu)

/* defines for INJ_RESULT register */
#define Adc_Sar_INJ_COLLISION_INTR_MIR     (0x10000000Lu)
#define Adc_Sar_INJ_SATURATE_INTR_MIR      (0x20000000Lu)
#define Adc_Sar_INJ_RANGE_INTR_MIR         (0x40000000Lu)
#define Adc_Sar_INJ_EOC_INTR_MIR           (0x80000000Lu)

/* defines for MUX_SWITCH0 register */
#define Adc_Sar_MUX_FW_VSSA_VMINUS         (0x00010000Lu)

/* defines for PUMP_CTRL register */
#define Adc_Sar_PUMP_CTRL_ENABLED          (0x80000000Lu)

/* additional defines for channelsConfig parameter */
#define Adc_Sar_IS_SATURATE_EN_MASK        (0x00000001Lu)
#define Adc_Sar_IS_RANGE_CTRL_EN_MASK      (0x00000002Lu)

/* defines for WOUNDING register */
#define Adc_Sar_WOUNDING_12BIT             (0x00000000Lu)
#define Adc_Sar_WOUNDING_10BIT             (0x00000001Lu)
#define Adc_Sar_WOUNDING_8BIT              (0x00000002Lu)

/* Trim value based on characterization */
#define Adc_Sar_TRIM_COEF                  (2u)

#if(Adc_Sar_MAX_RESOLUTION == Adc_Sar_RESOLUTION_10)
    #define Adc_Sar_ALT_WOUNDING           Adc_Sar_WOUNDING_10BIT
#else
    #define Adc_Sar_ALT_WOUNDING           Adc_Sar_WOUNDING_8BIT
#endif /* Adc_Sar_MAX_RESOLUTION == Adc_Sar_RESOLUTION_10 */

#if(Adc_Sar_DEFAULT_VREF_SEL == Adc_Sar__VDDA_2)
    #define Adc_Sar_DEFAULT_VREF_SOURCE    Adc_Sar_VREF_VDDA_2
#elif(Adc_Sar_DEFAULT_VREF_SEL == Adc_Sar__VDDA)
    #define Adc_Sar_DEFAULT_VREF_SOURCE    Adc_Sar_VREF_VDDA
#elif(Adc_Sar_DEFAULT_VREF_SEL == Adc_Sar__INTERNAL1024)
    #define Adc_Sar_DEFAULT_VREF_SOURCE    Adc_Sar_VREF_INTERNAL1024
#elif(Adc_Sar_DEFAULT_VREF_SEL == Adc_Sar__INTERNAL1024BYPASSED)
    #define Adc_Sar_DEFAULT_VREF_SOURCE    Adc_Sar_VREF_INTERNAL1024BYPASSED
#elif(Adc_Sar_DEFAULT_VREF_SEL == Adc_Sar__INTERNALVREF)
    #define Adc_Sar_DEFAULT_VREF_SOURCE    Adc_Sar_VREF_INTERNALVREF
#elif(Adc_Sar_DEFAULT_VREF_SEL == Adc_Sar__INTERNALVREFBYPASSED)
    #define Adc_Sar_DEFAULT_VREF_SOURCE    Adc_Sar_VREF_INTERNALVREFBYPASSED
#elif(Adc_Sar_DEFAULT_VREF_SEL == Adc_Sar__VDDA_2BYPASSED)
    #define Adc_Sar_DEFAULT_VREF_SOURCE    Adc_Sar_VREF_VDDA_2BYPASSED
#else
    #define Adc_Sar_DEFAULT_VREF_SOURCE    Adc_Sar_VREF_EXTERNAL
#endif /* Adc_Sar_DEFAULT_VREF_SEL == Adc_Sar__VDDA_2 */

#if(Adc_Sar_DEFAULT_NEG_INPUT_SEL == Adc_Sar__VSS)
    /* Connect NEG input of SARADC to VSSA close to the SARADC for single channel mode */
    #if(Adc_Sar_TOTAL_CHANNELS_NUM == 1u)
        #define Adc_Sar_DEFAULT_SE_NEG_INPUT    Adc_Sar_NEG_VSSA
    #else
        #define Adc_Sar_DEFAULT_SE_NEG_INPUT    Adc_Sar_NEG_VSSA_KELVIN
    #endif /* (Adc_Sar_TOTAL_CHANNELS_NUM == 1u) */
    /* Do not connect VSSA to VMINUS when one channel in differential mode used */
    #if((Adc_Sar_TOTAL_CHANNELS_NUM == 1u) && (Adc_Sar_CHANNELS_MODE != 0u))
        #define Adc_Sar_DEFAULT_MUX_SWITCH0     0u
    #else    /* miltiple channels or one single channel */
        #define Adc_Sar_DEFAULT_MUX_SWITCH0     Adc_Sar_MUX_FW_VSSA_VMINUS
    #endif /* (Adc_Sar_TOTAL_CHANNELS_NUM == 1u) */
#elif(Adc_Sar_DEFAULT_NEG_INPUT_SEL == Adc_Sar__VREF)
    /* Do not connect VNEG to VREF when one channel in differential mode used */
    #if((Adc_Sar_TOTAL_CHANNELS_NUM == 1u) && (Adc_Sar_CHANNELS_MODE != 0u))
        #define Adc_Sar_DEFAULT_SE_NEG_INPUT    0u
    #else    /* miltiple channels or one single channel */
        #define Adc_Sar_DEFAULT_SE_NEG_INPUT    Adc_Sar_NEG_VREF
    #endif /* (Adc_Sar_TOTAL_CHANNELS_NUM == 1u) */
    #define Adc_Sar_DEFAULT_MUX_SWITCH0     0u
#elif (Adc_Sar_SINGLE_PRESENT != 0u)
    #define Adc_Sar_DEFAULT_SE_NEG_INPUT    Adc_Sar_NEG_OTHER
    #define Adc_Sar_DEFAULT_MUX_SWITCH0     0u
#else
    #define Adc_Sar_DEFAULT_SE_NEG_INPUT    0u
    #define Adc_Sar_DEFAULT_MUX_SWITCH0     0u
#endif /* Adc_Sar_DEFAULT_NEG_INPUT_SEL == Adc_Sar__VREF */

/* If the SAR is configured for multiple channels, always set SAR_HW_CTRL_NEGVREF to 1 */
#if(Adc_Sar_TOTAL_CHANNELS_NUM == 1u)
    #define Adc_Sar_DEFAULT_HW_CTRL_NEGVREF 0u
#else
    #define Adc_Sar_DEFAULT_HW_CTRL_NEGVREF Adc_Sar_SAR_HW_CTRL_NEGVREF
#endif /* (Adc_Sar_TOTAL_CHANNELS_NUM == 1u) */


#if(Adc_Sar_DEFAULT_ALT_RESOLUTION_SEL == Adc_Sar__RES8)
    #define Adc_Sar_DEFAULT_ALT_RESOLUTION     (Adc_Sar_ALT_RESOLUTION_8BIT)
    #define Adc_Sar_DEFAULT_MAX_WRK_ALT        (Adc_Sar_SAR_WRK_MAX_8BIT)
#else
    #define Adc_Sar_DEFAULT_ALT_RESOLUTION     (Adc_Sar_ALT_RESOLUTION_10BIT)
    #define Adc_Sar_DEFAULT_MAX_WRK_ALT        (Adc_Sar_SAR_WRK_MAX_10BIT)
#endif /* End Adc_Sar_DEFAULT_ALT_RESOLUTION_SEL == Adc_Sar__RES8 */

#if(Adc_Sar_DEFAULT_JUSTIFICATION_SEL == Adc_Sar__RIGHT)
    #define Adc_Sar_DEFAULT_JUSTIFICATION  Adc_Sar_DATA_ALIGN_RIGHT
#else
    #define Adc_Sar_DEFAULT_JUSTIFICATION  Adc_Sar_DATA_ALIGN_LEFT
#endif /* Adc_Sar_DEFAULT_JUSTIFICATION_SEL == Adc_Sar__RIGHT */

#if(Adc_Sar_DEFAULT_DIFF_RESULT_FORMAT_SEL == Adc_Sar__FSIGNED)
    #define Adc_Sar_DEFAULT_DIFF_RESULT_FORMAT Adc_Sar_DIFF_SIGNED_RESULT
#else
    #define Adc_Sar_DEFAULT_DIFF_RESULT_FORMAT Adc_Sar_DIFF_UNSIGNED_RESULT
#endif /* Adc_Sar_DEFAULT_DIFF_RESULT_FORMAT_SEL == Adc_Sar__FSIGNED */

#if(Adc_Sar_DEFAULT_SE_RESULT_FORMAT_SEL == Adc_Sar__FSIGNED)
    #define Adc_Sar_DEFAULT_SE_RESULT_FORMAT Adc_Sar_SE_SIGNED_RESULT
#else
    #define Adc_Sar_DEFAULT_SE_RESULT_FORMAT Adc_Sar_SE_UNSIGNED_RESULT
#endif /* Adc_Sar_DEFAULT_SE_RESULT_FORMAT_SEL == Adc_Sar__FSIGNED */

#if(Adc_Sar_DEFAULT_SAMPLE_MODE_SEL == Adc_Sar__FREERUNNING)
    #define Adc_Sar_DSI_TRIGGER        0u
#else /* Firmware trigger */
    #define Adc_Sar_DSI_TRIGGER        (Adc_Sar_DSI_TRIGGER_EN | Adc_Sar_DSI_SYNC_TRIGGER)
#endif /* End Adc_Sar_DEFAULT_SAMPLE_MODE == Adc_Sar__FREERUNNING */

#if(Adc_Sar_INJ_CHANNEL_ENABLED)
    #define Adc_Sar_SAR_INTR_MASK      (Adc_Sar_EOS_MASK | Adc_Sar_INJ_EOC_MASK)
#else
    #define Adc_Sar_SAR_INTR_MASK      (Adc_Sar_EOS_MASK)
#endif /* Adc_Sar_INJ_CHANNEL_ENABLED*/

#if(Adc_Sar_DEFAULT_AVG_MODE == Adc_Sar__FIXEDRESOLUTION)
    #define Adc_Sar_AVG_SHIFT_MODE     Adc_Sar_AVG_SHIFT
#else
    #define Adc_Sar_AVG_SHIFT_MODE     0u
#endif /* End Adc_Sar_DEFAULT_AVG_MODE */

#define Adc_Sar_COMPARE_MODE           (uint32)((uint32)(Adc_Sar_DEFAULT_COMPARE_MODE) \
                                                << Adc_Sar_CMP_OFFSET)

#if(Adc_Sar_TOTAL_CHANNELS_NUM > 1u)
    #define Adc_Sar_DEFAULT_SWITCH_CONF    0u
#else /* Disable SAR sequencer from enabling routing switches in single channel mode */
    #define Adc_Sar_DEFAULT_SWITCH_CONF    Adc_Sar_SWITCH_DISABLE
#endif /* End Adc_Sar_TOTAL_CHANNELS_NUM > 1 */

#define Adc_Sar_DEFAULT_POWER \
       ((Adc_Sar_NOMINAL_CLOCK_FREQ > (Adc_Sar_MAX_FREQUENCY / 4)) ? Adc_Sar_NORMAL_PWR : \
       ((Adc_Sar_NOMINAL_CLOCK_FREQ > (Adc_Sar_MAX_FREQUENCY / 8)) ? Adc_Sar_HALF_PWR : \
                                                                                       Adc_Sar_QUARTER_PWR))

#define Adc_Sar_DEFAULT_CTRL_REG_CFG       (Adc_Sar_DEFAULT_VREF_SOURCE \
                                                   | Adc_Sar_DEFAULT_SE_NEG_INPUT \
                                                   | Adc_Sar_DEFAULT_HW_CTRL_NEGVREF \
                                                   | Adc_Sar_DEFAULT_POWER \
                                                   | Adc_Sar_DSI_SYNC_CONFIG \
                                                   | Adc_Sar_DEFAULT_SWITCH_CONF)

#define Adc_Sar_DEFAULT_SAMPLE_CTRL_REG_CFG (Adc_Sar_DEFAULT_DIFF_RESULT_FORMAT \
                                                    | Adc_Sar_DEFAULT_SE_RESULT_FORMAT \
                                                    | Adc_Sar_DEFAULT_JUSTIFICATION \
                                                    | Adc_Sar_DEFAULT_ALT_RESOLUTION \
                                           | (uint8)(Adc_Sar_DEFAULT_AVG_SAMPLES_NUM \
                                                   << Adc_Sar_AVG_CNT_OFFSET) \
                                                    | Adc_Sar_AVG_SHIFT_MODE \
                                                    | Adc_Sar_DSI_TRIGGER \
                                                    | Adc_Sar_EOS_DSI_OUT_EN)

#define Adc_Sar_DEFAULT_RANGE_THRES_REG_CFG (Adc_Sar_DEFAULT_LOW_LIMIT \
            | (uint32)((uint32)Adc_Sar_DEFAULT_HIGH_LIMIT << Adc_Sar_RANGE_HIGH_OFFSET))

#define Adc_Sar_DEFAULT_SAMPLE_TIME01_REG_CFG (Adc_Sar_DEFAULT_ACLKS_NUM \
            | (uint32)((uint32)Adc_Sar_DEFAULT_BCLKS_NUM << Adc_Sar_SAMPLE_TIME13_OFFSET))

#define Adc_Sar_DEFAULT_SAMPLE_TIME23_REG_CFG (Adc_Sar_DEFAULT_CCLKS_NUM \
            | (uint32)((uint32)Adc_Sar_DEFAULT_DCLKS_NUM << Adc_Sar_SAMPLE_TIME13_OFFSET))


#endif /* End CY_ADC_SAR_SEQ_Adc_Sar_H */


/* [] END OF FILE */
