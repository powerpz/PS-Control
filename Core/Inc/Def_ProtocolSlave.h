/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PROTOCOL_SLAVE_H
#define	PROTOCOL_SLAVE_H

#include "GeneralDefines.h"
#include "stm32f4xx_hal.h"

#define PROTSLV_VERSION        0x01

#define MAX_INTERNAL_MEMORY			4
#define MAX_EXTERNAL_MEMORY			4

#define PROTSLV_POS_MEMSTATUS					25

enum
{
    PROTSLV_OP_READING = 0,
    PROTSLV_OP_WRITING = 1
};

typedef enum
{
    PROTSLV_STAT_ESPERANT_ADDR,
    PROTSLV_STAT_ESPERANT_HEADER,
    PROTSLV_STAT_VERIFICACIO_HEADER,
    PROTSLV_STAT_VERIFICACIO_ORDRE,
    PROTSLV_STAT_NOVA_ORDRE,
    PROTSLV_STAT_ESPERANT_DADES1,
    PROTSLV_STAT_ESPERANT_DADES2,
    PROTSLV_STAT_ESPERANT_CHECKSUM,
    PROTSLV_STAT_VERIFICACIO_CHECKSUM,
    PROTSLV_STAT_INICIAR_ORDRE,
    PROTSLV_STAT_PREPARE_ANSWER,
    PROTSLV_STAT_ENVIAR_HEADER,
    PROTSLV_STAT_ENVIAR_DADES,
    PROTSLV_STAT_ENVIAR_CHECKSUM,
    PROTSLV_STAT_ORDRE_KO,
    PROTSLV_STAT_ESPERANT_DADES_ORDRE_KO1,
    PROTSLV_STAT_ESPERANT_DADES_ORDRE_KO2,
    PROTSLV_STAT_ESPERANT_CHECKSUM_ORDRE_KO,
    PROTSLV_STAT_VERIFICACIO_CHECKSUM_ORDRE_KO,
            
    PROTSLV_STAT_SINCRONISME1,
    PROTSLV_STAT_SINCRONISME2
}PROTSLV_estats_t;
    
typedef union
{
    uint8_t u8Value;
    struct
    {
        uint8_t u8MemNoDefinida             :1;
        uint8_t u8NovaOrdreAmbOrdreActiva   :1;
        uint8_t u8OrdreResetAmbMemRepos     :1;
        uint8_t u8Unused                    :5;
    }bit_field;
}PROTSLV_error_t;

typedef union
{
    uint8_t value;
    struct
    {
        uint8_t u8RFU           : 3;
        uint8_t u8Direction     : 1;    /* 0, direcci�n master a esclavo; 1, esclavo a master */
        uint8_t u8acces_mem     : 3;
        uint8_t u8operation     : 1;
    }bit_field;
}protslv_tipo_op_field_t;

typedef union
{
    uint32_t addres;
    uint8_t ptu8Bytes[4];
}mem_addr_t;

typedef union
{
    uint16_t status;
    struct
    {
        uint8_t UsedInternalRAM    : 2;
        uint8_t InternalRAM        : 2;
        uint8_t InternalFLASH      : 2;
        uint8_t InternalE2PROM     : 2;
        uint8_t ExternalMem1       : 2;
        uint8_t ExternalMem2       : 2;
        uint8_t ExternalMem3       : 2;
        uint8_t ExternalMem4       : 2;
    }mem;
}PROTSLV_mems_status_t;

typedef enum
{
    PROTSLV_ORDER_IDLE,
    PROTSLV_ORDER_ORDER,
    PROTSLV_ORDER_POOLING,
    PROTSLV_ORDER_RESET_ORDER
}PROTSLV_order_t;

typedef enum
{
    PROTSLV_MEM_STAT_IDLE,
    PROTSLV_MEM_STAT_RUNNING,
    PROTSLV_MEM_STAT_FINISHED,
    PROTSLV_MEM_STAT_ABORTED
}PROTSLV_mem_status_t;

typedef struct
{
    uint8_t u8AddrSlave;
    protslv_tipo_op_field_t tipooperacion;
    uint8_t u8ProtocolVers;
    uint8_t u8NumData;
    mem_addr_t memaddr;
    PROTSLV_mems_status_t memoria;
    uint8_t u8ChecksumSlaveAddr;
    uint8_t u8ChecksumMemAddr;
}protslv_header_t;

typedef union
{
    struct
    {
        protslv_header_t header;
        uint8_t *ptu8Data;
        uint16_t u16FrameChecksum;
    }field;
    uint8_t ptucBytes[12+4+2];
}protslv_frame_t;

extern protslv_frame_t frame;

typedef struct
{
    uint8_t u8Enable;
    // Variables d'estat
    PROTSLV_estats_t u8ProtocolMotor;
    uint8_t u8BytesRebuts;
    uint8_t u8ActionMotor;
    uint8_t u8TamanyOrdreAcceptada;
    uint32_t u32AddrOrdreAcceptada;
    // Variable pel timer
    uint32_t u32TicksProtocol;
    
    
    returnstatus_t (*Receive)(uint8_t u8Channel, uint8_t* ptu8Dades, uint16_t u16NumDades);
    uint32_t (*QuantsBytesBufferRX)(uint8_t u8Channel);
    void (*ResetBuffers)(uint8_t u8Channel);
    returnstatus_t (*Transmit)(uint8_t u8Channel, uint8_t* ptu8Dades, uint16_t u16NumDades, uint8_t u8LastPacket);
    returnstatus_t (*Action[CMP_MAX_MEMORIES])(uint8_t u8Operation, uint8_t u8Order, uint32_t u32Addr, uint8_t* ptu8Dades, uint16_t u16NumDades);
}protslv_status_t;

void PROTSLV_Init(void);

returnstatus_t PROTSLV_Motor(void);

#endif	/* PROTOCOL_SLAVE_H */

