/*
 * configstm.h
 *
 *  Created on: Apr 27, 2022
 *      Author: 77FA9F4-Desktop
 */

#ifndef INC_CONFIGSTM_H_
#define INC_CONFIGSTM_H_

/**********************************************************************/
/****************	Port scheduling  		***************/
/**********************************************************************/

/* 2 - Input - Digital - Temp Fail 1 */
#define prtInput1_TempFail  0
/* 5 - Input - Digital - Temp Fail 2 */
#define prtInput2_TempFail  0
/* 0 - Input - Digital - DC Fail 1 */
#define prtInput1_DC_fail   0 //NA
/* 3 - Input - Digital - DC Fail 2 */
#define prtInput2_DC_fail   0 //NA
/* 1 - Input - Digital - AC Fail 1 (INT1)*/
#define prtInput1_AC_fail   0
/* 0 - Input - Digital - AC Fail 2 (INT0)*/
#define prtInput2_AC_fail   0
/* 1 - Input - Digital - Fan OK 1 */
#define prtInput1_FanOK     1
/* 4 - Input - Digital - Fan OK 2 */
#define prtInput2_FanOK     1
/* 5 - Input - Digital - Modulos Off 1 */
#define prtOutput1_ModulosOff(m)   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, m)
/* 4 - Input - Digital - Modulos Off 2 */
#define prtOutput2_ModulosOff(m)   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, m)
/* 2 - Input - Digital - Alarma In (INT2)*/
#define prtAlarmaIn             0

/* 5 - Input - Digital - SEL1 (Futuros usos)*/
#define prtSEL1         HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6)
/* 5 - Input - Digital - SEL2 (Futuros usos)*/
#define prtSEL2         HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5)

/* 0 - Input - Digital - OnOff stand-by*/
#define prtOnOffn       HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14)


/* 0 - Output - Digital - Alarma out*/
#define prtAlarmaOut(m)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, m)


/* PUERTO C */
/* 7 - Output - Digital - LED Green*/
#define prtDL2(m)	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, m)
/* 6 - Output - Digital - LED Yellow*/
#define prtDL1(m)	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, m)
/* 5 - Output - Digital - CS Aplicador 2 Peltier 2 */
//#define prtApl2Plt2n    // NA
/* 2 - Output - Digital - CS Aplicador 2 Peltier 1 */
//#define prtApl2Plt1n    // NA
/* 1 - Output - Digital - CS Aplicador 1 Peltier 2 */
//#define prtApl1Plt2n    // NA
/* 2 - Output - Digital - CS Aplicador 1 Peltier 1 */
//#define prtApl1Plt1n    // NA



/* 3 - Input - Digital - Artesin 2 */
#define prtArtesin2     PORTDbits.RD3
/* 2 - Input - Digital - Excelsys 2 */
#define prtExcelsys2    PORTDbits.RD2
/* 1 - Input - Digital - Artesin 1 */
#define prtArtesin1     PORTDbits.RD1
/* 0 - Input - Digital - Excelsys 1 */
#define prtExcelsys1    PORTDbits.RD0
/* 7 - Input - Digital - Power Good Peltier 2 */
//#define prtPG_Plt2n     PORTEbits.RE7
/* 6 - Input - Digital - Power Good Peltier 1 */
//#define prtPG_Plt1n     PORTEbits.RE6

/* 7 - Input - Digital - Power Good Peltier 2 */
#define prtPG_Plt1n     0
/* 6 - Input - Digital - Power Good Peltier 1 */
#define prtPG_Plt2n     0
/* 7 - Input - Digital - Power Good Peltier 2 */
#define prtPG_Plt3n     0
/* 6 - Input - Digital - Power Good Peltier 1 */
#define prtPG_Plt4n     0
/* 7 - Input - Digital - Power Good Peltier 2 */
#define prtPG_Plt5n     0
/* 6 - Input - Digital - Power Good Peltier 1 */
#define prtPG_Plt6n     0
/* 7 - Input - Digital - Power Good Peltier 2 */
#define prtPG_Plt7n     0
/* 6 - Input - Digital - Power Good Peltier 1 */
#define prtPG_Plt8n     0

/* 4 - Output - Digital - Green stand-by*/
#define prtGreenLED(m)		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, m)
/* 2 - Output - Digital - Red stand-by*/
#define prtRedLED(m)		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, m)


/* 5 - Salida - Digital - Enable FA digital */
#define prtEN_FA_DigitalN(m)  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, m)
/* 4 - Salida - Digital - Enable FA motor */
#define prtEN_FA_MotorN(m)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, m)
/* 3 - Salida - Digital - Enable Aplicador 2 */
//#define prtEN_Apl2n        LATFbits.LATF3
/* 2 - Salida - Digital - Enable Aplicador 1 */
//#define prtEN_Apl1n        LATFbits.LATF2


/*  */
#define prtEN_PS_Global(m)        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, m)

#define prtEN_Apl1n(m)        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, m)
/* 2 - Output - Digital - Enable Aplicador 1 */
#define prtEN_Apl2n(m)        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, m)
/* 3 - Output - Digital - Enable Aplicador 2 */
#define prtEN_Apl3n(m)        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, m)
/* 2 - Output - Digital - Enable Aplicador 1 */
#define prtEN_Apl4n(m)        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, m)
/* 3 - Output - Digital - Enable Aplicador 2 */
#define prtEN_Apl5n(m)        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, m)
/* 2 - Output - Digital - Enable Aplicador 1 */
#define prtEN_Apl6n(m)        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, m)
/* 3 - Output - Digital - Enable Aplicador 2 */
#define prtEN_Apl7n(m)        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, m)
/* 2 - Output - Digital - Enable Aplicador 1 */
#define prtEN_Apl8n(m)        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, m)


/* 1 - Input - Digital - Power good digital*/
#define prtPG_DigitalN     0

/*Interrupcion INT0 e INT1 para el AC_FAIL*/
//Fuente 1: (INT1)
//#define flgHabInt1_AC_fail      INTCON3bits.INT1E
//#define flgInt1_AC_fail         INTCON3bits.INT1F
//Prioridad definida abajo, en el INTCON3: Alta prioridad
//Tipo de flanco: INTCON2bits.INTEDG1 = 1; //Configurado ya en configuracion interrupciones (abajo): Rising edge

//Fuente 1: (INT0)
//#define flgHabInt1_Temp_fail      INTCONbits.INT0IE
//#define flgInt1_Temp_fail         INTCONbits.INT0IF
//INT0 siempre es de alta prioridad
//Tipo de flanco: INTCON2bits.INTEDG0 = 1; //Configurado ya en configuracion interrupciones (abajo): Rising edge

//Alarma (INT2)
//#define flgHabInt_Alarma      INTCON3bits.INT2IE
//#define flgInt_Alarma         INTCON3bits.INT2IF





#endif /* INC_CONFIGSTM_H_ */
