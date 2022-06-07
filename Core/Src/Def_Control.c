/*
 * File:   control.c
 * Author: ingenieria9
 *
 * Created on 27 de marzo de 2018, 12:33
 */


#include "Def_Control.h"
#include "Def_Standby.h"
#include "Def_ProtocolSlave.h"

#if (FNT_FW_VERSION > 20)
#define INVERT_LOGIC_ENABLE
#endif

#define TIME_REFRESH_CH 25 //Refresh time between channels
#define TIME_REFRESH_FUENTES_ON_OFF 10 //Refresh time to turn on or off the secondary modules of the sources
#define FNT_TIME_REFRESH_FAN 10 //Refresh time to update the fan_ok signal

identif_t identif;
fuentesStatus_t fuentesStatus;

typedef struct {
    uint8_t u8b1;
    uint8_t u8b2;
    uint8_t u8b3;
    uint8_t enviar[3];
    uint16_t u16vTrim;
    uint16_t u16vPer;
    float f_vTrim;
    float f_vPer;
} SPI_CH_t;

typedef struct {
    uint8_t u8Motor;
    uint8_t u8ActCH;
    uint32_t u32Timer;
    SPI_CH_t SPI_CH[MAX_NUM_SPI_CH];
} SPI_t;

typedef struct {
    uint8_t u8Motor;
    uint32_t u32Timer;
} fuentes_ON_OFF_t;


SPI_t SPIst;
uint8_t badChannel;

fuentes_ON_OFF_t fuentes_ON_OFF;

//debug
uint8_t testNum;
uint8_t debControl;
uint16_t i;

void CTR_identif(void) {

	prtEN_PS_Global(0);
	prtOutput1_ModulosOff(0);
	prtOutput2_ModulosOff(0);

#ifdef SOLO_2_FUENTES
    identif.IdFuente1 = UX6;
    identif.IdFuente2 = NONE;
    

        identif.IdPCB = PCB1;		// Excelsys, PCB 1, only Exelsys is using !!! iii


//        MemoryMap.var.cabecera_fw.board_definition.u8BoardAddres = 1;

// We identify the source 1 that is connected from the bridge of pins 11-12 (Artesin), 13-14 (Excelsys)
//    if(prtExcelsys2 == 0 && prtArtesin2 == 0 && prtArtesin1 == 0) {
//		/* in the second connector there is nothing and the jumper of pins 11-12 is not connected */
//        if (prtExcelsys1 == 1) {
//		/* the jumper of pins 13-14 is connected => PCB2 */
//            identif.IdPCB = PCB2;
//            MemoryMap.var.cabecera_fw.board_definition.u8BoardAddres = 2;
//        } else {
//		/* the jumper of pins 13-14 is NOT connected => PCB1 */
//            identif.IdPCB = PCB1;
//            MemoryMap.var.cabecera_fw.board_definition.u8BoardAddres = 1;
//        }
//    } else {
///* in the second connector there is something and it shouldn't or the jumper of pins 11-12 is connected */
//        identif.IdFuente1 = ERROR;
//        identif.IdFuente2 = ERROR;
//        identif.IdPCB = NOT_DEFINED;
//        MemoryMap.var.cabecera_fw.board_definition.u8BoardAddres = 0;
//        LED_Configure(LED1, 0xFF, 100, 100, 0); //Yellow LED infinite blink
//    }
#else
// We identify the source 1 that is connected from the bridge of pins 11-12 (Artesin), 13-14 (Excelsys)
    if (prtExcelsys1 == 1 && prtArtesin1 == 0) {
        identif.IdFuente1 = UX6;
    } else if (prtExcelsys1 == 0 && prtArtesin1 == 1) {
        identif.IdFuente1 = UX4;
    } else if (prtExcelsys1 == 0 && prtArtesin1 == 0) {
        identif.IdFuente1 = NONE;
    } else {
        identif.IdFuente1 = ERROR;
    }

// We identify the source 2 that is connected from the bridge of pins 11-12, 13-14
    if (prtExcelsys2 == 1 && prtArtesin2 == 0) {
        identif.IdFuente2 = UX6;
    } else if (prtExcelsys2 == 0 && prtArtesin2 == 1) {
        identif.IdFuente2 = UX4;
    } else if (prtExcelsys2 == 0 && prtArtesin2 == 0) {
        identif.IdFuente2 = NONE;
    } else {
        identif.IdFuente2 = ERROR;
    }

    // We identify the PCB from the sources that are connected
    if (identif.IdFuente1 == UX4 && identif.IdFuente2 == UX6) {
        identif.IdPCB = PCB1;
        MemoryMap.var.cabecera_fw.board_definition.u8BoardAddres = 1;
    } else if (identif.IdFuente1 == UX6 && identif.IdFuente2 == NONE) {
        identif.IdPCB = PCB2;
        MemoryMap.var.cabecera_fw.board_definition.u8BoardAddres = 2;
    } else {
        identif.IdPCB = NOT_DEFINED;
        LED_Configure(LED1, 0xFF, 100, 100, 0); //Yellow LED infinite blink
    }
#endif

}

void alarmsControl_init(void) {
    fuentesStatus.u8MotorFan = 0;
    MemoryMap.var.cabecera_fw.board_alarms.bit_field.RS584_Error = 0;
    MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente1TempFail = 0;
    MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente2TempFail = 0;
    MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente1FanFail = 0;
    MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente2FanFail = 0;
    MemoryMap.var.cabecera_fw.board_alarms.bit_field.AlarmaExterna = 0;
}

void alarmsControl_motor(void) {
    uint8_t i;
    uint8_t u8Error = 0;

prtDL1(1);
    //We see if from main they send us to lower or raise the alarm signal
    if ((MemoryMap.var.fuentes.alarma == FNT_MAIN_ALARM_OFF)||(MemoryMap.var.fuentes.alarma == FNT_MAIN_ALARM_NONE)) {
        //We only deactivate the alarm if we no longer see the alarms that have lowered the signal
#ifdef SOLO_2_FUENTES
        if ((prtInput1_AC_fail == 0) && (prtInput1_TempFail == 0)) {
			//Deactivate alarm line
            //MemoryMap.var.cabecera_fw.board_alarms.bit_field.AlarmaExterna = 0;
            prtAlarmaOut(0);
        }
        if (prtAlarmaIn == 0) {
            MemoryMap.var.cabecera_fw.board_alarms.bit_field.AlarmaExterna = 0;
        }
#else
        if ((identif.IdPCB == PCB1) && (prtInput1_AC_fail == 0) && (prtInput1_TempFail == 0)) {
			//Deactivate alarm line
        	prtAlarmaOut(0);
        }
		//On PCB2 there are no signals that lower the alarm, so it can be deactivated although it should never have been raised
        if (identif.IdPCB == PCB2) {
			//Deactivate alarm line
        	prtAlarmaOut(0);
        }
#endif
    } else if (MemoryMap.var.fuentes.alarma == FNT_MAIN_ALARM_ON) {
        //activate alarm line
    	prtAlarmaOut(1);
        for (i = 0; i < FNT_NUM_FNTS_ADJUST; i++)
        {
            MemoryMap.var.fuentes.aVoltagePercent[i].fvalue = 0;
        }
        for (i = 0; i < FNT_NUM_MODULES; i++)
        {
            MemoryMap.var.fuentes.aModulos_ON_OFF[i] = 0;
        }
        if(identif.IdPCB == PCB2)
        {
            /* if it's PCB2 I don't turn off the digital source */
            MemoryMap.var.fuentes.aModulos_ON_OFF[1] = 1;
        }
    }
    if (prtInput1_AC_fail == 1) {
            MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente1ACFail = 1;
    }
    else
    {
        MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente1ACFail = 0;
    }
    if (prtInput1_TempFail == 1) {
            MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente1TempFail = 1;
    }
    else
    {
        MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente1TempFail = 0;
    }
    //debControl = prtAlarmaOut;

    //TempFail1
    for (i = 0; i < FNT_NUM_TEMP_FAIL; i++) {
        fuentesStatus.atempFail1[i] = prtInput1_TempFail;
        if (fuentesStatus.atempFail1[i] == 1) {
            //If we see the alarm signal FNT_NUM_TEMP_FAIL consecutive times, we give as alarm
            u8Error = 1;

        } else {
            u8Error = 0;
            break;
        }
    }
    if (u8Error == 1) {
        //activate alarm line
    	prtAlarmaOut(1);
        //We notify by protocol that there has been an alarm
        MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente1TempFail = 1;
        //Activate alarm flag
        //  fuentesStatus.tempFail1 = 1;
    }

    //TempFail2
    u8Error = 0;
    for (i = 0; i < FNT_NUM_TEMP_FAIL; i++) {
        fuentesStatus.atempFail2[i] = prtInput2_TempFail;
        if (fuentesStatus.atempFail2[i] == 1) {
            //If we see the alarm signal FNT_NUM_TEMP_FAIL consecutive times, we give as alarm
            u8Error = 1;

        } else {
            u8Error = 0;
            break;
        }
    }
    if (u8Error == 1) {
        //We notify by protocol that there has been an alarm
        MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente2TempFail = 1;
        //Activate alarm flag
        // fuentesStatus.tempFail2 = 1;
    }

    //FanFail1/2
    switch (fuentesStatus.u8MotorFan) {
        case 0:
            //We save the status of the fan signal
            fuentesStatus.afanFail1[0] = prtInput1_FanOK;
            fuentesStatus.afanFail2[0] = prtInput2_FanOK;

            //We capture refresh time between each measurement
            fuentesStatus.u32fanTimer = TICKS_GetTicks();
            //We advance to the next state
            fuentesStatus.u8MotorFan++;
            break;

        case 1:
            //We check if the refresh time has expired
            if (TICKS_DiffTicks(fuentesStatus.u32fanTimer) > FNT_TIME_REFRESH_FAN) {
                fuentesStatus.afanFail1[1] = prtInput1_FanOK;
                fuentesStatus.afanFail2[1] = prtInput2_FanOK;
                fuentesStatus.u8MotorFan++;
            }
            break;

        case 2:
            if (fuentesStatus.afanFail1[0] == 0 && fuentesStatus.afanFail1[1] == 0) {
                //We notify by protocol that there has been an alarm
                MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente1FanFail = 1;
                //Activate alarm flag
                //   fuentesStatus.fanFail1 = 1;
            }
            else
            {
                MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente1FanFail = 0;
            }

            if (fuentesStatus.afanFail2[0] == 0 && fuentesStatus.afanFail2[1] == 0) {
                //We notify by protocol that there has been an alarm
                MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente2FanFail = 1;
                //Activate alarm flag
                //   fuentesStatus.fanFail1 = 1;
            }
            else
            {
                MemoryMap.var.cabecera_fw.board_alarms.bit_field.fuente2FanFail = 0;
            }
            //We return to the initial state
            fuentesStatus.u8MotorFan = 0;
            break;


        default:
            //We return to the initial state
            fuentesStatus.u8MotorFan = 0;
            break;

    }


}

void CTR_fuentes_ON_OFF_init(void) {

    //Initialize the source engine
    fuentes_ON_OFF.u8Motor = 0;

}

void CTR_fuentes_ON_OFF_motor(void) {

    switch (fuentes_ON_OFF.u8Motor) {
        case 0:
            //We capture refresh time between each measurement
            fuentes_ON_OFF.u32Timer = TICKS_GetTicks();
            //We advance to the next state
            fuentes_ON_OFF.u8Motor++;
            break;

        case 1:
            //We check if the refresh time has expired
            if (TICKS_DiffTicks(fuentes_ON_OFF.u32Timer) > TIME_REFRESH_FUENTES_ON_OFF) {

                //void CTR_fuentes_ON_OFF(void) {

                if (standby.fuentesHab.digital == 1 && (MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_FIXED1] == 1)) {//0b00000001) { //&&protocol  
                    standby.fuentesOn.digital = 1;
                } else if (standby.fuentesHab.digital == 0 || ((MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_FIXED1] == 0) && (MemoryMap.var.fuentes.mainStatus != 0))) {
                    standby.fuentesOn.digital = 0;
                }

                if (standby.fuentesHab.motor == 1 && (MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_FIXED2] == 1)) { //&&protocol
                    standby.fuentesOn.motor = 1;
                } else if (standby.fuentesHab.motor == 0 || ((MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_FIXED2] == 0) && (MemoryMap.var.fuentes.mainStatus != 0))) {
                    standby.fuentesOn.motor = 0;
                }

                if (standby.fuentesHab.apl1 == 1 && (MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_ADJUSTABLE1] == 1)) { //&&protocol
                    standby.fuentesOn.apl1 = 1;
                } else if (standby.fuentesHab.apl1 == 0 || ((MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_ADJUSTABLE1] == 0) && (MemoryMap.var.fuentes.mainStatus != 0))) {
                    standby.fuentesOn.apl1 = 0;
                }

                if (standby.fuentesHab.apl2 == 1 && (MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_ADJUSTABLE2] == 1)) { //&&protocol
                    standby.fuentesOn.apl2 = 1;
                } else if (standby.fuentesHab.apl2 == 0 || ((MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_ADJUSTABLE2] == 0) && (MemoryMap.var.fuentes.mainStatus != 0))) {
                    standby.fuentesOn.apl2 = 0;
                }

                //We update the status of the modules
                if (standby.fuentesOn.digital == 1) {
                    //The 24V digital module is also turned on by pressing the standby button
                    prtEN_FA_DigitalN(0); //24V digital module on
                } else {
                    prtEN_FA_DigitalN(1); //24V digital module off
                }

                if (standby.fuentesOn.motor == 1) {
                    prtEN_FA_MotorN(0); //motor module on
                } else {
                    prtEN_FA_MotorN(1); //motor module off
                }

                if (standby.fuentesOn.apl1 == 1) {
                    //We turn on the peltier modules in the spi motor, after updating the DAC
                } else {

                    //prtEN_Apl1n = 0; Peltiers module 1 off The logic of the signal has been changed
                	if (frame.field.header.u8AddrSlave == 1) {
						prtEN_Apl1n(1); //Peltier module 1 off
						prtEN_Apl2n(1);
                	} else if (frame.field.header.u8AddrSlave == 2) {
						prtEN_Apl5n(1);
						prtEN_Apl6n(1);
                	}

                    /* we set the set points to 0 */
                    MemoryMap.var.fuentes.aVoltagePercent[0].fvalue = 0; //iii TODO
                    MemoryMap.var.fuentes.aVoltagePercent[1].fvalue = 0;
                }

                if (standby.fuentesOn.apl2 == 1) {
                    //We turn on the peltier modules in the spi motor, after updating the DAC
                } else {

                    //prtEN_Apl2n = 0; //Peltiers module 2 off The logic of the signal has been changed
                	if (frame.field.header.u8AddrSlave == 1) {
						prtEN_Apl3n(1); //Peltier module 1 off
						prtEN_Apl4n(1);
                	} else if (frame.field.header.u8AddrSlave == 2) {
						prtEN_Apl7n(1);
						prtEN_Apl8n(1);
                	}

                    /* we set the set points to 0 */
                    MemoryMap.var.fuentes.aVoltagePercent[2].fvalue = 0;  //iii TODO
                    MemoryMap.var.fuentes.aVoltagePercent[3].fvalue = 0;
                }

                //We advance to the next state
                fuentes_ON_OFF.u8Motor++;
            }

            break;

        case 2:
            //We update the state variables of the protocol modules TODO update pins with read macro!
#ifndef INVERT_LOGIC_ENABLE
//          MemoryMap.var.fuentes.estadoModulos = (uint8_t) (((!prtEN_Apl2n) << 3) | ((!prtEN_Apl1n) << 2) | ((!prtEN_FA_MotorN) << 1) | (!prtEN_FA_DigitalN));
#else
//          MemoryMap.var.fuentes.estadoModulos = (uint8_t) (((prtEN_Apl2n) << 3) | ((prtEN_Apl1n) << 2) | ((!prtEN_FA_MotorN) << 1) | (!prtEN_FA_DigitalN)); //enable signal logic changed
        	if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13)) MemoryMap.var.fuentes.estadoModulos |= (1<<3); else MemoryMap.var.fuentes.estadoModulos &= ~(1<<3); //prtEN_Apl2n
        	if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12)) MemoryMap.var.fuentes.estadoModulos |= (1<<2); else MemoryMap.var.fuentes.estadoModulos &= ~(1<<2); //prtEN_Apl1n
        	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)) MemoryMap.var.fuentes.estadoModulos |= (1<<1); else MemoryMap.var.fuentes.estadoModulos &= ~(1<<1);  //prtEN_FA_MotorN
        	if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7)) MemoryMap.var.fuentes.estadoModulos |= (1<<0); else MemoryMap.var.fuentes.estadoModulos &= ~(1<<0); //prtEN_FA_DigitalN

#endif

            //We update the power goods
// TODO           MemoryMap.var.fuentes.powerGoods = (uint8_t) (((!prtPG_Plt2n) << 2) | ((!prtPG_Plt1n) << 1) | (!prtEN_FA_DigitalN));
        	if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7)) MemoryMap.var.fuentes.powerGoods &= ~(1<<0); else MemoryMap.var.fuentes.powerGoods |= (1<<0);
        	if (prtPG_Plt1n && prtPG_Plt1n && prtPG_Plt1n && prtPG_Plt1n) MemoryMap.var.fuentes.powerGoods &= ~(1<<1); else MemoryMap.var.fuentes.powerGoods |= (1<<1); //prtPG_Plt1n old
        	if (prtPG_Plt1n && prtPG_Plt1n && prtPG_Plt1n && prtPG_Plt1n) MemoryMap.var.fuentes.powerGoods &= ~(1<<2); else MemoryMap.var.fuentes.powerGoods |= (1<<2); //prtPG_Plt1n old

            //We return to the initial state
            fuentes_ON_OFF.u8Motor = 0;
            break;



        default:
            //We return to the initial state
            fuentes_ON_OFF.u8Motor = 0;
            break;

    }

}

/* adjustable font settings */
void appSPI_init(void) {

    uint8_t i;

    //We initialize with 0V to send to the SPI
    for (i = 0; i < MAX_NUM_SPI_CH; i++) {
        SPIst.SPI_CH[i].enviar[0] = 0b00110000;
        SPIst.SPI_CH[i].enviar[1] = 0b00000000;
        SPIst.SPI_CH[i].enviar[2] = 0b00000000;
    }

    //We initialize the SPI application engine
    SPIst.u8Motor = 0;

    //We initialize the channel to send
    SPIst.u8ActCH = 0;

    testNum = 0; //debug
}




void appSPI_motor(void) {
const uint8_t DAC_channel_num_wrapper [8] = {6, 7, 0, 1, 2, 3, 4, 5};

        for (i=0; i<MAX_NUM_SPI_CH; i++)
        {
                //Faltaaa
                //We load the protocol values ??to our SPI variable
                SPIst.SPI_CH[i].f_vPer = MemoryMap.var.fuentes.aVoltagePercent[i].fvalue;
                //Let's see if we have the module on
                //We calculate the value to send by the SPI

        		if (SPIst.SPI_CH[i].f_vPer <= 100) {
        			if (SPIst.SPI_CH[i].f_vPer >= 5) {
    					//Rule of three: vTrim = vPer*0x0FFF/100 = vPer*4095/100
                        //The source gives 13V instead of 12V when we program the maximum value,
                        //the new maximum is 3780 accounts instead of 4095. vTrim = vPer*3780/100
                        SPIst.SPI_CH[i].f_vTrim = SPIst.SPI_CH[i].f_vPer * 655.35;
                        SPIst.SPI_CH[i].u16vTrim = (uint16_t) SPIst.SPI_CH[i].f_vTrim;
                    } else {
    					//If they program below the minimum, we program 0 and turn off the source module
                        //if both peltier modules are below the minimum
                		SPIst.SPI_CH[i].u16vTrim = 0;
                    //Missing: Turn off the module and turn it on the first time we program if the standby is ok
        			}
        		} else {
                    //If they program above the maximum, we program the maximum
                		SPIst.SPI_CH[i].u16vTrim = 100;
        		}

        		if (frame.field.header.u8AddrSlave == 2) // of board 2, shift to another 4 dac channels
        			AD5676_Update_DAC_Channel_N(DAC_channel_num_wrapper[i + MAX_NUM_SPI_CH], SPIst.SPI_CH[i].u16vTrim);
        		else
        			AD5676_Update_DAC_Channel_N(DAC_channel_num_wrapper[i], SPIst.SPI_CH[i].u16vTrim);


                //We see if we have to turn on or turn off any module

        		if (frame.field.header.u8AddrSlave == 1)
        		{
					  if (SPIst.SPI_CH[i].u16vTrim == 0)
					  {
							//If the two peltier modules have been programmed below the minimum, we turn off the corresponding module
							if ((i == 0 || i == 1))//&& prtEN_Apl1n == 0) {
							{
								//We see that the other peltiers module also has 0 programmed
								if (SPIst.SPI_CH[0].u16vTrim == 0 && SPIst.SPI_CH[1].u16vTrim == 0)
								{
									//We turn off the module
									//prtEN_Apl1n = 0; //the logic of the enable signal has been changed
									prtEN_Apl1n(1); //Peltier module 1 OFF
									prtEN_Apl2n(1);
								}
							} else if ((i == 2 || i == 3))
							{
								if (SPIst.SPI_CH[2].u16vTrim == 0 && SPIst.SPI_CH[3].u16vTrim == 0)
								{
									//prtEN_Apl2n = 0;  //the logic of the enable signal has been changed
									prtEN_Apl3n(1); //Peltier module 2 OFF
									prtEN_Apl4n(1);
								}
							}
					} else if ((i == 0 || i == 1) && standby.fuentesOn.apl1 == 1) {
						//prtEN_Apl1n = 1;  //the logic of the enable signal has been changed
						prtEN_Apl1n(0); //Peltier module 1 ON
						prtEN_Apl2n(0);
					} else if ((i == 2 || i == 3) && standby.fuentesOn.apl2 == 1) {
						//prtEN_Apl2n = 1;  //the logic of the enable signal has been changed
						prtEN_Apl3n(0); //Peltier module 2 ON
						prtEN_Apl4n(0);
					}
        		} else if (frame.field.header.u8AddrSlave == 2)
        		{
					  if (SPIst.SPI_CH[i].u16vTrim == 0)
					  {
							//If the two peltier modules have been programmed below the minimum, we turn off the corresponding module
							if ((i == 0 || i == 1))//&& prtEN_Apl1n == 0) {
							{
								//We see that the other peltiers module also has 0 programmed
								if (SPIst.SPI_CH[0].u16vTrim == 0 && SPIst.SPI_CH[1].u16vTrim == 0)
								{
									//We turn off the module
									//prtEN_Apl1n = 0; //the logic of the enable signal has been changed
									prtEN_Apl5n(1); //Peltier module 1 OFF
									prtEN_Apl6n(1);
								}
							} else if ((i == 2 || i == 3))
							{
								if (SPIst.SPI_CH[2].u16vTrim == 0 && SPIst.SPI_CH[3].u16vTrim == 0)
								{
									//prtEN_Apl2n = 0;  //the logic of the enable signal has been changed
									prtEN_Apl7n(1); //Peltier module 2 OFF
									prtEN_Apl8n(1);
								}
							}
					} else if ((i == 0 || i == 1) && standby.fuentesOn.apl1 == 1) {
						//prtEN_Apl1n = 1;  //the logic of the enable signal has been changed
						prtEN_Apl5n(0); //Peltier module 1 ON
						prtEN_Apl6n(0);
					} else if ((i == 2 || i == 3) && standby.fuentesOn.apl2 == 1) {
						//prtEN_Apl2n = 1;  //the logic of the enable signal has been changed
						prtEN_Apl7n(0); //Peltier module 2 ON
						prtEN_Apl8n(0);
					}

        		} // end if (frame.field.header.u8AddrSlave == 2)


        } //end for

} // end appSPI_motor



