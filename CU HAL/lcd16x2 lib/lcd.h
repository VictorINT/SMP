/**
 * @file lcd.h
 * @brief Header file for 16x2 LCD driver using STM32 HAL
 */

#ifndef LCD_H
#define LCD_H

#include "main.h"

/* Pin definitions */
/* RS Pin */
#ifndef LCD16X2_RS_GPIO_Port
#define LCD16X2_RS_GPIO_Port GPIOB
#define LCD16X2_RS_Pin GPIO_PIN_5
#endif

/* RW Pin */
#ifndef LCD16X2_RW_GPIO_Port
#define LCD16X2_RW_GPIO_Port GPIOB
#define LCD16X2_RW_Pin GPIO_PIN_4
#endif

/* EN Pin */
#ifndef LCD16X2_EN_GPIO_Port
#define LCD16X2_EN_GPIO_Port GPIOB
#define LCD16X2_EN_Pin GPIO_PIN_10
#endif

/* Data pins (D4-D7 for 4-bit mode) */
#ifndef LCD16X2_D4_GPIO_Port
#define LCD16X2_D4_GPIO_Port GPIOA
#define LCD16X2_D4_Pin GPIO_PIN_9
#endif

#ifndef LCD16X2_D5_GPIO_Port
#define LCD16X2_D5_GPIO_Port GPIOA
#define LCD16X2_D5_Pin GPIO_PIN_7
#endif

#ifndef LCD16X2_D6_GPIO_Port
#define LCD16X2_D6_GPIO_Port GPIOA
#define LCD16X2_D6_Pin GPIO_PIN_6
#endif

#ifndef LCD16X2_D7_GPIO_Port
#define LCD16X2_D7_GPIO_Port GPIOA
#define LCD16X2_D7_Pin GPIO_PIN_5
#endif

/* Display commands */
#define LCD16X2_CLEAR_DISPLAY              0x01
#define LCD16X2_RETURN_HOME                0x02
#define LCD16X2_ENTRY_MODE_SET             0x04
#define LCD16X2_DISPLAY_CONTROL            0x08
#define LCD16X2_CURSOR_SHIFT               0x10
#define LCD16X2_FUNCTION_SET               0x20
#define LCD16X2_SET_CGRAM_ADDR             0x40
#define LCD16X2_SET_DDRAM_ADDR             0x80

/* Entry mode set commands */
#define LCD16X2_ENTRY_RIGHT                0x00
#define LCD16X2_ENTRY_LEFT                 0x02
#define LCD16X2_ENTRY_SHIFT_INC            0x01
#define LCD16X2_ENTRY_SHIFT_DEC            0x00

/* Display control commands */
#define LCD16X2_DISPLAY_ON                 0x04
#define LCD16X2_DISPLAY_OFF                0x00
#define LCD16X2_CURSOR_ON                  0x02
#define LCD16X2_CURSOR_OFF                 0x00
#define LCD16X2_BLINK_ON                   0x01
#define LCD16X2_BLINK_OFF                  0x00

/* Display/cursor shift commands */
#define LCD16X2_DISPLAY_MOVE               0x08
#define LCD16X2_CURSOR_MOVE                0x00
#define LCD16X2_MOVE_RIGHT                 0x04
#define LCD16X2_MOVE_LEFT                  0x00

/* Function set commands */
#define LCD16X2_8BIT_MODE                  0x10
#define LCD16X2_4BIT_MODE                  0x00
#define LCD16X2_2LINE                      0x08
#define LCD16X2_1LINE                      0x00
#define LCD16X2_5x10DOTS                   0x04
#define LCD16X2_5x8DOTS                    0x00

/* Display modes */
#define LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF      (LCD16X2_DISPLAY_ON | LCD16X2_CURSOR_OFF | LCD16X2_BLINK_OFF)
#define LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_ON       (LCD16X2_DISPLAY_ON | LCD16X2_CURSOR_OFF | LCD16X2_BLINK_ON)
#define LCD16X2_DISPLAY_ON_CURSOR_ON_BLINK_OFF       (LCD16X2_DISPLAY_ON | LCD16X2_CURSOR_ON | LCD16X2_BLINK_OFF)
#define LCD16X2_DISPLAY_ON_CURSOR_ON_BLINK_ON        (LCD16X2_DISPLAY_ON | LCD16X2_CURSOR_ON | LCD16X2_BLINK_ON)
#define LCD16X2_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF     (LCD16X2_DISPLAY_OFF | LCD16X2_CURSOR_OFF | LCD16X2_BLINK_OFF)

/* Function prototypes */
void LCD16X2_Init(uint8_t displayMode);
void LCD16X2_ClrScr(void);
void LCD16X2_Home(void);
void LCD16X2_GotoXY(uint8_t x, uint8_t y);
void LCD16X2_PutChar(char c);
void LCD16X2_Puts(const char *s);
void LCD16X2_CreateCustomChar(uint8_t location, uint8_t *charmap);
void LCD16X2_PutCustomChar(uint8_t x, uint8_t y, uint8_t location);
void LCD16X2_DisplayControl(uint8_t displayMode);

#endif /* LCD_H */
