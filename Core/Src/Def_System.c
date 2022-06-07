/*
 * File:   System.c
 * Author: ingenieria9
 *
 * Created on 23 de febrero de 2018, 10:54
 */


#include "Def_System.h"
#include "Def_ProtocolSlave.h"

// structure for read-destructive and read-only parameters
typedef union {

    struct {
       //board_definition
        uint8_t u8BoardAddres               : 1;
        uint8_t u8BoardType                 : 1;
        uint8_t u8MaxBuffRx                 : 1;
        uint8_t u8BootVers                  : 1;
        uint8_t u8FWVers                    : 1;
        uint8_t u8ProtVers                  : 1;
        uint8_t u8MinSlot                   : 1;
        uint8_t u8MaxSlot                   : 1;
        uint8_t u8SizeInternalUsedRAM_LO    : 1;
        uint8_t u8SizeInternalUsedRAM_MID_LO: 1;
        uint8_t u8SizeInternalUsedRAM_MID_HI: 1;
        uint8_t u8SizeInternalUsedRAM_HI    : 1;
        uint8_t u8SizeInternalRAM_LO        : 1;
        uint8_t u8SizeInternalRAM_MID_LO    : 1;
        uint8_t u8SizeInternalRAM_MID_HI    : 1;
        uint8_t u8SizeInternalRAM_HI        : 1;
        uint8_t u8SizeInternalFLASH_LO      : 1;
        uint8_t u8SizeInternalFLASH_MID_LO  : 1;
        uint8_t u8SizeInternalFLASH_MID_HI  : 1;
        uint8_t u8SizeInternalFLASH_HI      : 1;
        uint8_t u8SizeInternalE2PROM_LO     : 1;
        uint8_t u8SizeInternalE2PROM_MID_LO : 1;
        uint8_t u8SizeInternalE2PROM_MID_HI : 1;
        uint8_t u8SizeInternalE2PROM_HI     : 1;
        uint8_t u8SizeExternalMemory0_LO    : 1;
        uint8_t u8SizeExternalMemory0_MID_LO: 1;
        uint8_t u8SizeExternalMemory0_MID_HI: 1;
        uint8_t u8SizeExternalMemory0_HI    : 1;
        uint8_t u8SizeExternalMemory1_LO    : 1;
        uint8_t u8SizeExternalMemory1_MID_LO: 1;
        uint8_t u8SizeExternalMemory1_MID_HI: 1;
        uint8_t u8SizeExternalMemory1_HI    : 1;
        uint8_t u8SizeExternalMemory2_LO    : 1;
        uint8_t u8SizeExternalMemory2_MID_LO: 1;
        uint8_t u8SizeExternalMemory2_MID_HI: 1;
        uint8_t u8SizeExternalMemory2_HI    : 1;
        uint8_t u8SizeExternalMemory3_LO    : 1;
        uint8_t u8SizeExternalMemory3_MID_LO: 1;
        uint8_t u8SizeExternalMemory3_MID_HI: 1;
        uint8_t u8SizeExternalMemory3_HI    : 1;
        
        uint8_t u8BoardStatus               : 1;
        uint8_t u8BoardAlarms               : 1;
        uint8_t u8dummy_LO                  : 1;
        uint8_t u8dummy_HI                  : 1;
        uint8_t u8Accions_LO                : 1;
        uint8_t u8Accions_HI                : 1;
        uint8_t u8accioactual_LO            : 1;
        uint8_t u8accioactual_HI            : 1;
        uint8_t u8MemsStatusUsedRAM         : 1;
        uint8_t u8MemsStatusIntRAM          : 1;
        uint8_t u8MemsStatusIntFLASH        : 1;
        uint8_t u8MemsStatusIntE2PROM       : 1;
        uint8_t u8MemsStatusMemory0         : 1;
        uint8_t u8MemsStatusMemory1         : 1;
        uint8_t u8MemsStatusMemory2         : 1;
        uint8_t u8MemsStatusMemory3         : 1;
        uint8_t u8MemsAlarmsUsedRAM         : 1;
        uint8_t u8MemsAlarmsIntRAM          : 1;
        uint8_t u8MemsAlarmsIntFLASH        : 1;
        uint8_t u8MemsAlarmsIntE2PROM       : 1;
        uint8_t u8MemsAlarmsMemory0         : 1;
        uint8_t u8MemsAlarmsMemory1         : 1;
        uint8_t u8MemsAlarmsMemory2         : 1;
        uint8_t u8MemsAlarmsMemory3         : 1;
        
        //fuentes
        uint8_t mainStatus                  : 1; 
        uint8_t estadoModulos               : 1;
        uint8_t alarma                      : 1;
        uint8_t powerGoods                  : 1;
        uint8_t aModulos_ON_OFF_Fixed1      : 1;  
        uint8_t aModulos_ON_OFF_Fixed2      : 1; 
        uint8_t aModulos_ON_OFF_Adjustable1 : 1;  
        uint8_t aModulos_ON_OFF_Adjustable2 : 1;
        uint8_t aVoltagePercent0_LO         : 1;
        uint8_t aVoltagePercent0_MID_LO     : 1;
        uint8_t aVoltagePercent0_MID_HI     : 1;
        uint8_t aVoltagePercent0_HI         : 1;
        uint8_t aVoltagePercent1_LO         : 1;
        uint8_t aVoltagePercent1_MID_LO     : 1;
        uint8_t aVoltagePercent1_MID_HI     : 1;
        uint8_t aVoltagePercent1_HI         : 1;
        uint8_t aVoltagePercent2_LO         : 1;
        uint8_t aVoltagePercent2_MID_LO     : 1;
        uint8_t aVoltagePercent2_MID_HI     : 1;
        uint8_t aVoltagePercent2_HI         : 1;
        uint8_t aVoltagePercent3_LO         : 1;
        uint8_t aVoltagePercent3_MID_LO     : 1;
        uint8_t aVoltagePercent3_MID_HI     : 1;
        uint8_t aVoltagePercent3_HI         : 1;
        
        //puerto485_slv
        uint8_t u8TramasOK_LO               : 1;
        uint8_t u8TramasOK_MID_LO           : 1;
        uint8_t u8TramasOK_MID_HI           : 1;
        uint8_t u8TramasOK_HI               : 1;
        uint8_t u8TramasKO_LO               : 1;
        uint8_t u8TramasKO_MID_LO           : 1;
        uint8_t u8TramasKO_MID_HI           : 1;
        uint8_t u8TramasKO_HI               : 1;
    } bit_field;
    uint8_t ptucBytes[(sizeof (memory_map_def_t)/8) + 1];
} flags_registro_t;

const flags_registro_t reg_lect_destructiva ={
        //board_definition   
        /*u8BoardAddres               = */0,
        /*u8BoardType                 = */0,
        /*u8MaxBuffRx                 = */0,
        /*u8BootVers                  = */0,
        /*u8FWVers                    = */0,
        /*u8ProtVers                  = */0,
        /*u8MinSlot                   = */0,
        /*u8MaxSlot                   = */0,
        /*u8SizeInternalUsedRAM_LO    = */0,
        /*u8SizeInternalUsedRAM_MID_LO= */0,
        /*u8SizeInternalUsedRAM_MID_HI= */0,
        /*u8SizeInternalUsedRAM_HI    = */0,
        /*u8SizeInternalRAM_LO        = */0,
        /*u8SizeInternalRAM_MID_LO    = */0,
        /*u8SizeInternalRAM_MID_HI    = */0,
        /*u8SizeInternalRAM_HI        = */0,
        /*u8SizeInternalFLASH_LO      = */0,
        /*u8SizeInternalFLASH_MID_LO  = */0,
        /*u8SizeInternalFLASH_MID_HI  = */0,
        /*u8SizeInternalFLASH_HI      = */0,
        /*u8SizeInternalE2PROM_LO     = */0,
        /*u8SizeInternalE2PROM_MID_LO = */0,
        /*u8SizeInternalE2PROM_MID_HI = */0,
        /*u8SizeInternalE2PROM_HI     = */0,
        /*u8SizeExternalMemory0_LO    = */0,
        /*u8SizeExternalMemory0_MID_LO= */0,
        /*u8SizeExternalMemory0_MID_HI= */0,
        /*u8SizeExternalMemory0_HI    = */0,
        /*u8SizeExternalMemory1_LO    = */0,
        /*u8SizeExternalMemory1_MID_LO= */0,
        /*u8SizeExternalMemory1_MID_HI= */0,
        /*u8SizeExternalMemory1_HI    = */0,
        /*u8SizeExternalMemory2_LO    = */0,
        /*u8SizeExternalMemory2_MID_LO= */0,
        /*u8SizeExternalMemory2_MID_HI= */0,
        /*u8SizeExternalMemory2_HI    = */0,
        /*u8SizeExternalMemory3_LO    = */0,
        /*u8SizeExternalMemory3_MID_LO= */0,
        /*u8SizeExternalMemory3_MID_HI= */0,
        /*u8SizeExternalMemory3_HI    = */0,
       
        /*u8BoardStatus               = */0,
        /*u8BoardAlarms               = */1,
        /*u8dummy_LO                  = */0,
        /*u8dummy_HI                  = */0,
        /*u8Accions_LO                = */0,
        /*u8Accions_HI                = */0,
        /*u8accioactual_LO            = */0,
        /*u8accioactual_HI            = */0,
        /*u8MemsStatusUsedRAM         = */0,
        /*u8MemsStatusIntRAM          = */0,
        /*u8MemsStatusIntFLASH        = */0,
        /*u8MemsStatusIntE2PROM       = */0,
        /*u8MemsStatusMemory0         = */0,
        /*u8MemsStatusMemory1         = */0,
        /*u8MemsStatusMemory2         = */0,
        /*u8MemsStatusMemory3         = */0,
        /*u8MemsAlarmsUsedRAM         = */0,
        /*u8MemsAlarmsIntRAM          = */0,
        /*u8MemsAlarmsIntFLASH        = */0,
        /*u8MemsAlarmsIntE2PROM       = */0,
        /*u8MemsAlarmsMemory0         = */0,
        /*u8MemsAlarmsMemory1         = */0,
        /*u8MemsAlarmsMemory2         = */0,
        /*u8MemsAlarmsMemory3         = */0,
       
        //fuentes                                  
        /*mainStatus                  = */0, 
        /*estadoModulos               = */0,
        /*alarma                      = */0,
        /*powerGoods                  = */0,
        /*aModulos_ON_OFF_Fixed1      = */0,  
        /*aModulos_ON_OFF_Fixed2      = */0, 
        /*aModulos_ON_OFF_Adjustable1 = */0,  
        /*aModulos_ON_OFF_Adjustable2 = */0,
        /*aVoltagePercent0_LO         = */0,
        /*aVoltagePercent0_MID_LO     = */0,
        /*aVoltagePercent0_MID_HI     = */0,
        /*aVoltagePercent0_HI         = */0,
        /*aVoltagePercent1_LO         = */0,
        /*aVoltagePercent1_MID_LO     = */0,
        /*aVoltagePercent1_MID_HI     = */0,
        /*aVoltagePercent1_HI         = */0,
        /*aVoltagePercent2_LO         = */0,
        /*aVoltagePercent2_MID_LO     = */0,
        /*aVoltagePercent2_MID_HI     = */0,
        /*aVoltagePercent2_HI         = */0,
        /*aVoltagePercent3_LO         = */0,
        /*aVoltagePercent3_MID_LO     = */0,
        /*aVoltagePercent3_MID_HI     = */0,
        /*aVoltagePercent3_HI         = */0,
      
        //puerto485_slv
        /*u8TramasOK_LO               = */0,
        /*u8TramasOK_MID_LO           = */0,
        /*u8TramasOK_MID_HI           = */0,
        /*u8TramasOK_HI               = */0,
        /*u8TramasKO_LO               = */0,
        /*u8TramasKO_MID_LO           = */0,
        /*u8TramasKO_MID_HI           = */0,
        /*u8TramasKO_HI               = */0,                                                   
};

const flags_registro_t reg_read_only ={
      //board_definition   
        /*u8BoardAddres               = */0, //0
        /*u8BoardType                 = */0,
        /*u8MaxBuffRx                 = */0,
        /*u8BootVers                  = */0,
        /*u8FWVers                    = */0,
        /*u8ProtVers                  = */0,
        /*u8MinSlot                   = */0,
        /*u8MaxSlot                   = */0,
        /*u8SizeInternalUsedRAM_LO    = */0,
        /*u8SizeInternalUsedRAM_MID_LO= */0,
        /*u8SizeInternalUsedRAM_MID_HI= */0, //10
        /*u8SizeInternalUsedRAM_HI    = */0,
        /*u8SizeInternalRAM_LO        = */0,
        /*u8SizeInternalRAM_MID_LO    = */0,
        /*u8SizeInternalRAM_MID_HI    = */0,
        /*u8SizeInternalRAM_HI        = */0,
        /*u8SizeInternalFLASH_LO      = */0,
        /*u8SizeInternalFLASH_MID_LO  = */0,
        /*u8SizeInternalFLASH_MID_HI  = */0,
        /*u8SizeInternalFLASH_HI      = */0,
        /*u8SizeInternalE2PROM_LO     = */0, //20
        /*u8SizeInternalE2PROM_MID_LO = */0,
        /*u8SizeInternalE2PROM_MID_HI = */0,
        /*u8SizeInternalE2PROM_HI     = */0,
        /*u8SizeExternalMemory0_LO    = */0,
        /*u8SizeExternalMemory0_MID_LO= */0,
        /*u8SizeExternalMemory0_MID_HI= */0,
        /*u8SizeExternalMemory0_HI    = */0,
        /*u8SizeExternalMemory1_LO    = */0,
        /*u8SizeExternalMemory1_MID_LO= */0,
        /*u8SizeExternalMemory1_MID_HI= */0, //30
        /*u8SizeExternalMemory1_HI    = */0,
        /*u8SizeExternalMemory2_LO    = */0,
        /*u8SizeExternalMemory2_MID_LO= */0,
        /*u8SizeExternalMemory2_MID_HI= */0,
        /*u8SizeExternalMemory2_HI    = */0,
        /*u8SizeExternalMemory3_LO    = */0,
        /*u8SizeExternalMemory3_MID_LO= */0,
        /*u8SizeExternalMemory3_MID_HI= */0,
        /*u8SizeExternalMemory3_HI    = */0,
       
        /*u8BoardStatus               = */1, //40
        /*u8BoardAlarms               = */0,
        /*u8dummy_LO                  = */0,
        /*u8dummy_HI                  = */0,
        /*u8Accions_LO                = */0, //44
        /*u8Accions_HI                = */0,
        /*u8accioactual_LO            = */0,
        /*u8accioactual_HI            = */0,
        /*u8MemsStatusUsedRAM         = */0,
        /*u8MemsStatusIntRAM          = */0,
        /*u8MemsStatusIntFLASH        = */0, //50
        /*u8MemsStatusIntE2PROM       = */0,
        /*u8MemsStatusMemory0         = */0,
        /*u8MemsStatusMemory1         = */0,
        /*u8MemsStatusMemory2         = */0,
        /*u8MemsStatusMemory3         = */0,
        /*u8MemsAlarmsUsedRAM         = */0,
        /*u8MemsAlarmsIntRAM          = */0,
        /*u8MemsAlarmsIntFLASH        = */0,
        /*u8MemsAlarmsIntE2PROM       = */0,
        /*u8MemsAlarmsMemory0         = */0, //60
        /*u8MemsAlarmsMemory1         = */0,
        /*u8MemsAlarmsMemory2         = */0,
        /*u8MemsAlarmsMemory3         = */0,
       
        //fuentes                                  
        /*mainStatus                  = */0, //64
        /*estadoModulos               = */1,
        /*alarma                      = */0,
        /*powerGoods                  = */1,
        /*aModulos_ON_OFF_Fixed1      = */0,  
        /*aModulos_ON_OFF_Fixed2      = */0, 
        /*aModulos_ON_OFF_Adjustable1 = */0,  //70
        /*aModulos_ON_OFF_Adjustable2 = */0,
        /*aVoltagePercent0_LO         = */0,
        /*aVoltagePercent0_MID_LO     = */0,
        /*aVoltagePercent0_MID_HI     = */0,
        /*aVoltagePercent0_HI         = */0,
        /*aVoltagePercent1_LO         = */0,
        /*aVoltagePercent1_MID_LO     = */0,
        /*aVoltagePercent1_MID_HI     = */0,
        /*aVoltagePercent1_HI         = */0,
        /*aVoltagePercent2_LO         = */0,
        /*aVoltagePercent2_MID_LO     = */0,
        /*aVoltagePercent2_MID_HI     = */0,
        /*aVoltagePercent2_HI         = */0,
        /*aVoltagePercent3_LO         = */0,
        /*aVoltagePercent3_MID_LO     = */0,
        /*aVoltagePercent3_MID_HI     = */0,
        /*aVoltagePercent3_HI         = */0,
      
        //puerto485_slv
        /*u8TramasOK_LO               = */0,
        /*u8TramasOK_MID_LO           = */0,
        /*u8TramasOK_MID_HI           = */0,
        /*u8TramasOK_HI               = */0,
        /*u8TramasKO_LO               = */0,
        /*u8TramasKO_MID_LO           = */0,
        /*u8TramasKO_MID_HI           = */0,
        /*u8TramasKO_HI               = */0,  
};


memory_map_t MemoryMap;

uint8_t esLecturaDestructiva(uint16_t u16Addr);

uint8_t isReadOnlyRegister(uint16_t u16Addr);

void SYS_Init(void)
{
    uint8_t i;
    
    MemoryMap.var.cabecera_fw.board_definition.u8BoardAddres = FNT_DEFAULT_ADDR;
    MemoryMap.var.cabecera_fw.board_definition.u8BoardType = FNT_BOARD_TYPE;
    MemoryMap.var.cabecera_fw.board_definition.u8BootVers = 5;//u8BootVersion;
    MemoryMap.var.cabecera_fw.board_definition.u8FWVers = FNT_FW_VERSION;
    MemoryMap.var.cabecera_fw.board_definition.u8MinSlot = EXP_MIN_SLOT;
    MemoryMap.var.cabecera_fw.board_definition.u8MaxSlot = EXP_MAX_SLOT;
//    MemoryMap.board_definition.u8ProtVers /* the protocol touches this variable in the initialization */
    MemoryMap.var.cabecera_fw.board_definition.u8MaxBuffRx = CMP_MAX_BUF_RX;
    
    MemoryMap.var.cabecera_fw.board_definition.SizeMemories[CMP_INTERNAL_USED_RAM].u32value = CMP_INTERNAL_USED_RAM_SIZE;
    MemoryMap.var.cabecera_fw.board_definition.SizeMemories[CMP_INTERNAL_RAM].u32value = CMP_INTERNAL_RAM_SIZE;
    MemoryMap.var.cabecera_fw.board_definition.SizeMemories[CMP_INTERNAL_FLASH].u32value = CMP_INTERNAL_FLASH_SIZE;
    MemoryMap.var.cabecera_fw.board_definition.SizeMemories[CMP_INTERNAL_EEPROM].u32value = CMP_INTERNAL_EEPROM_SIZE;
    MemoryMap.var.cabecera_fw.board_definition.SizeMemories[CMP_UNDEFINED_4].u32value = EXP_UNDEFINED_4_SIZE;
    MemoryMap.var.cabecera_fw.board_definition.SizeMemories[CMP_UNDEFINED_5].u32value = EXP_UNDEFINED_5_SIZE;
    MemoryMap.var.cabecera_fw.board_definition.SizeMemories[CMP_UNDEFINED_6].u32value = EXP_UNDEFINED_6_SIZE;
    MemoryMap.var.cabecera_fw.board_definition.SizeMemories[CMP_UNDEFINED_7].u32value = EXP_UNDEFINED_7_SIZE;
    
    MemoryMap.var.cabecera_fw.board_status.status = 0;
    MemoryMap.var.cabecera_fw.board_alarms.alarms = 0;
    for(i = 0; i < CMP_MAX_MEMORIES; i++)
    {
        MemoryMap.var.cabecera_fw.mems_status[i].status = 0;
        MemoryMap.var.cabecera_fw.mems_alarms[i].alarms = 0;
    }
    MemoryMap.var.cabecera_fw.u16accions = 0;
    MemoryMap.var.cabecera_fw.u16accioactual = 0;
		
		
}

returnstatus_t SYS_GetParam_Char(uint16_t u16Parametro, uint8_t *ptu8Value, uint16_t u16NumReg)
{
    uint32_t i;
    for (i = 0; i < u16NumReg; i++){
        if(i >= sizeof(memory_map_t)) return ERROR_HTP;     //write outside the structure
        ptu8Value[i] = MemoryMap.ptucBytes[u16Parametro];
        if(esLecturaDestructiva(u16Parametro) != 0)
        {
            MemoryMap.ptucBytes[u16Parametro] = 0;
        }
        u16Parametro++;
    }
    return SUCCESS_HTP;
}

returnstatus_t SYS_SetParam_Char(uint16_t u16Parametro, uint8_t *ptu8Value, uint16_t u16NumReg)
{
    uint32_t i;
    for (i = 0; i < u16NumReg; i++){
        if(i >= sizeof(memory_map_t)) return ERROR_HTP;     //write outside the structure
        if(isReadOnlyRegister(u16Parametro) == 0)
        {
            MemoryMap.ptucBytes[u16Parametro] = ptu8Value[i];
        }
        u16Parametro++;
    }
    return SUCCESS_HTP;
}

returnstatus_t SYS_Action(uint8_t u8Operation, uint8_t u8Order, uint32_t u32Addr, uint8_t* ptu8Dades, uint16_t u16NumDades)
{
  returnstatus_t temp;
	uint8_t aux;
    
	if((u8Order) == PROTSLV_ORDER_ORDER)
	{
		if(u8Operation == PROTSLV_OP_READING)
		{
			if(SYS_GetParam_Char(u32Addr, ptu8Dades, u16NumDades) == ERROR_HTP)
			{
					MemoryMap.var.cabecera_fw.mems_alarms[CMP_INTERNAL_USED_RAM].bit_field.u8AccesForaRang = 1;
					temp = ERROR_HTP;
			}
			else
			{
					temp = SUCCESS_HTP;
			}
		}
		else
		{
			if((u32Addr == 24) && (ptu8Dades[0] == 1))
			{
					MemoryMap.var.cabecera_fw.mems_alarms[CMP_INTERNAL_USED_RAM].bit_field.u8AccesForaRang = 0;
			}
			if(SYS_SetParam_Char(u32Addr, ptu8Dades, u16NumDades) == ERROR_HTP)
			{
					MemoryMap.var.cabecera_fw.mems_alarms[CMP_INTERNAL_USED_RAM].bit_field.u8AccesForaRang = 1;
					temp = ERROR_HTP;
			}
			else
			{
					temp = SUCCESS_HTP;
			}
		}
	}
	else if(u8Operation == PROTSLV_MEM_STAT_ABORTED)
	{
		/* it will never be given ... orders end immediately */
		temp = ERROR_HTP;
	}
	else
	{
		aux = MemoryMap.var.cabecera_fw.mems_status[CMP_INTERNAL_USED_RAM].bit_field.stat;
		if(aux == PROTSLV_MEM_STAT_ABORTED)
		{
			MemoryMap.var.cabecera_fw.mems_status[CMP_INTERNAL_USED_RAM].bit_field.stat = PROTSLV_MEM_STAT_IDLE;
			temp = ERROR_HTP;
		}
		else if(aux == PROTSLV_MEM_STAT_FINISHED)
		{
			MemoryMap.var.cabecera_fw.mems_status[CMP_INTERNAL_USED_RAM].bit_field.stat = PROTSLV_MEM_STAT_IDLE;
			temp = SUCCESS_HTP;
		}
		else if(aux == PROTSLV_MEM_STAT_RUNNING)
		{
			temp = RUNNING_HTP;
		}
		else
		{
			temp = IDLE_HTP;
		}
	}
	return temp;
}



uint8_t esLecturaDestructiva(uint16_t u16Addr)
{
	return 0;//(uint8_t)(reg_lect_destructiva.ptucBytes[u16Addr/8]&(1<<u16Addr&0x07));
}

uint8_t isReadOnlyRegister(uint16_t u16Addr)
{
	return 0;//(uint8_t)(reg_read_only.ptucBytes[u16Addr/8]&(1<<u16Addr&0x07));
}

