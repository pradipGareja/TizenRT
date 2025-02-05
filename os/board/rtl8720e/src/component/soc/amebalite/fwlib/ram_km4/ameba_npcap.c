/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

//KM4 is NP, KM4 control KR4

static u32 KR4SleepTick = 0;
static u32 kr4_sleep_type;

u32 kr4_status_on(void)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKE_GRP0);

	if (Temp & APBPeriph_KR4_CLOCK) {
		return 1;
	} else {
		return 0;
	}
}

void kr4_power_gate(void)
{
	u32 Rtemp;
	u32 timeout = 20;

	if (!kr4_status_on()) {
		return;
	}

	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "R4G\n");

	if (HAL_READ32(PMC_BASE, SYSPMC_CTRL) & PMC_BIT_KR4_IRQ_MASK) {
	} else {
		DBG_8195A("KR4 exit WFI\n");
		return;
	}

	while (timeout > 0) {
		if (HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PLAT_STATUS) & LSYS_BIT_KR4_SLP_MODE) {	/*get KR4 sleep status*/
			break;
		}
		timeout--;
		if (timeout == 0) {
			DBG_8195A("KR4 wake, no need to close KR4\n");
			return;
		}
	}

	RCC_PeriphClockCmd(APBPeriph_KR4, APBPeriph_KR4_CLOCK, DISABLE);

	/*unmask kr4 irqs*/
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
	Rtemp &= (~PMC_BIT_KR4_IRQ_MASK);
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

	pmu_release_wakelock(PMU_KR4_RUN);
	pmu_release_wakelock(PMU_OS);
}

void kr4_power_wake(void)
{
	if (kr4_status_on()) {
		return;
	}

	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "R4W\n");

	RCC_PeriphClockCmd(APBPeriph_KR4, APBPeriph_KR4_CLOCK, ENABLE);
}

void kr4_clock_gate(void)
{
	u32 Rtemp;
	u32 timeout = 20;

	if (!kr4_status_on()) {
		return;
	}

	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "R4CG\n");

	if (HAL_READ32(PMC_BASE, SYSPMC_CTRL) & PMC_BIT_KR4_IRQ_MASK) {
	} else {
		DBG_8195A("KR4 exit WFI\n");
		return;
	}

	while (timeout > 0) {
		if (HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PLAT_STATUS) & LSYS_BIT_KR4_SLP_MODE) {	/*get KR4 sleep status*/
			break;
		}
		timeout--;
		if (timeout == 0) {
			DBG_8195A("KR4 wake, no need to close KR4\n");
			return;
		}
	}

	RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_KR4_CLOCK, DISABLE);

	/*unmask kr4 irqs*/
	Rtemp = HAL_READ32(PMC_BASE, SYSPMC_CTRL);
	Rtemp &= (~PMC_BIT_KR4_IRQ_MASK);
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, Rtemp);

	pmu_release_wakelock(PMU_KR4_RUN);
	pmu_release_wakelock(PMU_OS);
}

void kr4_clock_on(void)
{
	if (kr4_status_on()) {
		return;
	}

	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "R4CW\n");

	RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_KR4_CLOCK, ENABLE);
}

u32 NPWAP_INTHandler(UNUSED_WARN_DIS void *Data)
{
	UNUSED(Data);
	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "NP WAKE AP HANDLER %x %x\n", \
			   HAL_READ32(PMC_BASE, WAK_STATUS0), HAL_READ32(PMC_BASE, WAK_STATUS1));

	InterruptDis(NP_WAKE_IRQ);

	kr4_resume();

	return TRUE;
}

u32 aontimer_int_wakeup_ap(UNUSED_WARN_DIS void *Data)
{
	UNUSED(Data);

	SOCPS_AONTimerClearINT();
	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "wakereason: 0x%x\n", SOCPS_AONWakeReason());
	RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, DISABLE);

	IPC_MSG_STRUCT ipc_msg_temp;
	ipc_msg_temp.msg_type = IPC_USER_POINT;
	ipc_msg_temp.msg = TIMER_NPWAP_IPC;
	ipc_msg_temp.msg_len = 1;
	ipc_msg_temp.rsvd = 0;
	ipc_send_message(IPC_KM4_TO_KR4, IPC_M2R_WAKE_AP, &ipc_msg_temp);

	return TRUE;
}

u32 kr4_suspend(u32 type)
{
	u32 ret = _SUCCESS;
	SLEEP_ParamDef *sleep_param;
	u32 duration = 0;
	u32 sleep_wevent_config_val[2] = {0};

	sleep_param = (SLEEP_ParamDef *)ipc_get_message(IPC_KR4_TO_KM4, IPC_R2M_TICKLESS_INDICATION);

	if (sleep_param != NULL) {
		duration = sleep_param->sleep_time;
	}

	if (duration > 0) {
		DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "duration: %d\n", duration);
		RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, ENABLE);
		SOCPS_AONTimer(duration);
		SOCPS_SetNPWakeEvent_MSK1(WAKE_SRC_AON_TIM, ENABLE);
		SOCPS_AONTimerINT_EN(ENABLE);
		InterruptRegister(aontimer_int_wakeup_ap, AON_TIM_IRQ, (u32)PMC_BASE, INT_PRI3);
		InterruptEn(AON_TIM_IRQ, INT_PRI3);
	}

	/*clean np wake pending interrupt*/
	NVIC_ClearPendingIRQ(NP_WAKE_IRQ);

	sleep_wevent_config_val[1] = HAL_READ32(PMC_BASE, WAK_MASK1_AP);
	sleep_wevent_config_val[0] = HAL_READ32(PMC_BASE, WAK_MASK0_AP);

	if ((sleep_wevent_config_val[0] | sleep_wevent_config_val[1])) {
		DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "register np_wake_irq\n");
		InterruptRegister(NPWAP_INTHandler, NP_WAKE_IRQ, (u32)PMC_BASE, INT_PRI5);
		InterruptEn(NP_WAKE_IRQ, INT_PRI5);
	}

	if (type == SLEEP_CG) {
		kr4_clock_gate();
	} else {
		kr4_power_gate();
	}

	SOCPS_NP_suspend_config(type);

	return ret;
}

void kr4_resume(void)
{
	if (kr4_status_on()) {
		return;
	}

	pmu_acquire_wakelock(PMU_KR4_RUN);

	SOCPS_NP_resume_config(kr4_sleep_type);

	if (kr4_sleep_type == SLEEP_CG) {
		kr4_clock_on();
	} else {
		kr4_power_wake();
	}
}

void kr4_tickless_ipc_int(UNUSED_WARN_DIS VOID *Data, UNUSED_WARN_DIS u32 IrqStatus, UNUSED_WARN_DIS u32 ChanNum)
{
	/* To avoid gcc warnings */
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);
	u32 Rtemp;
	SLEEP_ParamDef *psleep_param;

	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "kr4_tickless_ipc_int\n");
	psleep_param = (SLEEP_ParamDef *)ipc_get_message(IPC_KR4_TO_KM4, IPC_R2M_TICKLESS_INDICATION);
	DCache_Invalidate((u32)psleep_param, sizeof(SLEEP_ParamDef));

	//TODO: set dlps
	if (psleep_param->dlps_enable) {
		RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, ENABLE);
		if (psleep_param->sleep_time) {
			SOCPS_AONTimerClearINT();
			SOCPS_AONTimer(psleep_param->sleep_time);
			SOCPS_AONTimerINT_EN(ENABLE);
		}
		SOCPS_DeepSleep_RAM();
	}

	kr4_sleep_type = psleep_param->sleep_type;

	switch (psleep_param->sleep_type) {
	case SLEEP_PG:
		if (_SUCCESS == kr4_suspend(SLEEP_PG)) {
			Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_BOOT_CFG);
			Rtemp |= LSYS_BIT_BOOT_WAKE_FROM_PS_LS;
			HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_BOOT_CFG, Rtemp);
		}
		break;
	case SLEEP_CG:
		if (_SUCCESS == kr4_suspend(SLEEP_CG)) {
			pmu_set_sysactive_timeFromISR(2);
		}
		break;

	default:
		DBG_8195A("unknow sleep type\n");
	}

	KR4SleepTick = SYSTIMER_TickGet();
//	DBG_PRINTF(MODULE_KM4, LEVEL_INFO, "T:%d, tms:%d\r\n", KR4SleepTick, (((KR4SleepTick & 0xFFFF8000) / 32768) * 1000 + ((KR4SleepTick & 0x7FFF) * 1000) / 32768));
}

/**
  * @brief  Get kr4 sleep time.
  * @param  null.
  * @retval kr4 sleep time in ms.
  */
uint32_t pmu_get_kr4sleeptime(void)
{
	u32 current_tick = 0, kr4sleeptime = 0;

	current_tick = SYSTIMER_TickGet();

	if (current_tick >= KR4SleepTick) {
		kr4sleeptime = current_tick - KR4SleepTick;
	} else {
		kr4sleeptime = (0xFFFFFFFF - KR4SleepTick) + current_tick;
	}

	kr4sleeptime = ((kr4sleeptime & 0xFFFF8000) / 32768) * 1000 + ((kr4sleeptime & 0x7FFF) * 1000) / 32768;

	return kr4sleeptime;
}

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   ipc_tickless_table[] = {
	{IPC_USER_DATA, 	kr4_tickless_ipc_int,	(VOID *) NULL, IPC_KR4_TO_KM4, IPC_R2M_TICKLESS_INDICATION, IPC_RX_FULL},
};
