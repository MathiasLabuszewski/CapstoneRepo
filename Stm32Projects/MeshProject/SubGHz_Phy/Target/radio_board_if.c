/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    radio_board_if.c
  * @author  MCD Application Team
  * @brief   This file provides an interface layer between MW and Radio Board
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "radio_board_if.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions --------------------------------------------------------*/
int32_t RBI_Init(void)
{
  /* USER CODE BEGIN RBI_Init_1 */

  /* USER CODE END RBI_Init_1 */

  /* 2/ Or implement RBI_Init here */
  int32_t retcode = 0;
  /* USER CODE BEGIN RBI_Init_2 */
#warning user to provide its board code or to call his board driver functions
  /* USER CODE END RBI_Init_2 */
  return retcode;
}

int32_t RBI_DeInit(void)
{
  /* USER CODE BEGIN RBI_DeInit_1 */

  /* USER CODE END RBI_DeInit_1 */

  /* 2/ Or implement RBI_DeInit here */
  int32_t retcode = 0;
  /* USER CODE BEGIN RBI_DeInit_2 */
#warning user to provide its board code or to call his board driver functions
  /* USER CODE END RBI_DeInit_2 */
  return retcode;
}

int32_t RBI_ConfigRFSwitch(RBI_Switch_TypeDef Config)
{
  /* USER CODE BEGIN RBI_ConfigRFSwitch_1 */

  /* USER CODE END RBI_ConfigRFSwitch_1 */

  /* 2/ Or implement RBI_ConfigRFSwitch here */
  int32_t retcode = 0;
  /* USER CODE BEGIN RBI_ConfigRFSwitch_2 */
#warning user to provide its board code or to call his board driver functions
  /* USER CODE END RBI_ConfigRFSwitch_2 */
  return retcode;
}

int32_t RBI_GetTxConfig(void)
{

  /* 2/ Or implement RBI_GetTxConfig here */
  int32_t retcode = RBI_CONF_RFO;
  /* USER CODE BEGIN RBI_GetTxConfig_2 */
#warning user to provide its board code or to call his board driver functions
  /* USER CODE END RBI_GetTxConfig_2 */
  return retcode;
}

int32_t RBI_IsTCXO(void)
{

  /* 2/ Or implement RBI_IsTCXO here */
  int32_t retcode = IS_TCXO_SUPPORTED;
  /* USER CODE BEGIN RBI_IsTCXO_2 */
#warning user to provide its board code or to call his board driver functions
  /* USER CODE END RBI_IsTCXO_2 */
  return retcode;
}

int32_t RBI_IsDCDC(void)
{

  /* 2/ Or implement RBI_IsDCDC here */
  int32_t retcode = IS_DCDC_SUPPORTED;
  /* USER CODE BEGIN RBI_IsDCDC_2 */
#warning user to provide its board code or to call his board driver functions
  /* USER CODE END RBI_IsDCDC_2 */
  return retcode;
}

int32_t RBI_GetRFOMaxPowerConfig(RBI_RFOMaxPowerConfig_TypeDef Config)
{
  int32_t ret = 0;
#warning Make sure we chnage this based on schip we use
  if (Config == RBI_RFO_LP_MAXPOWER)
  {
    ret = 15; /*dBm*/
  }
  else
  {
    ret = 22; /*dBm*/
  }
  return ret;
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private Functions Definition -----------------------------------------------*/
/* USER CODE BEGIN PrFD */

/* USER CODE END PrFD */
