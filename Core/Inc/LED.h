#ifndef LED_H
#define	LED_H


#include "GeneralDefines.h"
#include "stm32f4xx_hal.h"
#include "configstm.h"

enum
{
//    LED_GROC = 0,
//    LED_VERMELL = 1,
//    LED_VERD = 2,
    LED1 = 0,
    LED2 = 1,
    MAX_NUM_LED
};

//#define LEDS_OUTPUT_DIRECTION TRISAbits.TRISA0 = 0; TRISAbits.TRISA1 = 0; TRISAbits.TRISA2 = 0;

#define LED_ON 0
#define LED_OFF 1

//#define LED_Y_OUT LATAbits.LATA0
//#define LED_R_OUT LATAbits.LATA1
//#define LED_G_OUT LATAbits.LATA2

//#define LED_Y_ON LATAbits.LATA0 = LED_ON;
//#define LED_R_ON LATAbits.LATA1 = LED_ON;
//#define LED_G_ON LATAbits.LATA2 = LED_ON;
#define LED1_ON 	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
#define LED2_ON 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

//#define LED_Y_OFF LATAbits.LATA0 = LED_OFF;
//#define LED_R_OFF LATAbits.LATA1 = LED_OFF;
//#define LED_G_OFF LATAbits.LATA2 = LED_OFF;
#define LED1_OFF 	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
#define LED2_OFF 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);

//#define LED_Y_CPL LATAbits.LATA0 ^= 1;
//#define LED_R_CPL LATAbits.LATA1 ^= 1;
//#define LED_G_CPL LATAbits.LATA2 ^= 1;

typedef struct
{
    uint8_t u8Motor;
    uint8_t u8Repeticions;
    uint8_t u8Fixe;
    uint8_t u8NovaConfig;
    uint16_t u16TempsON;
    uint16_t u16TempsOFF;
    uint32_t u32Timer;
}led_status_t;

void LED_Init(void);

void LED_Motor(uint8_t u8Channel);

//Si ponemos 0xFF repeticiones, se repetira para siempre
void LED_Configure(uint8_t u8Channel, uint8_t u8Repeats, uint16_t u16TimeOff, uint16_t u16TimeOn, uint8_t u8Fixe);

#endif	/* LED_H */


