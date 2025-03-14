/**
 * @file lcd.c
 * @brief 16x2 LCD driver implementation using STM32 HAL
 */

#include "lcd.h"

/* Private function prototypes */
static void LCD16X2_Write(uint8_t data);
static void LCD16X2_WriteCommand(uint8_t cmd);
static void LCD16X2_WriteData(uint8_t data);
static void LCD16X2_Pulse_E(void);
static void LCD16X2_Write4Bits(uint8_t nibble);
static void LCD16X2_Delay_us(uint32_t us);

/**
 * @brief Initialize the LCD
 * @param displayMode: Display mode (ON/OFF, cursor, blink)
 */
void LCD16X2_Init(uint8_t displayMode)
{
    /* Enable GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure GPIO pins */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    /* RS pin */
    GPIO_InitStruct.Pin = LCD16X2_RS_Pin;
    HAL_GPIO_Init(LCD16X2_RS_GPIO_Port, &GPIO_InitStruct);

    /* RW pin */
    GPIO_InitStruct.Pin = LCD16X2_RW_Pin;
    HAL_GPIO_Init(LCD16X2_RW_GPIO_Port, &GPIO_InitStruct);

    /* EN pin */
    GPIO_InitStruct.Pin = LCD16X2_EN_Pin;
    HAL_GPIO_Init(LCD16X2_EN_GPIO_Port, &GPIO_InitStruct);

    /* Data pins */
    GPIO_InitStruct.Pin = LCD16X2_D4_Pin;
    HAL_GPIO_Init(LCD16X2_D4_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD16X2_D5_Pin;
    HAL_GPIO_Init(LCD16X2_D5_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD16X2_D6_Pin;
    HAL_GPIO_Init(LCD16X2_D6_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD16X2_D7_Pin;
    HAL_GPIO_Init(LCD16X2_D7_GPIO_Port, &GPIO_InitStruct);

    /* Initialize LCD in 4-bit mode */
    HAL_Delay(50);  /* Wait for LCD to power up */

    /* Set RW low for write operations */
    HAL_GPIO_WritePin(LCD16X2_RW_GPIO_Port, LCD16X2_RW_Pin, GPIO_PIN_RESET);

    /* Set RS low for command */
    HAL_GPIO_WritePin(LCD16X2_RS_GPIO_Port, LCD16X2_RS_Pin, GPIO_PIN_RESET);

    /* Send reset sequence for 4-bit mode */
    LCD16X2_Write4Bits(0x03);
    HAL_Delay(5);
    LCD16X2_Write4Bits(0x03);
    HAL_Delay(1);
    LCD16X2_Write4Bits(0x03);
    HAL_Delay(1);
    LCD16X2_Write4Bits(0x02);  /* Set to 4-bit mode */
    HAL_Delay(1);

    /* Set function: 4-bit mode, 2 lines, 5x8 dots */
    LCD16X2_WriteCommand(LCD16X2_FUNCTION_SET | LCD16X2_4BIT_MODE | LCD16X2_2LINE | LCD16X2_5x8DOTS);

    /* Display control */
    LCD16X2_WriteCommand(LCD16X2_DISPLAY_CONTROL | displayMode);

    /* Clear display */
    LCD16X2_ClrScr();

    /* Entry mode set */
    LCD16X2_WriteCommand(LCD16X2_ENTRY_MODE_SET | LCD16X2_ENTRY_LEFT | LCD16X2_ENTRY_SHIFT_DEC);
}

/**
 * @brief Clear the display
 */
void LCD16X2_ClrScr(void)
{
    LCD16X2_WriteCommand(LCD16X2_CLEAR_DISPLAY);
    HAL_Delay(2);  /* Clear display command needs longer delay */
}

/**
 * @brief Return cursor to home position
 */
void LCD16X2_Home(void)
{
    LCD16X2_WriteCommand(LCD16X2_RETURN_HOME);
    HAL_Delay(2);  /* Return home command needs longer delay */
}

/**
 * @brief Set cursor position
 * @param x: Column position (0-15)
 * @param y: Row position (0-1)
 */
void LCD16X2_GotoXY(uint8_t x, uint8_t y)
{
    uint8_t address;

    /* Calculate DDRAM address: first line starts at 0x00, second line at 0x40 */
    address = (y == 0) ? (0x00 + x) : (0x40 + x);

    LCD16X2_WriteCommand(LCD16X2_SET_DDRAM_ADDR | address);
}

/**
 * @brief Write a character to the LCD
 * @param c: Character to write
 */
void LCD16X2_PutChar(char c)
{
    LCD16X2_WriteData((uint8_t)c);
}

/**
 * @brief Write a string to the LCD
 * @param s: String to write
 */
void LCD16X2_Puts(const char *s)
{
    while(*s)
    {
        LCD16X2_PutChar(*s++);
    }
}

/**
 * @brief Create a custom character
 * @param location: Character code (0-7)
 * @param charmap: Character bitmap (8 bytes)
 */
void LCD16X2_CreateCustomChar(uint8_t location, uint8_t *charmap)
{
    uint8_t i;

    /* Set CGRAM address */
    LCD16X2_WriteCommand(LCD16X2_SET_CGRAM_ADDR | ((location & 0x7) << 3));

    /* Write character pattern */
    for(i = 0; i < 8; i++)
    {
        LCD16X2_WriteData(charmap[i]);
    }

    /* Return to DDRAM address */
    LCD16X2_WriteCommand(LCD16X2_SET_DDRAM_ADDR);
}

/**
 * @brief Display custom character at specified position
 * @param x: Column position (0-15)
 * @param y: Row position (0-1)
 * @param location: Character code (0-7)
 */
void LCD16X2_PutCustomChar(uint8_t x, uint8_t y, uint8_t location)
{
    LCD16X2_GotoXY(x, y);
    LCD16X2_WriteData(location);
}

/**
 * @brief Control display mode
 * @param displayMode: Display mode (ON/OFF, cursor, blink)
 */
void LCD16X2_DisplayControl(uint8_t displayMode)
{
    LCD16X2_WriteCommand(LCD16X2_DISPLAY_CONTROL | displayMode);
}

/**
 * @brief Write a byte to the LCD
 * @param data: Byte to write
 */
static void LCD16X2_Write(uint8_t data)
{
    LCD16X2_Write4Bits(data >> 4);    /* High nibble */
    LCD16X2_Write4Bits(data & 0x0F);  /* Low nibble */
}

/**
 * @brief Write a command to the LCD
 * @param cmd: Command to write
 */
static void LCD16X2_WriteCommand(uint8_t cmd)
{
    /* Set RS low for command */
    HAL_GPIO_WritePin(LCD16X2_RS_GPIO_Port, LCD16X2_RS_Pin, GPIO_PIN_RESET);
    LCD16X2_Write(cmd);
}

/**
 * @brief Write data to the LCD
 * @param data: Data to write
 */
static void LCD16X2_WriteData(uint8_t data)
{
    /* Set RS high for data */
    HAL_GPIO_WritePin(LCD16X2_RS_GPIO_Port, LCD16X2_RS_Pin, GPIO_PIN_SET);
    LCD16X2_Write(data);
}

/**
 * @brief Generate enable pulse
 */
static void LCD16X2_Pulse_E(void)
{
    HAL_GPIO_WritePin(LCD16X2_EN_GPIO_Port, LCD16X2_EN_Pin, GPIO_PIN_RESET);
    LCD16X2_Delay_us(1);
    HAL_GPIO_WritePin(LCD16X2_EN_GPIO_Port, LCD16X2_EN_Pin, GPIO_PIN_SET);
    LCD16X2_Delay_us(1);
    HAL_GPIO_WritePin(LCD16X2_EN_GPIO_Port, LCD16X2_EN_Pin, GPIO_PIN_RESET);
    LCD16X2_Delay_us(100);  /* Command execution time */
}

/**
 * @brief Write 4 bits to the LCD
 * @param nibble: 4 bits to write
 */
static void LCD16X2_Write4Bits(uint8_t nibble)
{
    /* Set data pins */
    HAL_GPIO_WritePin(LCD16X2_D4_GPIO_Port, LCD16X2_D4_Pin, (nibble & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD16X2_D5_GPIO_Port, LCD16X2_D5_Pin, (nibble & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD16X2_D6_GPIO_Port, LCD16X2_D6_Pin, (nibble & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD16X2_D7_GPIO_Port, LCD16X2_D7_Pin, (nibble & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    /* Pulse enable */
    LCD16X2_Pulse_E();
}

/**
 * @brief Microsecond delay
 * @param us: Delay in microseconds
 */
static void LCD16X2_Delay_us(uint32_t us)
{
    /* Simple delay loop - adjust according to your system clock */
    uint32_t i;
    for(i = 0; i < (us * 8); i++)
    {
        __NOP();
    }
}
