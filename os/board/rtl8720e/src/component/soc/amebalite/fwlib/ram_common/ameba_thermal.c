/**
  ******************************************************************************
  * @file    ameba_thermal.c
  * @author
  * @version V1.0.0
  * @date    2020-10-16
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the thermal peripheral:
  *           - Initialization and Configuration
  *           - Analog configuration
  *           - Threshold configuration
  *           - Interrupts and flags management
  *
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

/**
  * @brief  Initializes the parameters in the TM_InitStruct with default values.
  * @param  TM_InitStruct: pointer to a TM_InitTypeDef structure that contains
  *         the configuration information for the thermal peripheral.
  * @retval  None
  */
void TM_StructInit(TM_InitTypeDef *TM_InitStruct)
{
	TM_InitStruct->TM_DownSampRate = TM_DSR_512;
	TM_InitStruct->TM_AdcClkDiv = TM_ADC_CLK_DIV_128;
	TM_InitStruct->TM_HighProtectThreshold = 0x7D;
	TM_InitStruct->TM_HighWarnThreshold = 0x69;
	TM_InitStruct->TM_LowWarnThreshold = 0x1D8;
	TM_InitStruct->TM_TimePeriod = 0x0F;
}

/**
  * @brief  Initializes the thermal according to the specified parameters in TM_InitStruct.
  * @param  TM_InitStruct: pointer to a TM_InitTypeDef structure that contains
  *         the configuration information for the thermal peripheral.
  * @retval  None
  */
void TM_Init(TM_InitTypeDef *TM_InitStruct)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	assert_param(IS_TM_DOWN_SAMPLE_RATE(TM_InitStruct->TM_DownSampRate));
	assert_param(IS_TM_ADC_CLK_DIV(TM_InitStruct->TM_AdcClkDiv));

	TM_Cmd(DISABLE);

	thermal->TM_CTRL &= ~TM_MASK_OSR;
	thermal->TM_CTRL |= TM_OSR(TM_InitStruct->TM_DownSampRate);
	if (TM_InitStruct->TM_AdcClkDiv == TM_ADC_CLK_DIV_128) {
		thermal->TM_CTRL |= TM_BIT_ADCCKSEL;
	} else {
		thermal->TM_CTRL &= ~TM_BIT_ADCCKSEL;
	}

	TM_SetLatch(DISABLE);

	TM_HighPtConfig(TM_InitStruct->TM_HighProtectThreshold, ENABLE);
	TM_HighWtConfig(TM_InitStruct->TM_HighWarnThreshold, ENABLE);
	TM_LowWtConfig(TM_InitStruct->TM_LowWarnThreshold, ENABLE);

	TM_MaxTempClr();
	TM_MinTempClr();
	// clear all interrupt
	TM_INTClear();

	thermal->TM_TIMER = TM_TIME_PERIOD(TM_InitStruct->TM_TimePeriod);

	TM_INTConfig(TM_BIT_IMR_TM_HIGH_WT | TM_BIT_IMR_TM_LOW_WT, ENABLE);
}

/**
  * @brief  Enable or Disable specified Thermal fields to be programmed.
  * @param  NewState: ENABLE/DISABLE.
  *         @arg ENABLE: Fill TM_PWR field with right password 0x69.
  *         @arg DISABLE: Fill TM_PWR field with wrong password 0x0 or others.
  * @retval None
  */
void TM_PwrProgCmd(u32 NewState)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	if (NewState != DISABLE) {
		thermal->TM_CTRL |= TM_PWR(TM_PROG_PWR);
	} else {
		thermal->TM_CTRL &= ~TM_MASK_PWR;
	}
}

/**
  * @brief  Enable or Disable the thermal peripheral.
  * @param  NewState: new state of the thermal peripheral.
  *   			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TM_Cmd(u32 NewState)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	TM_PwrProgCmd(ENABLE);

	if (NewState != DISABLE) {
		thermal->TM_CTRL |= TM_BIT_POWCUT;
		thermal->TM_CTRL |= TM_BIT_POW;
		thermal->TM_CTRL |= TM_BIT_RSTB;
	} else {
		thermal->TM_CTRL &= ~TM_BIT_RSTB;
		thermal->TM_CTRL &= ~TM_BIT_POW;
		thermal->TM_CTRL &= ~TM_BIT_POWCUT;
	}

	TM_PwrProgCmd(DISABLE);
}

/**
  * @brief  ENABLE/DISABLE  the thermal interrupt bits.
  * @param  TM_IT: specifies the thermal interrupt to be setup.
  *          This parameter can be one or combinations of the following values:
  *            @arg TM_BIT_IMR_TM_LOW_WT:	thermal low temperature warning interrupt
  *            @arg TM_BIT_IMR_TM_HIGH_WT:	thermal high temperature warning interrupt
  * @param  NewState: ENABLE/DISABLE.
  * @retval  None
  */
void TM_INTConfig(u32 TM_IT, u32 NewState)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	if (NewState != DISABLE) {
		thermal->TM_INTR_CTRL |= TM_IT;
	} else {
		thermal->TM_INTR_CTRL &= ~TM_IT;
	}
}

/**
  * @brief	 Clears all the thermal interrupt pending bits.
  * @param  None
  * @retval  None
  */
void TM_INTClear(void)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	/* Clear the all IT pending Bits */
	thermal->TM_INTR_STS = (TM_BIT_ISR_TM_LOW_WT | TM_BIT_ISR_TM_HIGH_WT);
}

/**
  * @brief  Clears the thermal interrupt pending bits.
  * @param  TM_IT: specifies the pending bit to clear.
  *   This parameter can be any combination of the following values:
  *            @arg TM_BIT_ISR_TM_LOW_WT
  *            @arg TM_BIT_ISR_TM_HIGH_WT
  * @retval  None
  */
void TM_INTClearPendingBits(u32 TM_IT)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	thermal->TM_INTR_STS = TM_IT;
}

/**
  * @brief  Get thermal interrupt status.
  * @param  None
  * @retval  Current Interrupt Status, each bit of this value represents one
  *		interrupt status which is as follows:
  *            @arg TM_BIT_ISR_TM_LOW_WT
  *            @arg TM_BIT_ISR_TM_HIGH_WT
  */
u32 TM_GetISR(void)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	return thermal->TM_INTR_STS;
}

/**
  * @brief  Get thermal temperature result.
  * @param  None.
  * @retval  The measured temperature.
  */
u32 TM_GetTempResult(void)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	return TM_GET_OUT(thermal->TM_RESULT);
}

/**
  * @brief  Get thermal power on temperature result.
  * @param  None.
  * @retval  The measured power on temperature.
  */
u32 TM_GetPowOnTemp(void)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	return TM_GET_TEMP_OUT_POWERON(thermal->TM_OUT_PWR_ON);
}

/**
  * @brief  Get thermal max temperature result.
  * @param  None.
  * @retval  The measured max temperature.
  */
u32 TM_GetMaxTemp(void)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	return TM_GET_MAX(thermal->TM_MAX_CTRL);
}

/**
  * @brief  Get thermal min temperature result.
  * @param  None.
  * @retval  The measured min temperature.
  */
u32 TM_GetMinTemp(void)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	return TM_GET_MIN(thermal->TM_MIN_CTRL);
}

/**
  * @brief  Clear the max temperature recorded by thermal.
  * @note tm_max_clr bit sets to 1, the record max temperature is cleared;
  *         tm_max_clr bit sets to 0, thermal start to record the max temperature.
  * @retval None
  */
void TM_MaxTempClr(void)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	/* tm_max_clr bit will be cleared automatically in the next period when set to 1 */
	thermal->TM_MAX_CTRL |= TM_BIT_MAX_CLR;
}

/**
  * @brief  Clear the min temperature recorded by thermal.
  * @note tm_min_clr bit sets to 1, the record min temperature is cleared;
  *         tm_min_clr bit sets to 0, thermal start to record the min temperature.
  * @retval None
  */
void TM_MinTempClr(void)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	/* tm_min_clr bit will be cleared automatically in the next period when set to 1 */
	thermal->TM_MIN_CTRL |= TM_BIT_MIN_CLR;
}

/**
  * @brief  Enable or Disable latch function.
  * @param  NewState: new state of the thermal latch.
  *   			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TM_SetLatch(u32 NewState)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	if (NewState != DISABLE) {
		thermal->TM_CTRL |= TM_BIT_EN_LATCH;
	} else {
		thermal->TM_CTRL &= ~TM_BIT_EN_LATCH;
	}
}

/**
  * @brief  Configure over-temp protect threshold for comparison with TEMP_OUT.
  * @param  TM_HighPtThre: When TEMP_OUT > TM_HighPtThre, it will set aon reset.
  * @note   Only between 0x046 (70��C) and 0x08C (140��C) are valid.
  * @param  NewState: new state of the thermal over-temp protect comparison.
  *   			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TM_HighPtConfig(u16 TM_HighPtThre, u32 NewState)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	TM_PwrProgCmd(ENABLE);

	if (NewState != DISABLE) {
		assert_param((TM_HighPtThre >= 0x46) && (TM_HighPtThre <= 0x8C));
		thermal->TM_TH_CTRL |= (TM_BIT_HIGHCMP_PT_EN | TM_HIGH_PT_THR(TM_HighPtThre));
	} else {
		thermal->TM_TH_CTRL &= ~TM_BIT_HIGHCMP_PT_EN;
	}

	TM_PwrProgCmd(DISABLE);
}

/**
  * @brief  Configure over-temp warning threshold for comparison with TEMP_OUT.
  * @param  TM_HighWtThre: When TM_HighWtThre <= TEMP_OUT < TM_HighPtThre,  it will set int pending flag ISR_TM_HIGH.
  * @note   Only values greater than 0 are supported.
  * @param  NewState: new state of the thermal over-temp warning comparison.
  *   			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TM_HighWtConfig(u16 TM_HighWtThre, u32 NewState)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	if (NewState != DISABLE) {
		assert_param(TM_HighWtThre < 0x100);
		thermal->TM_TH_CTRL |= (TM_BIT_HIGHCMP_WT_EN | TM_HIGH_WT_THR(TM_HighWtThre));
	} else {
		thermal->TM_TH_CTRL &= ~TM_BIT_HIGHCMP_WT_EN;
	}

}

/**
  * @brief  Configure low-temp warning threshold for comparison with TEMP_OUT.
  * @param  TM_LowWtThre: When TEMP_OUT <= TM_LowWtThre, it will set int pending flag ISR_TM_LOW.
  * @note   Only values greater than 0 are supported.
  * @param  NewState: new state of the thermal low-temp warning comparison.
  *   			This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TM_LowWtConfig(u16 TM_LowWtThre, u32 NewState)
{
	THERMAL_TypeDef *thermal = TM_DEV;

	if (TrustZone_IsSecure()) {
		thermal = TM_DEV_S;
	}

	if (NewState != DISABLE) {
		assert_param(TM_LowWtThre < 0x200);
		thermal->TM_TH_CTRL |= (TM_BIT_LOWCMP_WT_EN | TM_LOW_WT_THR(TM_LowWtThre));
	} else {
		thermal->TM_TH_CTRL &= ~TM_BIT_LOWCMP_WT_EN;
	}

}

/******************* (C) COPYRIGHT 2020 Realtek Semiconductor *****END OF FILE****/
