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
#ifndef GENERALDEFINES_H
#define	GENERALDEFINES_H



/*
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "main.h"
 */
#include "stm32f4xx_hal.h"

/* TIPOS */
//typedef unsigned char uint8_t;
//typedef unsigned short uint16;
//typedef unsigned short long uint24;
//typedef unsigned long uint32;
//
//typedef signed char int8;
//typedef signed short int16;
//typedef signed short long int24;
//typedef signed long int32;
//
//
//typedef unsigned char uint8_t;
//typedef unsigned short uint16_t;
//typedef unsigned short long uint24_t;
//typedef unsigned long uint32_t;
//
//typedef signed char int8_t;
//typedef signed short int16_t;
//typedef signed short long int24_t;
//typedef signed long int32_t;

typedef enum {
    RUNNING_HTP = -2,
    ERROR_HTP = -1,
    IDLE_HTP = 0,
    SUCCESS_HTP = 1,
    BLOCKING_HTP = 3,
} returnstatus_t;

#define SOLO_2_FUENTES      1

//#define MAX_PCB_FUENTES             2
//#define MAX_PCB_COMPLEMENTOS		6//11


//#define EXP_DEFAULT_ADDR            0x04
//#define CMP_DEFAULT_ADDR            4
#define FNT_DEFAULT_ADDR            0
#define FNT_BOARD_TYPE              0x02


#define EXP_MIN_SLOT                2
#define EXP_MAX_SLOT				2

#define FNT_FW_VERSION              21

#define CMP_MAX_BUF_RX              0xFF
#define CMP_INTERNAL_USED_RAM_SIZE  sizeof(memory_map_t)
#define CMP_INTERNAL_RAM_SIZE       1536
#define CMP_INTERNAL_FLASH_SIZE     32768
#define CMP_INTERNAL_EEPROM_SIZE    256
#define EXP_UNDEFINED_4_SIZE   		0
#define EXP_UNDEFINED_5_SIZE      	0
#define EXP_UNDEFINED_6_SIZE        0
#define EXP_UNDEFINED_7_SIZE        0

//#define EXP_MAX_BUFFER_DATA_MANIPULO    256
//#define EXP_MAX_REMAP_DATA_IN           8
//#define EXP_MAX_REMAP_DATA_OUT			8

//#define EEPROM_ID_DEFAULT_NUM   0
#define EEPROM_ID_NUM             5 //Pruebas para tener una ID fija desde es inicio, quitar al final


#define NUM_VALORES_NTC 4
#define NUM_VALORES_AIRE 4
#define NUM_VALORES_HIDR 4
#define NUM_VALORES_CAUDAL 4

#define FNT_NUM_MODULES 4
#define FNT_NUM_FNTS_ADJUST 4
//#define FNT_NUM_POWER_GOOD 4

//enum {
//    EXP_PERIFERICO_MAIN, /* no se enviaran tramas SPI, pero sirve para reservar una zona para la main */
//    EXP_PERIFERICO_EXPANSORA, /* no se enviaran tramas SPI, pero sirve para reservar una zona para la expansora */
//    EXP_PERIFERICO_RF1,
//    EXP_PERIFERICO_RF2,
//    EXP_PERIFERICO_MAN1,
//    EXP_PERIFERICO_MAN2,
//    EXP_PERIFERICO_MAN3,
//    EXP_PERIFERICO_MAN4,
//
//    EXP_MAX_PERIFERICOS
//};

enum {
    CMP_INTERNAL_USED_RAM = 0,
    CMP_INTERNAL_RAM,
    CMP_INTERNAL_FLASH,
    CMP_INTERNAL_EEPROM,
    CMP_UNDEFINED_4,
    CMP_UNDEFINED_5,
    CMP_UNDEFINED_6,
    CMP_UNDEFINED_7,

    CMP_MAX_MEMORIES
};

//enum {
//    EXP_ACCION_IDLE,
//    EXP_ACCION_RESET,
//    EXP_ACCION_GUARDAR_CONFIG,
//    EXP_ACCION_DESCUBRIMIENTO,
//
//    EXP_MAX_ACCIONES
//};

typedef enum {
    MAIN_STATE_OFF = 0,
    MAIN_STATE_STANDBY = 1,
    MAIN_STATE_READY_FOR_ON = 2,
    MAIN_STATE_ON = 3,
    MAIN_STATE_READY_FOR_STANDBY = 4,
} mainState;

//4typedef enum {
//    FIXED1 = 0b00000001,
//    FIXED2 = 0b00000010,
//    ADJUSTABLE1 = 0b00000100,
//    ADJUSTABLE2 = 0b00001000,
//} modulos_ON_OFF;

typedef enum {
    FNT_MOD_FIXED1 = 0,
    FNT_MOD_FIXED2 = 1,
    FNT_MOD_ADJUSTABLE1 = 2,
    FNT_MOD_ADJUSTABLE2 = 3,
} modulos_ON_OFF;

typedef enum {
    FNT_MAIN_ALARM_NONE = 0,
    FNT_MAIN_ALARM_OFF = 1,
    FNT_MAIN_ALARM_ON = 2,
} alarmasFuente;

typedef union {
    uint16_t u16value;

    struct {
        uint8_t u8LO;
        uint8_t u8HI;
    } byteacces;

    struct {
        uint8_t bit0 : 1;
        uint8_t bit1 : 1;
        uint8_t bit2 : 1;
        uint8_t bit3 : 1;
        uint8_t bit4 : 1;
        uint8_t bit5 : 1;
        uint8_t bit6 : 1;
        uint8_t bit7 : 1;
        uint8_t bit8 : 1;
        uint8_t bit9 : 1;
        uint8_t bit10 : 1;
        uint8_t bit11 : 1;
        uint8_t bit12 : 1;
        uint8_t bit13 : 1;
        uint8_t bit14 : 1;
        uint8_t bit15 : 1;
    } bitacces;
} doublereg_t;

typedef union {
    uint32_t u32value;
    float fvalue;

    struct {
        doublereg_t u16LO;
        doublereg_t u16HI;
    } intacces;
} tetrareg_t;

typedef struct {
    uint8_t u8BoardAddres;
    uint8_t u8BoardType;
    uint8_t u8MaxBuffRx;
    uint8_t u8BootVers;
    uint8_t u8FWVers;
    uint8_t u8ProtVers;
    uint8_t u8MinSlot;
    uint8_t u8MaxSlot;
    tetrareg_t SizeMemories[CMP_MAX_MEMORIES];
} boarddef_t;

typedef union {
    uint8_t status;

    struct {
        //        uint8_t coms_error : 1;
        //        uint8_t ledState : 1;
        //        uint8_t EVstate : 1;
        //        uint8_t switchState : 1;
        //        uint8_t pressAireReach: 1;

        uint8_t pcbState : 3; //Standby.h pcbState
        uint8_t switchState : 1;
        uint8_t dummy : 4;
    } bit_field;
} board_status_t;

typedef union {
    uint8_t alarms;

    struct {
        uint8_t RS584_Error     : 1;
        uint8_t fuente1ACFail   : 1;
        uint8_t fuente2ACFail   : 1;
        uint8_t fuente1TempFail : 1;
        uint8_t fuente2TempFail : 1;
        uint8_t fuente1FanFail  : 1;
        uint8_t fuente2FanFail  : 1;
        uint8_t AlarmaExterna   : 1;
    } bit_field;
} board_alarms_t;

typedef union {
    uint8_t status;

    struct {
		uint8_t stat: 2; /* check for pending tasks per protocol */
        uint8_t abort: 1; /* from protocol you want to abort operation (you can only abort operations done by itself) */
        uint8_t op_protocol: 1; /* read / write function by protocol */
        uint8_t func_prot: 1; /* module occupied by protocol command */
        uint8_t write: 1; /* writing module */
        uint8_t read: 1; /* reading module */
        uint8_t scan: 1; /* card scanning module */
    } bit_field;
} mems_status_t;

typedef union {
    uint8_t alarms;

    struct {
        uint8_t u8AccesForaRang : 1;
        uint8_t u7code : 7;
    } bit_field;
} mems_alarms_t;

typedef struct {
    boarddef_t board_definition;
    board_status_t board_status;
    board_alarms_t board_alarms;
    uint8_t dummy[2];
    uint16_t u16accions;
    uint16_t u16accioactual;
    mems_status_t mems_status[CMP_MAX_MEMORIES];
    mems_alarms_t mems_alarms[CMP_MAX_MEMORIES];
} cabecera_fw_t;

//typedef struct {
//    cabecera_fw_t cabecera_fw;
//    /* anyadir variables propias de la placa */
//    uint32_t cmsd;
//
//} mapa_memoria_comp_t;
//
//typedef struct {
//    cabecera_fw_t cabecera_fw;
//    /* anyadir variables propias de la placa */
//    uint32_t cmsd;
//
//} mapa_memoria_fuentes_t;
//
//typedef struct {
//    uint8_t id_data;
//    uint8_t num_bytes;
//    uint16_t addr_init;
//} struct_remap_t;

//typedef struct {
//    struct_remap_t data_in[EXP_MAX_REMAP_DATA_IN];
//    struct_remap_t data_out[EXP_MAX_REMAP_DATA_OUT];
//} perif_remap_t;

//typedef struct {
//    uint16_t max_buffer;
//    uint8_t version[EXP_MAX_PERIFERICOS];
//} version_data_t;

//typedef struct {
//    uint32_t perif_habilitados;
//    uint8_t ptu8Buffer_DataPerif[EXP_MAX_BUFFER_DATA_MANIPULO];
//    perif_remap_t remapeo_periferico[EXP_MAX_PERIFERICOS];
//    version_data_t version_data[EXP_MAX_PERIFERICOS];
//} gestion_manipulos_t;

typedef struct {
    uint32_t u32TramasOK;
    uint32_t u32TramasKO;
} contador_coms_t;

//typedef union {
//    uint16_t value;
//
//    struct {
//        uint8_t slot0 : 1;
//        uint8_t slot1 : 1;
//        uint8_t slot2 : 1;
//        uint8_t detect_M1 : 1;
//        uint8_t detect_M2 : 1;
//        uint8_t detect_M3 : 1;
//        uint8_t detect_M4 : 1;
//        uint8_t pedal_A : 1;
//        uint8_t pedal_C : 1;
//        uint8_t alarma_gral : 1;
//        uint8_t puerta_vacum : 1;
//        uint8_t puerta_iza : 1;
//        uint8_t puerta_dch : 1;
//        uint8_t puerta_elec : 1;
//        uint8_t puerta_tobera : 1;
//
//        uint8_t dummy : 1;
//    } bit_field;
//} estado_entradas_t;

//typedef struct {
//    //    uint8_t ledState;
//    //    uint8_t EVstate;
//    //    uint8_t switchState;
//    //    uint8_t dummy;
//    tetrareg_t NTCtemp; //dir: 64d[40h]
//    tetrareg_t pressAire;
//    tetrareg_t pressHidr;
//    tetrareg_t caudal;
//    tetrareg_t tiempoEV_ON; //dir: 80d[50h]
//    
//    tetrareg_t pressAireMax;
//
//    tetrareg_t aNTCtemp[NUM_VALORES_NTC];
//    tetrareg_t aPressAire[NUM_VALORES_AIRE];
//    tetrareg_t aPressHidr[NUM_VALORES_HIDR];
//    tetrareg_t aCaudal[NUM_VALORES_CAUDAL];
//
//    tetrareg_t debug1;
//    tetrareg_t debug2;
//    tetrareg_t debug3;
//
//} complementos_t;

typedef struct {
    uint8_t mainStatus; //GeneralDefines.h mainState
    uint8_t estadoModulos;
    uint8_t alarma; //GeneralDefines.h alarmaMain
    uint8_t powerGoods; //[FNT_NUM_POWER_GOOD];
   // uint8_t modulos_ON_OFF; //GeneralDefines.h modulos_ON_OFF
    uint8_t aModulos_ON_OFF[FNT_NUM_MODULES];
    // uint8_t aPowerGood[FNT_NUM_POWER_GOOD];
    tetrareg_t aVoltagePercent[FNT_NUM_FNTS_ADJUST];
    uint8_t colorLED;

} fuentes_t;

typedef struct {
    cabecera_fw_t cabecera_fw;
    /* anyadir variables propias de la placa */
    //    estado_entradas_t estado_entradas;
    //    estado_entradas_t accion_entradas;

    // complementos_t complementos;
    fuentes_t fuentes;

    /* contadores de errores de protocolos */
    contador_coms_t puerto485_slv;

} memory_map_def_t;

typedef union {
    memory_map_def_t var;
    uint8_t ptucBytes[sizeof (memory_map_def_t)];
} memory_map_t;


extern memory_map_t MemoryMap;















#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // If C++ is being used, regular C code needs function names to have C
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* GENERALDEFINES_H */


