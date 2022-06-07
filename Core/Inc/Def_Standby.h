/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef STANDBY_H
#define	STANDBY_H

#include "GeneralDefines.h"
#include "ticks.h"

#include "LED.h" //debug
#include "Def_Control.h" //debug

typedef enum {
    PCB_STATE_OFF = 0,
    PCB_STATE_STANDBY = 1,
    PCB_STATE_WAIT_FOR_ON = 2,
    PCB_STATE_ON = 3,
    PCB_STATE_WAIT_FOR_STANDBY = 4,
}pcbState;


typedef struct{
    uint8_t digital;
    uint8_t motor;
    uint8_t apl1;
    uint8_t apl2;
}fuentes;


typedef struct {
 //   uint8_t u8hab1;
 //   uint8_t u8hab2;
    uint8_t u8Motor;
    uint8_t u8swcAnt;
    uint8_t u8standbyStatus;
    uint32_t u32Time;
    pcbState state;
    fuentes fuentesHab;
    fuentes fuentesOn;
} standby_t;

extern standby_t standby;


void standbyInit(void);
void standbyMotor(void);


void pruebasInit(void);
void pruebasMotor(void);


#endif	/* STANDBY_H */

