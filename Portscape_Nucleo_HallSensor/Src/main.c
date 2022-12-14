/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

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
ADC_HandleTypeDef hadc1;

DAC_HandleTypeDef hdac;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_NVIC_Init(void);
/*-- My code --*/
void Home_Offset(void);
void Mode_Home(void);
void Mode_Home2(void);
void Mode_Home3(void);
void Home_Null_Offset(void);

void OSC_MODE_HOME(void);
void OSC_MODE_HOME2(void);
void MainWhile_Homing(void);
/*-- My code ends --*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*-- My code --*/
   bool stepA=1,stepB=0,stepC=0,stepCA=0,stepAB=0,stepBC=0,pendingPhase=1, moveMotor =1;
   uint16_t oldhallstate =0, presenthallstate=0,count =0,oldAngle=0,execute = 0,waitToMoveCounter =0;
   int16_t pulse = 100;
   uint8_t MotorHallState = 0;
   uint16_t buffer[40];
   uint8_t no_of_times = 0;
//   uint8_t homingOffset = 0;

   uint16_t Null_Offset = 0;
/*-- My code ends --*/
/* USER CODE END 0 */
#define STATE_0 (uint8_t)0
#define STATE_1 (uint8_t)1
#define STATE_2 (uint8_t)2
#define STATE_3 (uint8_t)3
#define STATE_4 (uint8_t)4
#define STATE_5 (uint8_t)5
#define STATE_6 (uint8_t)6
#define STATE_7 (uint8_t)7

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_MotorControl_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
     __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
     __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
     HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
     HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
     HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}



void Home_Null_Offset(){
//	Null_Offset = 0;
	no_of_times++;
	Null_Offset = Null_Offset + HALL_M1.HallStateCounter;
//	int temp = HALL_M1.HallStateCounter;
//
//	if(temp != Null_Offset)
//	{
//		if(temp>Null_Offset){
//			Null_Offset += ( temp - Null_Offset);
//		}
//		else{
//			Null_Offset += (Null_Offset - temp);
//		}
//	}
}
void Mode_Home(){

	uint8_t HallState_Internal = HALL_M1.HallState;
	uint8_t HallStateCounter_Internal =  HALL_M1.HallStateCounter + Null_Offset;// + (Null_Offset/no_of_times);
	uint8_t HallState_Home = HALL_M1.HallStateHomePosition;
	uint8_t HallStateCounter_Home = HALL_M1.HallCounterHomePosition;
	uint8_t tomove = ceil(Null_Offset/no_of_times);


//	if(HallStateCounter_Internal < tomove)
//	{
//		HallStateCounter_Internal = 72 - HallStateCounter_Internal;
//	}
//	else{
//		HallStateCounter_Internal = tomove - HallStateCounter_Internal;
//	}

//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Input:  %u\r\n", HallState_Internal), 100);
	while (HallStateCounter_Internal != HallStateCounter_Home){ //&&(HallState_Home != HallState_Home)
		switch(HallState_Internal){
		//step 1
				case STATE_6:
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 2
				case STATE_1:
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 1, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 3, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 3
				case STATE_2:
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 2, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 1, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 4
				case STATE_3:
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
						  HallState_Internal++;
						  HallStateCounter_Internal++;
						  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 3, HallState:  %u\r\n", HallState_Internal), 100);
//						  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//						  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 5
				case STATE_4:
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 4, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 4, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 6
				case STATE_5:
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 2, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				default:
					HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Error, HallState:  %u\r\n", HallState_Internal), 100);
//					HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Error, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
		}

		HallStateCounter_Internal = HallStateCounter_Internal%72;

		if(HallState_Internal ==7)
		{
			HallState_Internal =1;
		}

	}
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
	  //NOte this two lines
//	  HALL_M1.HallStateCounter = 0;
	  HALL_M1.HallStateCounter = HALL_M1.HallCounterHomePosition;
	  HALL_M1.HallStateHomePosition = HALL_M1.HallState;
	  no_of_times = 0;
	  Null_Offset = 0;
}

//void Home_Offset(){
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "---------------------------------------\n"), 100);
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Prev STATE COunter: %u\r\n", HALL_M1.HallCounterHomePosition), 100);
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t PRev Hall POSITION: %u\r\n", HALL_M1.HallStateHomePosition), 100);
//	HALL_M1.HallCounterHomePosition = HALL_M1.HallStateCounter +1;
////	homingOffset = HALL_M1.HallStateCounter +1;
////	homingOffset +=1;
////	homingOffset = homingOffset%72;
//	//HALL_M1.HallStateHomePosition = HALL_M1.HallState;
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t New STATE COunter: %u\r\n", HALL_M1.HallCounterHomePosition), 100);
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t New Hall POSITION: %u\r\n", HALL_M1.HallStateHomePosition), 100);
//	Mode_Home2();
////	HALL_M1.HallStateCounter = HALL_M1.HallCounterHomePosition;
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "---------------------------------------\n"), 100);
//}

void Home_Offset(){
	// 21 01 10 34
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Prev HALL homrCOunter: %u\r\n", HALL_M1.HallCounterHomePosition), 100);
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t PRev Hall State home POSITION: %u\r\n", HALL_M1.HallStateHomePosition), 100);
	HALL_M1.HallCounterHomePosition = HALL_M1.HallCounterHomePosition +1;
//	HALL_M1.HallStateHomePosition = HALL_M1.HallState;
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t New HAll home COunter: %u\r\n", HALL_M1.HallCounterHomePosition), 100);
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t New Hall home State POSITION: %u\r\n", HALL_M1.HallStateHomePosition), 100);
	Mode_Home3();
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Final HAll Home COunter: %u\r\n", HALL_M1.HallCounterHomePosition), 100);
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Final Hall state home POSITION: %u\r\n", HALL_M1.HallStateHomePosition), 100);
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Final hall COunter: %u\r\n", HALL_M1.HallStateCounter), 100);
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Final Hall POSITION: %u\r\n", HALL_M1.HallState), 100);
}
void Mode_Home3(){

	uint8_t HallState_Internal = HALL_M1.HallState;
	uint8_t HallStateCounter_Internal = HALL_M1.HallStateCounter;
	uint8_t HallState_Home = HALL_M1.HallStateHomePosition;
	uint8_t HallStateCounter_Home = HALL_M1.HallCounterHomePosition;

	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Input:  %u\r\n", HallState_Internal), 100);
//	while (HallStateCounter_Internal != HallStateCounter_Home){ //&&(HallState_Home != HallState_Home)
		switch(HallState_Internal){
		//step 1
				case STATE_6:
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 2
				case STATE_1:
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 1, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 3, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 3
				case STATE_2:
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 2, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 1, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 4
				case STATE_3:
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
						  HallState_Internal++;
						  HallStateCounter_Internal++;
						  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 3, HallState:  %u\r\n", HallState_Internal), 100);
//						  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//						  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 5
				case STATE_4:
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 4, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 4, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 6
				case STATE_5:
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 2, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				default:
					HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Error, HallState:  %u\r\n", HallState_Internal), 100);
//					HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Error, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
		}

		HallStateCounter_Internal = HallStateCounter_Internal%72;

		if(HallState_Internal ==7)
		{
			HallState_Internal =1;
		}

//	}
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
	  //NOte this two lines
//	  HALL_M1.HallStateCounter = 0;
	  HALL_M1.HallStateCounter = HALL_M1.HallCounterHomePosition;
	  HALL_M1.HallStateHomePosition = HALL_M1.HallState;
}
void Mode_Home2(){
	uint8_t HallState_Internal = HALL_M1.HallState;
	uint8_t HallStateCounter_Internal = HALL_M1.HallStateCounter;
	uint8_t HallState_Home = HALL_M1.HallStateHomePosition;
	uint8_t HallStateCounter_Home = HALL_M1.HallCounterHomePosition;// + homingOffset;

	while (HallStateCounter_Internal != HallStateCounter_Home){ //&&(HallState_Home != HallState_Home)
		switch(HallState_Internal){
			   //step 1
			   case STATE_6://STATE_2:
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//				      for(int i =0; i<5000;i++);
					break;
				//step 2
				case STATE_1://STATE_3:
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 1, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 3, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//				      for(int i =0; i<5000;i++);
					break;
				//step 3
				case STATE_2://STATE_4:
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 2, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 1, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//				      for(int i =0; i<5000;i++);
					break;
				//step 4
				case STATE_3://STATE_5:
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
						  HallState_Internal++;
						  HallStateCounter_Internal++;
						  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 3, HallState:  %u\r\n", HallState_Internal), 100);
//						  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//						  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//					      for(int i =0; i<5000;i++);
					break;
				//step 5
				case STATE_4://STATE_6:
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 4, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 4, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//				      for(int i =0; i<5000;i++);
					break;
				//step 6
				case STATE_5://STATE_1:
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 2, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//				      for(int i =0; i<5000;i++);
					break;
				default:
					HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Error, HallState:  %u\r\n", HallState_Internal), 100);
//					HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t Error, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//					for(int i =0; i<5000;i++);
					break;
		}

		HallStateCounter_Internal = HallStateCounter_Internal%72;
		if(HallState_Internal ==7)
		{
			HallState_Internal =1;
		}

	}
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
	  HALL_M1.HallStateCounter = HALL_M1.HallCounterHomePosition;
	  HALL_M1.HallStateHomePosition = HALL_M1.HallState;
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_PLLCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM1_BRK_TIM15_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 4, 1);
  HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
  /* TIM1_UP_TIM16_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
  /* ADC1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(ADC1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(ADC1_IRQn);
  /* TIM2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 3, 1);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* EXTI15_10_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_InjectionConfTypeDef sConfigInjected = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_LEFT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Injected Channel
  */
  sConfigInjected.InjectedChannel = ADC_CHANNEL_1;
  sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;
  sConfigInjected.InjectedSingleDiff = ADC_SINGLE_ENDED;
  sConfigInjected.InjectedNbrOfConversion = 3;
  sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_19CYCLES_5;
  sConfigInjected.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONV_EDGE_RISING;
  sConfigInjected.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJECCONV_T1_TRGO;
  sConfigInjected.AutoInjectedConv = DISABLE;
  sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
  sConfigInjected.QueueInjectedContext = ENABLE;
  sConfigInjected.InjectedOffset = 0;
  sConfigInjected.InjectedOffsetNumber = ADC_OFFSET_NONE;
  if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Injected Channel
  */
  sConfigInjected.InjectedChannel = ADC_CHANNEL_7;
  sConfigInjected.InjectedRank = ADC_INJECTED_RANK_2;
  if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Injected Channel
  */
  sConfigInjected.InjectedChannel = ADC_CHANNEL_6;
  sConfigInjected.InjectedRank = ADC_INJECTED_RANK_3;
  if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_SOFTWARE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = ((TIM_CLOCK_DIVIDER) - 1);
  htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim1.Init.Period = ((PWM_PERIOD_CYCLES) / 2);
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim1.Init.RepetitionCounter = (REP_COUNTER);
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_TRIGGER;
  sSlaveConfig.InputTrigger = TIM_TS_ITR1;
  if (HAL_TIM_SlaveConfigSynchro(&htim1, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC4REF;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = ((PWM_PERIOD_CYCLES) / 4);
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = (((PWM_PERIOD_CYCLES) / 2) - (HTMIN));
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_1;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_ENABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_LOW;
  sBreakDeadTimeConfig.Break2Filter = 7;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_HallSensor_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = M1_HALL_TIM_PERIOD;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = M1_HALL_IC_FILTER;
  sConfig.Commutation_Delay = 0;
  if (HAL_TIMEx_HallSensor_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC2REF;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, M1_PWM_EN_U_Pin|M1_PWM_EN_V_Pin|M1_PWM_EN_W_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Start_Stop_Pin */
  GPIO_InitStruct.Pin = Start_Stop_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Start_Stop_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : M1_PWM_EN_U_Pin M1_PWM_EN_V_Pin M1_PWM_EN_W_Pin */
  GPIO_InitStruct.Pin = M1_PWM_EN_U_Pin|M1_PWM_EN_V_Pin|M1_PWM_EN_W_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
