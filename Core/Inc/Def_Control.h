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
#ifndef XC_CONTROL_H
#define	XC_CONTROL_H

#include "main.h"
#include "GeneralDefines.h"
#include "ticks.h"
#include "configstm.h"
#include "LED.h" //debug


#define MAX_NUM_SPI_CH 4
#define FNT_NUM_TEMP_FAIL 5
#define FNT_NUM_FAN_FAIL 2

//typedef enum {
//    NONE = 0,
//    EXCELSYS = 1,
//    ARTESIN = 2,
//    ERROR = 3,
//} IdFuente_t;

typedef enum {
    NONE = 0,
    UX6 = 1,
    UX4 = 2,
    DEF_ERROR = 3,
} IdFuente_t;

typedef enum {
    NOT_DEFINED = 0,
    PCB1 = 1,
    PCB2 = 2,
} IdPCB_t;

typedef struct {
    IdPCB_t IdPCB;
    IdFuente_t IdFuente1;
    IdFuente_t IdFuente2;
} identif_t;

typedef struct {
    uint8_t alarmsOn;
    uint8_t u8MotorFan;
    uint8_t atempFail1[FNT_NUM_TEMP_FAIL];
    uint8_t atempFail2[FNT_NUM_TEMP_FAIL];
     uint8_t afanFail1[FNT_NUM_FAN_FAIL];
    uint8_t afanFail2[FNT_NUM_FAN_FAIL];
  //  uint8_t tempFail1;
  //  uint8_t tempFail2;
 //   uint8_t fanFail1;
 //   uint8_t fanFail2;
 //   uint8_t PCB1Fuente1ACFail;
 //   uint8_t PCB1Fuente2ACFail;
 //   uint8_t PCB2Fuente1ACFail;
    uint32_t u32fanTimer;
} fuentesStatus_t;


extern identif_t identif;
extern fuentesStatus_t fuentesStatus;

void CTR_identif(void);
void CTR_fuentes_ON_OFF_init(void);
void CTR_fuentes_ON_OFF_motor(void);
void appSPI_init(void);
void appSPI_motor(void);
void alarmsControl_init(void);
void alarmsControl_motor(void);


#endif	/* XC_CONTROL_H */

