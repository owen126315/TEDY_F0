/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* USER CODE BEGIN 0 */
#include "NDEF_Message.h"
#include "Mifare_Classic.h"
#include "NFC_Tag.h"
#include "FN_RM01.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
extern NFC_Tag tag;
extern int file_num;
extern char file_name[4];
extern int is_recording;
/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
        * Free pins are configured automatically as Analog (this feature is enabled through 
        * the Code Generation settings)
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PF0 PF1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA5 PA6 
                           PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5|GPIO_PIN_6 
                          |GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 2 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
//	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == 0) //button pressed
//	{
//		if(PN532_InListPassiveTarget(PN532_MIFARE_ISO14443A, &tag))
//		{
//			FN_RM01_Play_Sound("reco");
//			FN_RM01_Get_FileName(file_num, file_name);
//
//			NDEF_Record_Create_TextRecord(file_name, sizeof(file_name), &tag.ndefMessage.record[0]);
//			tag.ndefMessage.record_Count++;
//
//			if(PN532_Write_Tag(&tag))
//			{
//				FN_RM01_Record_Sound(file_name);
//				is_recording = 1;
//			}
//			NFC_Clear_Tag(&tag);
//		}
//	}
//
//	else //button release
//	{
//		if(is_recording)
//		{
//			FN_RM01_Stop_Record();
//
//			if(PN532_InListPassiveTarget(PN532_MIFARE_ISO14443A, &tag))
//			{
//				if(PN532_Read_Tag(&tag))
//				{
//					if(memcmp(tag.ndefMessage.record[0].payload+3, file_name, 4) == 0)
//					{
//						FN_RM01_Play_Sound("done");
//						file_num++;
//					}
//					else
//						FN_RM01_Remove_Sound(file_name);
//				}
//				NFC_Clear_Tag(&tag);
//			}
//			else
//				FN_RM01_Remove_Sound(file_name);
//
//			is_recording = 0;
//		}
//	}


	if(PN532_InListPassiveTarget(PN532_MIFARE_ISO14443A, &tag))
	{
		FN_RM01_Get_FileName(file_num, file_name);

		NDEF_Record_Create_TextRecord(file_name, sizeof(file_name), &tag.ndefMessage.record[0]);
		tag.ndefMessage.record_Count++;

		if(PN532_Write_Tag(&tag))
		{
			FN_RM01_Play_Sound("reco");
			HAL_Delay(600);
			FN_RM01_Record_Sound(file_name);

			NFC_Clear_Tag(&tag);

			while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));

			FN_RM01_Stop_Record();

			if(PN532_InListPassiveTarget(PN532_MIFARE_ISO14443A, &tag))
			{
				if(PN532_Read_Tag(&tag))
				{
					if(memcmp(tag.ndefMessage.record[0].payload+3, file_name, 4) == 0)
					{
						FN_RM01_Play_Sound("done");
						HAL_Delay(600);
						file_num++;
					}
					else
						FN_RM01_Remove_Sound(file_name);
				}
				NFC_Clear_Tag(&tag);
			}
			else
				FN_RM01_Remove_Sound(file_name);
		}
		HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
	}
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
