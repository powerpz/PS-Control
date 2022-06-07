/*
 * File:   LED.c
 * Author: ingenieria9
 *
 * Created on 27 de febrero de 2018, 15:22
 */



#include "LED.h"
#include "ticks.h"

led_status_t estatled[MAX_NUM_LED];

void LED_CanviLED(uint8_t u8Channel, uint8_t u8Estat);

void LED_Init(void) {
    uint8_t i;
    //   LEDS_OUTPUT_DIRECTION;

    //    LED_Y_OFF;
    //    LED_R_OFF;
    //    LED_G_OFF;
    LED1_OFF;
    LED2_OFF;
    //Actualizamos mapa de memoria
    //MemoryMap.var.cabecera_fw.board_status.bit_field.ledState = prtDL1;
    for (i = 0; i < MAX_NUM_LED; i++) {
        estatled[i].u8Motor = 0;
        estatled[i].u8Repeticions = 0;
        estatled[i].u8Fixe = 0;
        estatled[i].u8NovaConfig = 0;
        estatled[i].u16TempsOFF = 0;
        estatled[i].u16TempsON = 0;
    }
}

void LED_Motor(uint8_t u8Channel) {
    switch (estatled[u8Channel].u8Motor) {
        case 0:
            //If it is a new fixed led configuration, we change it
            if (estatled[u8Channel].u8Fixe == 1 && estatled[u8Channel].u8NovaConfig == 1) {
                if (estatled[u8Channel].u16TempsON > 0 && estatled[u8Channel].u16TempsOFF == 0) {
                    LED_CanviLED(u8Channel, LED_ON);
                } else if (estatled[u8Channel].u16TempsOFF > 0 && estatled[u8Channel].u16TempsON == 0) {
                    LED_CanviLED(u8Channel, LED_OFF);
                }
                estatled[u8Channel].u8NovaConfig = 0;

                //If it is a repetition configuration, we start by turning on the led
            } else if (estatled[u8Channel].u8Fixe == 0 && estatled[u8Channel].u8Repeticions > 0) {
                estatled[u8Channel].u32Timer = TICKS_GetTicks();
                LED_CanviLED(u8Channel, LED_ON);
                //Actualizamos mapa de memoria
                //MemoryMap.var.cabecera_fw.board_status.bit_field.ledState = prtDL1;
                estatled[u8Channel].u8Motor++;
            }
            break;
        case 1:
            if (TICKS_DiffTicks(estatled[u8Channel].u32Timer) > estatled[u8Channel].u16TempsON) {
                estatled[u8Channel].u32Timer = TICKS_GetTicks();
                LED_CanviLED(u8Channel, LED_OFF);
                //Actualizamos mapa de memoria
                //MemoryMap.var.cabecera_fw.board_status.bit_field.ledState = prtDL1;
                estatled[u8Channel].u8Motor++;
            }
            break;
        case 2:
            if (TICKS_DiffTicks(estatled[u8Channel].u32Timer) > estatled[u8Channel].u16TempsOFF) {
                //If we put 0xFF repetitions, it will repeat forever
                if (estatled[u8Channel].u8Repeticions != 0xFF) {
                    estatled[u8Channel].u8Repeticions--;
                }
                estatled[u8Channel].u8Motor = 0;
            }
            break;
        default:
            estatled[u8Channel].u8Motor = 0;
            break;
    }
}

void LED_Configure(uint8_t u8Channel, uint8_t u8Repeats, uint16_t u16TimeOff, uint16_t u16TimeOn, uint8_t u8Fixe) {
    estatled[u8Channel].u8Repeticions = u8Repeats;
    estatled[u8Channel].u8Fixe = u8Fixe;
    estatled[u8Channel].u16TempsOFF = u16TimeOff;
    estatled[u8Channel].u16TempsON = u16TimeOn;
    estatled[u8Channel].u8NovaConfig = 1;
}

void LED_CanviLED(uint8_t u8Channel, uint8_t u8Estat) {
    if (u8Channel == LED1) {
        prtDL1(u8Estat);
    } else if (u8Channel == LED2) {
        prtDL1(u8Estat);
    }
    // else { LED_G_OUT = u8Estat; }
}




