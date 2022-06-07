/*
 * File:   ProtocolSlave.c
 * Author: ingenieria9
 *
 * Created on 23 de febrero de 2018, 10:04
 */

#include "Def_ProtocolSlave.h"
#include "ticks.h"
#include "Def_RS485.h"
#include "Def_System.h"
//#include "EEPROM_INTERNA.h"
//#include "../ATparser/ATparser.h"

#define PROTSLV_TIME_WAITING_HEADER    3      /* time waiting for the 9 bytes of the header */
#define PROTSLV_TIME_WAITING_DATA      3      /* time waiting between bytes, reset for each byte received */
#define PROTSLV_TIME_WAITING_CHECKSUM  3      /* time waiting for 2 checksum bytes */
#define PROTSLV_TIME_WAITING_SYNC      3       /* time waiting for synchronism in bad plot*/


protslv_status_t protslv;
protslv_frame_t frame;

uint8_t ptu8ProtBuffer[CMP_MAX_BUF_RX];
uint8_t u8ErrorNumber;
uint8_t u8OperacioEnCurs;

returnstatus_t ActionNULL(uint8_t u8Operation, uint8_t u8Order, uint32_t u32Addr, uint8_t* ptu8Dades, uint16_t u16NumDades);

returnstatus_t CheckHeader(protslv_header_t cab);

void PROTSLV_Init(void) {
    /* inicialitzacio punters a funcio */
    protslv.QuantsBytesBufferRX = RS485_QuantsBytesBufferRX;
    protslv.Receive = RS485_Receive;
    protslv.ResetBuffers = RS485_ResetBuffers;
    protslv.Transmit = RS485_Transmit;

    protslv.Action[CMP_INTERNAL_USED_RAM] = SYS_Action;
    protslv.Action[CMP_INTERNAL_RAM] = ActionNULL;
    protslv.Action[CMP_INTERNAL_FLASH] = ActionNULL;
    //protslv.Action[CMP_INTERNAL_EEPROM] = EEPROMINTERNA_Action;
    protslv.Action[CMP_INTERNAL_EEPROM] = ActionNULL;
    protslv.Action[CMP_UNDEFINED_4] = ActionNULL;
    protslv.Action[CMP_UNDEFINED_5] = ActionNULL;
    protslv.Action[CMP_UNDEFINED_6] = ActionNULL;
    protslv.Action[CMP_UNDEFINED_7] = ActionNULL;
    /* inicialitzacio variables */
    protslv.u8ActionMotor = 0;
    protslv.u8Enable = 1;
    protslv.u8ProtocolMotor = PROTSLV_STAT_ESPERANT_ADDR;
    protslv.ResetBuffers(RS485_SLV);
    /* inicialitzacio buffer protocol */
    frame.field.ptu8Data = ptu8ProtBuffer;

    u8ErrorNumber = 0;

    MemoryMap.var.puerto485_slv.u32TramasOK = 0;
    MemoryMap.var.puerto485_slv.u32TramasKO = 0;

    RS485_Init(RS485_SLV);

    //	BUZZ_Configure(1, 100, PATRO_SO_SLAVE_MODE);
    //	HAL_GPIO_WritePin(DBG_LED_1_GPIO_Port, DBG_LED_1_Pin, GPIO_PIN_RESET);
    //	HAL_GPIO_WritePin(DBG_LED_2_GPIO_Port, DBG_LED_2_Pin, GPIO_PIN_RESET);
    //	HAL_GPIO_WritePin(DBG_LED_3_GPIO_Port, DBG_LED_3_Pin, GPIO_PIN_RESET);
    //	HAL_GPIO_WritePin(DBG_LED_4_GPIO_Port, DBG_LED_4_Pin, GPIO_PIN_RESET);

}

returnstatus_t PROTSLV_Motor(void) {
    uint8_t i;
    uint16_t Aux;
    returnstatus_t retorn;

    retorn = RUNNING_HTP;
    switch (protslv.u8ProtocolMotor) {
        case PROTSLV_STAT_ESPERANT_ADDR:
            /* I take 1 byte to see if it's for me */
            if (protslv.Receive(RS485_SLV, (&frame.field.header.u8AddrSlave), 1) == SUCCESS_HTP) {
                {
               //   if ((frame.field.header.u8AddrSlave == MemoryMap.var.cabecera_fw.board_definition.u8BoardAddres) || (frame.field.header.u8AddrSlave == 0)) {
                    if ((frame.field.header.u8AddrSlave == 0) || (frame.field.header.u8AddrSlave == 1) || (frame.field.header.u8AddrSlave == 2)) {
                    	MemoryMap.var.cabecera_fw.board_definition.u8BoardAddres = frame.field.header.u8AddrSlave; // iii take the address from requested board
                        protslv.u32TicksProtocol = TICKS_GetTicks();
                        protslv.u8ProtocolMotor = PROTSLV_STAT_ESPERANT_HEADER;
                        break;
                    }
                }
            }
            break;
        case PROTSLV_STAT_ESPERANT_HEADER:
            /* it looks like it was for me, I'm going to take the rest of the header */
            if (protslv.Receive(RS485_SLV, (uint8_t *) ((uint8_t *) (frame.ptucBytes) + 1), 11) == SUCCESS_HTP) {
                protslv.u8ProtocolMotor = PROTSLV_STAT_VERIFICACIO_HEADER;
            } else if (TICKS_DiffTicks(protslv.u32TicksProtocol) > PROTSLV_TIME_WAITING_HEADER) {
                /* timeout of 0.25ms per byte approx */
                protslv.u32TicksProtocol = TICKS_GetTicks();
                protslv.u8ProtocolMotor = PROTSLV_STAT_SINCRONISME1;
                u8ErrorNumber = 1;
            }
            break;
        case PROTSLV_STAT_VERIFICACIO_HEADER:
            /* check if the header is correct, otherwise i will sync */
            if (CheckHeader(frame.field.header) == SUCCESS_HTP) {
                protslv.u8ProtocolMotor = PROTSLV_STAT_VERIFICACIO_ORDRE;
            } else {
                u8ErrorNumber = 2;
                protslv.u8ProtocolMotor = PROTSLV_STAT_SINCRONISME1;
            }
            break;
        case PROTSLV_STAT_VERIFICACIO_ORDRE:
			/* header is correct */
            /* before grabbing the data field, see if I can execute the action so as not to overwrite any data that may be using another */
            /* only in the case of reading or writing */
            Aux = ((uint16_t) frame.field.header.memoria.status)>>(2 * (uint16_t) frame.field.header.tipooperacion.bit_field.u8acces_mem);
            Aux &= 0x03;
            switch (Aux) {
                case PROTSLV_ORDER_IDLE:
                    /* if an IDLE command arrives, I treat it the same as pooling */
                case PROTSLV_ORDER_POOLING:
                    /* it is necessary to look for that it will not have data */
                    if (frame.field.header.u8NumData == 0) {
                        protslv.u8ProtocolMotor = PROTSLV_STAT_NOVA_ORDRE;
                    } else {
                        /* wrong plot, I'm going to sync */
                        protslv.u8ProtocolMotor = PROTSLV_STAT_SINCRONISME1;
                        u8ErrorNumber = 5;
                    }
                    break;
                case PROTSLV_ORDER_ORDER:
                    /* see if there is an order in progress */
                    if (
                            (MemoryMap.var.cabecera_fw.mems_status[CMP_INTERNAL_USED_RAM].bit_field.stat == 0) &&
                            (MemoryMap.var.cabecera_fw.mems_status[CMP_INTERNAL_RAM].bit_field.stat == 0) &&
                            (MemoryMap.var.cabecera_fw.mems_status[CMP_INTERNAL_FLASH].bit_field.stat == 0) &&
                            (MemoryMap.var.cabecera_fw.mems_status[CMP_INTERNAL_EEPROM].bit_field.stat == 0) &&
                            (MemoryMap.var.cabecera_fw.mems_status[CMP_UNDEFINED_4].bit_field.stat == 0) &&
                            (MemoryMap.var.cabecera_fw.mems_status[CMP_UNDEFINED_5].bit_field.stat == 0) &&
                            (MemoryMap.var.cabecera_fw.mems_status[CMP_UNDEFINED_6].bit_field.stat == 0) &&
                            (MemoryMap.var.cabecera_fw.mems_status[CMP_UNDEFINED_7].bit_field.stat == 0)
                            ) {
                        /* no action is being taken, I can continue */
                        protslv.u8ProtocolMotor = PROTSLV_STAT_NOVA_ORDRE;
                        protslv.u32AddrOrdreAcceptada = frame.field.header.memaddr.addres;
                        protslv.u8TamanyOrdreAcceptada = frame.field.header.u8NumData;
                    } else {
                        /* if some action is being taken, what can be done must be limited */
                        protslv.u8ProtocolMotor = PROTSLV_STAT_ORDRE_KO;
                        u8OperacioEnCurs = 1;
                    }
                    break;
                case PROTSLV_ORDER_RESET_ORDER:
                    /* I see if there is an order running and I answer, it can't have data */
                    if (frame.field.header.u8NumData == 0) {
                        /* see if the memory you want to abort has an initial order */

                        i = frame.field.header.tipooperacion.bit_field.u8acces_mem;
                        if (MemoryMap.var.cabecera_fw.mems_status[i].bit_field.stat != 0) {
                            /* order in progress, I will say cancel */
                            protslv.u8ProtocolMotor = PROTSLV_STAT_NOVA_ORDRE;
                        } else {
                            /* memory at rest */
                            protslv.u8ProtocolMotor = PROTSLV_STAT_ORDRE_KO;
                            u8OperacioEnCurs = 2;
                        }
                    } else {
                        /* wrong plot, I'm going to sync */
                        protslv.u8ProtocolMotor = PROTSLV_STAT_SINCRONISME1;
                        u8ErrorNumber = 6;
                    }
                    break;
            }
            break;
        case PROTSLV_STAT_NOVA_ORDRE:

/* new order without any */
            /* I will be receiving data, if there is any, ONLY if it is written */
            if (frame.field.header.tipooperacion.bit_field.u8operation == PROTSLV_OP_READING) {
                /* numb bytes es diferent de 0, pero es una lectura */
                protslv.u32TicksProtocol = TICKS_GetTicks();
                protslv.u8ProtocolMotor = PROTSLV_STAT_ESPERANT_CHECKSUM;
            } else {
                protslv.u8BytesRebuts = 0;
                protslv.u8ProtocolMotor++;
            }
            break;
        case PROTSLV_STAT_ESPERANT_DADES1:
            protslv.u32TicksProtocol = TICKS_GetTicks();
            if (protslv.u8BytesRebuts < frame.field.header.u8NumData) {
                /* data left to receive */
                protslv.u8ProtocolMotor = PROTSLV_STAT_ESPERANT_DADES2;
            } else {
                protslv.u8ProtocolMotor = PROTSLV_STAT_ESPERANT_CHECKSUM;
            }
            break;
        case PROTSLV_STAT_ESPERANT_DADES2:
            i = protslv.QuantsBytesBufferRX(RS485_SLV);
            if ((protslv.u8BytesRebuts + i) > frame.field.header.u8NumData) {
                /* there are more bytes than the data I have left, I only take the missing ones */
                /* maybe the plot checksum has already reached me, but it is taken in another state */
                i = (uint8_t) (frame.field.header.u8NumData - protslv.u8BytesRebuts);
            }
            if (i > 0) {
                protslv.Receive(RS485_SLV, frame.field.ptu8Data + protslv.u8BytesRebuts, i);
                protslv.u8BytesRebuts += i;
                protslv.u8ProtocolMotor = PROTSLV_STAT_ESPERANT_DADES1;
            } else if (TICKS_DiffTicks(protslv.u32TicksProtocol) > PROTSLV_TIME_WAITING_DATA) {
                protslv.u8ProtocolMotor = PROTSLV_STAT_SINCRONISME1;
                u8ErrorNumber = 7;
            }
            break;
        case PROTSLV_STAT_ESPERANT_CHECKSUM:
            if (protslv.Receive(RS485_SLV, (uint8_t *) (&frame.field.u16FrameChecksum), 2) == SUCCESS_HTP) {
                protslv.u8ProtocolMotor = PROTSLV_STAT_VERIFICACIO_CHECKSUM;
            } else if (TICKS_DiffTicks(protslv.u32TicksProtocol) > PROTSLV_TIME_WAITING_CHECKSUM) {
                protslv.u8ProtocolMotor = PROTSLV_STAT_SINCRONISME1;
                u8ErrorNumber = 8;
            }
            break;
        case PROTSLV_STAT_VERIFICACIO_CHECKSUM:
            u8OperacioEnCurs = 0;
            Aux = frame.field.u16FrameChecksum;
            for (i = 0; i < sizeof (protslv_header_t); i++) {
                Aux += frame.ptucBytes[i];
            }
            if (frame.field.header.tipooperacion.bit_field.u8operation == PROTSLV_OP_WRITING) {
                for (i = 0; i < frame.field.header.u8NumData; i++) {
                    Aux += frame.field.ptu8Data[i];
                }
            }
            if (Aux == 0) {
                /* correct package, i will start order */
                protslv.u8ProtocolMotor = PROTSLV_STAT_INICIAR_ORDRE;
            } else {
                protslv.u8ProtocolMotor = PROTSLV_STAT_SINCRONISME1;
                u8ErrorNumber = 9;
            }
            break;
        case PROTSLV_STAT_INICIAR_ORDRE:
            //					HAL_GPIO_TogglePin(DBG_LED_3_GPIO_Port, DBG_LED_3_Pin);
            //            frame.field.header.memoria.status = 0;
            i = (uint16_t) (frame.field.header.memoria.status)>>(2 * (uint16_t) frame.field.header.tipooperacion.bit_field.u8acces_mem);
            i &= 0x03;
            switch (protslv.Action[frame.field.header.tipooperacion.bit_field.u8acces_mem]
                    (frame.field.header.tipooperacion.bit_field.u8operation, i, frame.field.header.memaddr.addres, frame.field.ptu8Data, frame.field.header.u8NumData)) {
                default:
                case RUNNING_HTP:
                    frame.field.header.memoria.status = 0;
                    frame.field.header.u8NumData = 0;
                    frame.field.header.memoria.status |= ((uint16_t) PROTSLV_MEM_STAT_RUNNING) << (2 * (uint16_t) frame.field.header.tipooperacion.bit_field.u8acces_mem);
                    break;
                case ERROR_HTP:
                    frame.field.header.memoria.status = 0;
                    frame.field.header.u8NumData = 0;
                    frame.field.header.memoria.status |= ((uint16_t) PROTSLV_MEM_STAT_ABORTED) << (2 * (uint16_t) frame.field.header.tipooperacion.bit_field.u8acces_mem);
                    break;
                case IDLE_HTP:
                    frame.field.header.memoria.status = 0;
                    frame.field.header.u8NumData = 0;
                    frame.field.header.memoria.status |= ((uint16_t) PROTSLV_MEM_STAT_IDLE) << (2 * (uint16_t) frame.field.header.tipooperacion.bit_field.u8acces_mem);
                    break;
                case SUCCESS_HTP:
                    frame.field.header.memoria.status = 0;
                    frame.field.header.u8NumData = protslv.u8TamanyOrdreAcceptada;
                    frame.field.header.memaddr.addres = protslv.u32AddrOrdreAcceptada;
                    frame.field.header.memoria.status |= ((uint16_t) PROTSLV_MEM_STAT_FINISHED) << (2 * (uint16_t) frame.field.header.tipooperacion.bit_field.u8acces_mem);
                    break;
            }
            for (i = 0; i < CMP_MAX_MEMORIES; i++) {
                if (i != frame.field.header.tipooperacion.bit_field.u8acces_mem) {
                    frame.field.header.memoria.status |= ((uint16_t) MemoryMap.var.cabecera_fw.mems_status[i].bit_field.stat) << (2 * i);
                }
            }

            protslv.u8ProtocolMotor = PROTSLV_STAT_PREPARE_ANSWER;
            break;
        case PROTSLV_STAT_PREPARE_ANSWER:
            MemoryMap.var.puerto485_slv.u32TramasOK++;
            if (frame.field.header.u8AddrSlave == 0) {
                /* plot broadcast in context */
                // TODO WDT clear ! ClrWdt();
                protslv.u8ProtocolMotor = PROTSLV_STAT_ESPERANT_ADDR;
                retorn = SUCCESS_HTP;
            } else {
                /* plot that was for me, I will answer */
                frame.field.header.tipooperacion.bit_field.u8Direction = 1;
                frame.field.header.u8ProtocolVers = MemoryMap.var.cabecera_fw.board_status.status;
                frame.field.header.u8ChecksumSlaveAddr = MemoryMap.var.cabecera_fw.board_alarms.alarms; /* comodin 1 */
                frame.field.header.u8ChecksumMemAddr = MemoryMap.var.fuentes.mainStatus; /* comodin 2 */
                frame.field.u16FrameChecksum = 0;
                for (i = 0; i < sizeof (protslv_header_t); i++) {
                    frame.field.u16FrameChecksum += frame.ptucBytes[i];
                }
                for (i = 0; i < frame.field.header.u8NumData; i++) {
                    frame.field.u16FrameChecksum += frame.field.ptu8Data[i];
                }
                frame.field.u16FrameChecksum ^= 0xFFFF;
                frame.field.u16FrameChecksum++;

                protslv.u8ProtocolMotor = PROTSLV_STAT_ENVIAR_HEADER;

            }
            break;
        case PROTSLV_STAT_ENVIAR_HEADER: //send the header
            if (protslv.Transmit(RS485_SLV, (uint8_t *) (&frame.field.header), sizeof (protslv_header_t), 0) == SUCCESS_HTP) {
                protslv.u8ProtocolMotor = PROTSLV_STAT_ENVIAR_DADES;
            }
            break;
        case PROTSLV_STAT_ENVIAR_DADES://send the data
            if (protslv.Transmit(RS485_SLV, frame.field.ptu8Data, frame.field.header.u8NumData, 0) == SUCCESS_HTP) {
                protslv.u8ProtocolMotor = PROTSLV_STAT_ENVIAR_CHECKSUM;
            }
            break;
        case PROTSLV_STAT_ENVIAR_CHECKSUM://send the checksum
        	//frame.field.u16FrameChecksum = 0xffae;
            if (protslv.Transmit(RS485_SLV, (uint8_t *) (&frame.field.u16FrameChecksum), 2, 1) == SUCCESS_HTP) {
                //								HAL_GPIO_TogglePin(DBG_LED_2_GPIO_Port, DBG_LED_2_Pin);
                // TODO CLR WDT ClrWdt();
                retorn = SUCCESS_HTP;
                protslv.u8ProtocolMotor = PROTSLV_STAT_ESPERANT_ADDR;
                if (u8OperacioEnCurs != 0) {
                    retorn = ERROR_HTP;
                }
                retorn = SUCCESS_HTP;
            }
            break;
        case PROTSLV_STAT_ORDRE_KO:
            /* I will be receiving data, if there is any, ONLY if it is written */
            //						BUZZ_Configure(1, 80, PATRO_SO_WARNING);
            if (frame.field.header.tipooperacion.bit_field.u8operation == PROTSLV_OP_READING) {
                /* numb bytes es diferent de 0, pero es una lectura */
                protslv.u32TicksProtocol = TICKS_GetTicks();
                protslv.u8ProtocolMotor = PROTSLV_STAT_ESPERANT_CHECKSUM_ORDRE_KO;
            } else {
                protslv.u8BytesRebuts = 0;
                protslv.u8ProtocolMotor = PROTSLV_STAT_ESPERANT_DADES_ORDRE_KO1;
            }
            break;
        case PROTSLV_STAT_ESPERANT_DADES_ORDRE_KO1:
            protslv.u32TicksProtocol = TICKS_GetTicks();
            if (protslv.u8BytesRebuts < frame.field.header.u8NumData) {
                /* queden dades per rebre */
                protslv.u8ProtocolMotor = PROTSLV_STAT_ESPERANT_DADES_ORDRE_KO2;
            } else {
                protslv.u8ProtocolMotor = PROTSLV_STAT_ESPERANT_CHECKSUM_ORDRE_KO;
            }
            break;
        case PROTSLV_STAT_ESPERANT_DADES_ORDRE_KO2:
            if (protslv.Receive(RS485_SLV, &i, 1) == SUCCESS_HTP) {
                frame.field.u16FrameChecksum += i;
                protslv.u8BytesRebuts++;
            } else if (TICKS_DiffTicks(protslv.u32TicksProtocol) > PROTSLV_TIME_WAITING_DATA) {
                protslv.u8ProtocolMotor = PROTSLV_STAT_SINCRONISME1;
                u8ErrorNumber = 5; //10;
            }
            break;
        case PROTSLV_STAT_ESPERANT_CHECKSUM_ORDRE_KO:
            if (protslv.Receive(RS485_SLV, (uint8_t *) (&frame.field.u16FrameChecksum), 2) == SUCCESS_HTP) {
                protslv.u8ProtocolMotor = PROTSLV_STAT_VERIFICACIO_CHECKSUM_ORDRE_KO;
            } else if (TICKS_DiffTicks(protslv.u32TicksProtocol) > PROTSLV_TIME_WAITING_CHECKSUM) {
                protslv.u8ProtocolMotor = PROTSLV_STAT_SINCRONISME1;
                u8ErrorNumber = 6; //11;
            }
            break;
        case PROTSLV_STAT_VERIFICACIO_CHECKSUM_ORDRE_KO:
            for (i = 0; i < sizeof (protslv_header_t); i++) {
                frame.field.u16FrameChecksum += frame.ptucBytes[i];
            }
            if (Aux == 0) {
                /* correct package, i will send reply */
                frame.field.header.u8NumData = 0;
                frame.field.header.memoria.status |= ((uint16_t) PROTSLV_MEM_STAT_ABORTED) << (2 * (uint16_t) frame.field.header.tipooperacion.bit_field.u8acces_mem);
                protslv.u8ProtocolMotor = PROTSLV_STAT_PREPARE_ANSWER;
            } else {
                protslv.u8ProtocolMotor = PROTSLV_STAT_SINCRONISME1;
                u8ErrorNumber = 7; //14;
            }
            break;
        case PROTSLV_STAT_SINCRONISME1:
            //if(ATPARSER_AddFrameError(frame.field.header.u8AddrSlave, u8ErrorNumber+ATPARSER_SLV_INIT_ERRORS) == SUCCESS_HTP)
        {
            /* i'm going to sync for quiet time */
            //						BUZZ_Configure(1, 80, PATRO_SO_ALARMA);
            //						HAL_GPIO_WritePin(DBG_LED_4_GPIO_Port, DBG_LED_4_Pin, GPIO_PIN_SET);
            MemoryMap.var.puerto485_slv.u32TramasKO++; /* quizas es trama erronea porque no era para mi... */
            protslv.u32TicksProtocol = TICKS_GetTicks();
            protslv.u8ProtocolMotor = PROTSLV_STAT_SINCRONISME2;
        }
            break;
        case PROTSLV_STAT_SINCRONISME2:
            if (protslv.QuantsBytesBufferRX(RS485_SLV) > 0) {
                protslv.ResetBuffers(RS485_SLV);
                protslv.u32TicksProtocol = TICKS_GetTicks();
            } else if (TICKS_DiffTicks(protslv.u32TicksProtocol) > PROTSLV_TIME_WAITING_SYNC) {
                //								HAL_GPIO_WritePin(DBG_LED_4_GPIO_Port, DBG_LED_4_Pin, GPIO_PIN_RESET);
                protslv.u8ProtocolMotor = PROTSLV_STAT_ESPERANT_ADDR;
                retorn = ERROR_HTP;
            }
            break;
    }
    return retorn;
}

returnstatus_t ActionNULL(uint8_t u8Operation, uint8_t u8Order, uint32_t u32Addr, uint8_t* ptu8Dades, uint16_t u16NumDades) {
    //frame.field.header.errorcode.bit_field.u8MemNoDefinida = 1;
    return ERROR_HTP;
}

returnstatus_t CheckHeader(protslv_header_t cab) {
    uint8_t temporal, i;
    uint16_t Aux;

    if (cab.u8ProtocolVers != PROTSLV_VERSION) return ERROR_HTP;
    if (cab.tipooperacion.bit_field.u8Direction != 0) return ERROR_HTP;
    temporal = (uint8_t) (cab.u8AddrSlave + cab.tipooperacion.value + cab.u8ChecksumSlaveAddr);
    if (temporal != 0) return ERROR_HTP;

    temporal = cab.u8ChecksumMemAddr;
    for (i = 0; i < 4; i++) {
        temporal += cab.memaddr.ptu8Bytes[i];
    }
    if (temporal != 0) {
        return ERROR_HTP;
    }

    Aux = ((uint16_t) 0x0003) << (2 * (uint16_t) cab.tipooperacion.bit_field.u8acces_mem);
    if ((cab.memoria.status & (~Aux)) != 0x0000) {
        /* the validation field of the 1st byte of the frame is not the same as the composition of bytes 7 and 8 of the frame */
        /* the plot is to access a single peripheral */
        return ERROR_HTP;
    }

    return SUCCESS_HTP;
}
