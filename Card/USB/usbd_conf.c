/// -------------------------------------------------------------------------
// USB Config - STM32H743
// Developed by Soren Afzar Co. (www.sorenafzar.ir)
// First Version : October 2019
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#include <usbd_core.h>
#include <usbd_cdc.h>
// -------------------------------------------------------------------------
PCD_HandleTypeDef hpcd;
// -------------------------------------------------------------------------
void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	HAL_PWREx_EnableUSBVoltageDetector();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin = (GPIO_PIN_11 | GPIO_PIN_12);
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_FS;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	__HAL_RCC_USB2_OTG_FS_CLK_ENABLE();

#ifdef FREERTOS_ENABLE
	HAL_NVIC_SetPriority(OTG_FS_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 6, 0);
#else
	HAL_NVIC_SetPriority(OTG_FS_IRQn, 6, 0);
#endif
	HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}
// -------------------------------------------------------------------------
void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)
{
	if (hpcd->Instance == USB2_OTG_FS)
	{
		__HAL_RCC_USB2_OTG_FS_CLK_DISABLE();
	}
	else if (hpcd->Instance == USB1_OTG_HS)
	{
		__HAL_RCC_USB1_OTG_HS_CLK_DISABLE();
		__HAL_RCC_USB1_OTG_HS_ULPI_CLK_DISABLE();
	}
}
// -------------------------------------------------------------------------
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef* hpcd)
{
	USBD_LL_SetupStage(hpcd->pData, (uint8_t*)hpcd->Setup);
}
// -------------------------------------------------------------------------
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum)
{
	USBD_LL_DataOutStage(hpcd->pData, epnum, hpcd->OUT_ep[epnum].xfer_buff);
}
// -------------------------------------------------------------------------
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum)
{
	USBD_LL_DataInStage(hpcd->pData, epnum, hpcd->IN_ep[epnum].xfer_buff);
}
// -------------------------------------------------------------------------
void HAL_PCD_SOFCallback(PCD_HandleTypeDef* hpcd)
{
	USBD_LL_SOF(hpcd->pData);
}
// -------------------------------------------------------------------------
void HAL_PCD_ResetCallback(PCD_HandleTypeDef* hpcd)
{
	USBD_SpeedTypeDef speed = USBD_SPEED_FULL;

	switch (hpcd->Init.speed)
	{
	case PCD_SPEED_HIGH:
		speed = USBD_SPEED_HIGH;
		break;

	case PCD_SPEED_FULL:
		speed = USBD_SPEED_FULL;
		break;

	default:
		speed = USBD_SPEED_FULL;
		break;
	}

	USBD_LL_Reset(hpcd->pData);
	USBD_LL_SetSpeed(hpcd->pData, speed);
}
// -------------------------------------------------------------------------
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef* hpcd)
{
	USBD_LL_Suspend(hpcd->pData);
}
// -------------------------------------------------------------------------
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef* hpcd)
{
	USBD_LL_Resume(hpcd->pData);
}
// -------------------------------------------------------------------------
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum)
{
	USBD_LL_IsoOUTIncomplete(hpcd->pData, epnum);
}
// -------------------------------------------------------------------------
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum)
{
	USBD_LL_IsoINIncomplete(hpcd->pData, epnum);
}
// -------------------------------------------------------------------------
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef* hpcd)
{
	USBD_LL_DevConnected(hpcd->pData);
}
// -------------------------------------------------------------------------
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef* hpcd)
{
	USBD_LL_DevDisconnected(hpcd->pData);
}
// -------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef* pdev)
{
#ifdef USE_USB_FS
	hpcd.Instance = USB2_OTG_FS;
	hpcd.Init.dev_endpoints = 8;
	hpcd.Init.use_dedicated_ep1 = 0;
	hpcd.Init.ep0_mps = 0x40;
	hpcd.Init.low_power_enable = 0;
	hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
	hpcd.Init.Sof_enable = 0;
	hpcd.Init.speed = PCD_SPEED_FULL;
	hpcd.Init.vbus_sensing_enable = 0;
	hpcd.Init.lpm_enable = 0;

	hpcd.pData = pdev;
	pdev->pData = &hpcd;

	HAL_PCD_Init(&hpcd);

	HAL_PCDEx_SetRxFiFo(&hpcd, 0x80);
	HAL_PCDEx_SetTxFiFo(&hpcd, 0, 0x40);
	HAL_PCDEx_SetTxFiFo(&hpcd, 1, 0x80);

#endif

#ifdef USE_USB_HS
	hpcd.Instance = USB1_OTG_HS;
	hpcd.Init.dev_endpoints = 8;
	hpcd.Init.use_dedicated_ep1 = 0;
	hpcd.Init.ep0_mps = 0x40;
	hpcd.Init.dma_enable = 0;
	hpcd.Init.low_power_enable = 0;
	hpcd.Init.lpm_enable = 0;

#ifdef USE_USB_HS_IN_FS
	hpcd.Init.low_power_enable = 1;
	hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
#else
	hpcd.Init.phy_itface = PCD_PHY_ULPI;
#endif
	hpcd.Init.Sof_enable = 0;
	hpcd.Init.speed = PCD_SPEED_HIGH;
	hpcd.Init.vbus_sensing_enable = 0;

	hpcd.pData = pdev;
	pdev->pData = &hpcd;

	HAL_PCD_Init(&hpcd);

	HAL_PCDEx_SetRxFiFo(&hpcd, 0x200);
	HAL_PCDEx_SetTxFiFo(&hpcd, 0, 0x80);
	HAL_PCDEx_SetTxFiFo(&hpcd, 1, 0x174);
#endif

	return USBD_OK;
}
// -------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef* pdev)
{
	HAL_PCD_DeInit(pdev->pData);
	return USBD_OK;
}
// -------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef* pdev)
{
	HAL_PCD_Start(pdev->pData);
	return USBD_OK;
}
// -------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef* pdev)
{
	HAL_PCD_Stop(pdev->pData);
	return USBD_OK;
}
// -------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef* pdev, uint8_t ep_addr, uint8_t ep_type, uint16_t ep_mps)
{
	HAL_PCD_EP_Open(pdev->pData, ep_addr, ep_mps, ep_type);
	return USBD_OK;
}
// -------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef* pdev, uint8_t ep_addr)
{
	HAL_PCD_EP_Close(pdev->pData, ep_addr);
	return USBD_OK;
}
// -------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef* pdev, uint8_t ep_addr)
{
	HAL_PCD_EP_Flush(pdev->pData, ep_addr);
	return USBD_OK;
}
// -------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef* pdev, uint8_t ep_addr)
{
	HAL_PCD_EP_SetStall(pdev->pData, ep_addr);
	return USBD_OK;
}
// -------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef* pdev, uint8_t ep_addr)
{
	HAL_PCD_EP_ClrStall(pdev->pData, ep_addr);
	return USBD_OK;
}
// -------------------------------------------------------------------------
uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef* pdev, uint8_t ep_addr)
{
	PCD_HandleTypeDef* hpcd = pdev->pData;
	if ((ep_addr & 0x80) == 0x80)
	{
		return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
	}
	else
	{
		return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
	}
}
// -------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef* pdev, uint8_t dev_addr)
{
	HAL_PCD_SetAddress(pdev->pData, dev_addr);
	return USBD_OK;
}
// -------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef* pdev, uint8_t ep_addr, uint8_t* pbuf, uint32_t size)
{
	HAL_PCD_EP_Transmit(pdev->pData, ep_addr, pbuf, size);
	return USBD_OK;
}
// -------------------------------------------------------------------------
USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef* pdev, uint8_t ep_addr, uint8_t* pbuf, uint32_t size)
{
	HAL_PCD_EP_Receive(pdev->pData, ep_addr, pbuf, size);
	return USBD_OK;
}
// -------------------------------------------------------------------------
uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef* pdev, uint8_t ep_addr)
{
	return HAL_PCD_EP_GetRxCount(pdev->pData, ep_addr);
}
// -------------------------------------------------------------------------
void USBD_LL_Delay(uint32_t Delay)
{
	HAL_Delay(Delay);
}
// -------------------------------------------------------------------------
