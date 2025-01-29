#include "stm32wlxx_hal.h"
#include <string.h>
#include <stdio.h>

SPI_HandleTypeDef hspi1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

void Error_Handler();

void SPI_Flash_WriteEnable(void);
void SPI_Flash_Write(uint32_t address, uint8_t *data, uint16_t len);
void SPI_Flash_Read(uint32_t address, uint8_t *data, uint16_t len);
uint8_t SPI_Flash_ReadStatusRegister(void);
void SPI_Flash_EraseSector(uint32_t address);
void SPI_Flash_SpeedTest(void);
void SPI_Flash_DataIntegrityTest(void);



int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();


  /*Set verbose LEVEL*/

    // Test 1: Basic write and read
    uint8_t writeData[] = "Hello, SPI Flash! PLEAS EWORKKKK";
    uint8_t readData[sizeof(writeData)];

    SPI_Flash_WriteEnable();
    SPI_Flash_Write(0x0000, writeData, sizeof(writeData));
    SPI_Flash_Read(0x0000, readData, sizeof(readData));

    if (memcmp(writeData, readData, sizeof(writeData)) == 0)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // Turn on LED (success)
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // Turn off LED (failure)
    }

    // Test 2: Speed test
    SPI_Flash_SpeedTest();

    // Test 3: Data integrity test
    SPI_Flash_DataIntegrityTest();

    while (1)
    {
        // Main loop
    }
}

void SPI_Flash_WriteEnable(void)
{
    uint8_t cmd = 0x06; // Write Enable command
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); // Select the flash
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); // Deselect the flash
}

void SPI_Flash_Write(uint32_t address, uint8_t *data, uint16_t len)
{
    uint8_t cmd[4];
    cmd[0] = 0x02; // Page Program command
    cmd[1] = (address >> 16) & 0xFF;
    cmd[2] = (address >> 8) & 0xFF;
    cmd[3] = address & 0xFF;

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); // Select the flash
    HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); // Deselect the flash

    // Wait for the write to complete
    while (SPI_Flash_ReadStatusRegister() & 0x01);
}

void SPI_Flash_Read(uint32_t address, uint8_t *data, uint16_t len)
{
    uint8_t cmd[4];
    cmd[0] = 0x03; // Read Data command
    cmd[1] = (address >> 16) & 0xFF;
    cmd[2] = (address >> 8) & 0xFF;
    cmd[3] = address & 0xFF;

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); // Select the flash
    HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, data, len, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); // Deselect the flash
}

uint8_t SPI_Flash_ReadStatusRegister(void)
{
    uint8_t cmd = 0x05; // Read Status Register command
    uint8_t status;

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); // Select the flash
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, &status, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); // Deselect the flash

    return status;
}

void SPI_Flash_EraseSector(uint32_t address)
{
    uint8_t cmd[4];
    cmd[0] = 0x20; // Sector Erase command
    cmd[1] = (address >> 16) & 0xFF;
    cmd[2] = (address >> 8) & 0xFF;
    cmd[3] = address & 0xFF;

    SPI_Flash_WriteEnable();
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); // Select the flash
    HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); // Deselect the flash

    // Wait for the erase to complete
    while (SPI_Flash_ReadStatusRegister() & 0x01);
}

void SPI_Flash_SpeedTest(void)
{
    uint8_t buffer[256];
    uint32_t startTime, endTime;

    // Fill buffer with test data
    for (int i = 0; i < sizeof(buffer); i++)
    {
        buffer[i] = i & 0xFF;
    }

    // Measure write speed
    startTime = HAL_GetTick();
    SPI_Flash_WriteEnable();
    SPI_Flash_Write(0x1000, buffer, sizeof(buffer));
    endTime = HAL_GetTick();
    uint32_t writeT = endTime - startTime;

    // Measure read speed
    startTime = HAL_GetTick();
    SPI_Flash_Read(0x1000, buffer, sizeof(buffer));
    endTime = HAL_GetTick();
    uint32_t readT =  endTime - startTime;
}

void SPI_Flash_DataIntegrityTest(void)
{
    uint8_t writeBuffer[512];
    uint8_t readBuffer[512];

    // Fill write buffer with test data
    for (int i = 0; i < sizeof(writeBuffer); i++)
    {
        writeBuffer[i] = i & 0xFF;
    }

    // Erase sector before writing
    SPI_Flash_EraseSector(0x2000);

    // Write data
    SPI_Flash_WriteEnable();
    SPI_Flash_Write(0x2000, writeBuffer, sizeof(writeBuffer));

    // Read data back
    SPI_Flash_Read(0x2000, readBuffer, sizeof(readBuffer));

    // Verify data integrity
    if (memcmp(writeBuffer, readBuffer, sizeof(writeBuffer)) == 0)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // Turn on LED (success)
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // Turn off LED (failure)
    }
}

static void MX_SPI1_Init(void)
{
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 7;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        // Initialization Error
        while(1);
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure NSS pin (PB9)
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Configure LED pin (PB5)
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Configure SPI pins
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3|RCC_CLOCKTYPE_HCLK
                              |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


void Error_Handler(){
	while(1);
}
