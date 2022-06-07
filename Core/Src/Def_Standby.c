/*
 * File:   Standby.c
 * Author: ingenieria9
 *
 * Created on 15 de marzo de 2018, 9:27
 */


#include "Def_Standby.h"

#define PULS_REFRESH_TIME 100
#define PULS_DEBOUNCE_TIME 20
#define MODULES_INIT_TIME 100
#define STANDBY_WARNING_PROTOCOL_TIME 100
#define STANDBY_WAIT_FOR_ON_TIME 100
#define STANDBY_WAIT_FOR_STANDBY_TIME 100

//#define ARRANQUE_SIN_BOTON_ALR      1

enum {
    LED_STANDBY_TURN_OFF = 0,
    LED_STANDBY_RED = 1,
    LED_STANDBY_GREEN = 2,
    //   MAX_NUM_LED
};

enum {
    LED_STANDBY_1 = 0,
    MAX_NUM_LED_STANDBY
};

typedef struct {
    uint8_t u8Motor;
    uint8_t u8Repeticions;
    uint8_t u8Fixe;
    uint8_t u8Color;
    uint8_t u8NovaConfig;
    uint16_t u16TempsON;
    uint16_t u16TempsOFF;
    uint32_t u32Timer;
} ledStandby_status_t;

typedef struct
{
    uint8_t u8Value;
    uint32_t u32Timer;
}switch_status_t;

ledStandby_status_t estatledStandBy[MAX_NUM_LED_STANDBY];

standby_t standby;

switch_status_t switch_status;

void ledStandByColor(uint8_t ledColor);

void LED_Standby_Init(void);

void LED_Standby_Motor(uint8_t u8Channel);

//Si ponemos 0xFF repeticiones, se repetira para siempre
void LED_Standby_Configure(uint8_t u8Channel, uint8_t u8Repeats, uint16_t u16TimeOff, uint16_t u16TimeOn, uint8_t u8Fixe, uint8_t u8Color);

void LED_Standby_CanviLED(uint8_t u8Channel, uint8_t u8Estat);

void SWITCH_Detect(void);

void standbyInit(void) {

    //Inicializamos motor
    standby.u8Motor = 0;
    //Inicializamos el estado del standby
    standby.state = PCB_STATE_OFF;

    //Variable de protocolo boardStatus OFF
    MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_OFF;

    //Inicializamos led standby
    LED_Standby_Init();
    
    switch_status.u8Value = 0;
    switch_status.u32Timer = TICKS_GetTicks();
    LED_Standby_Configure(LED_STANDBY_1, 0, 0, 2000, 1, LED_STANDBY_TURN_OFF);

}

uint8_t debStand;
uint8_t ac_fail;
uint8_t temp_fail;
uint8_t fan_ok;
uint8_t colorLEDAnterior;
void standbyMotor(void) {
    
    SWITCH_Detect();
//    if (identif.IdPCB == PCB2)
//    {
//
//        if(colorLEDAnterior != MemoryMap.var.fuentes.colorLED)
//        {
//            colorLEDAnterior = MemoryMap.var.fuentes.colorLED;
//            LED_Standby_Configure(LED_STANDBY_1, 0, 0, 2000, 1, colorLEDAnterior);
//        }
//    }
            
    switch (standby.u8Motor) {

        case 0:
            //We check that the source AC_fail interrupt port 1 is ready, the tempFail is 0 and the FAN_OK is 1
            //flgHabInt_Alarma = 0;
            //flgInt_Alarma = 0;
            ac_fail = prtInput1_AC_fail;
            temp_fail = prtInput1_TempFail;
            fan_ok = prtInput1_FanOK;
            if (prtInput1_AC_fail == 0 && prtInput1_TempFail == 0 && prtInput1_FanOK == 1) {
                //We activate the alarm engine flag
                fuentesStatus.alarmsOn = 1;
				//Initially all secondary modules off except 24V digital
                // We see what source we have connected
                if (identif.IdPCB == PCB1) {
                    //ON
#ifdef SOLO_2_FUENTES
                   prtEN_FA_DigitalN(1); //Modulo motores (bombas+fans)
#else
                    prtEN_FA_DigitalN(0); //Modulo 24V digitales
#endif
                    //OFF
                    prtEN_FA_MotorN(1); //Modulo motores (bombas+fans)
//                    prtEN_Apl1n = 1; //Aplicador 1 (Apl1Plt1 + Apl1Plt2)
//                    prtEN_Apl2n = 1; //Aplicador 2 (Apl2Plt1 + Apl2Plt2)
                    prtEN_Apl1n(0); //Peltier module 1 and 2 ON
                    prtEN_Apl2n(0);
                    prtEN_Apl3n(0);
                    prtEN_Apl4n(0);
                    prtEN_Apl5n(0);
                    prtEN_Apl6n(0);
                    prtEN_Apl7n(0);
                    prtEN_Apl8n(0);

                    //We update the value of the enable variables
#ifdef SOLO_2_FUENTES
                    standby.fuentesHab.digital = 0;
#else
                    standby.fuentesHab.digital = 1;
#endif
                    standby.fuentesHab.motor = 0;
                    standby.fuentesHab.apl1 = 0;
                    standby.fuentesHab.apl2 = 0;

                    // We update the value of the module variables on and off
#ifdef SOLO_2_FUENTES
                    standby.fuentesOn.digital = 0;

                    //We update the standby status
                    standby.state = PCB_STATE_STANDBY;

                    //We announce by protocol that we have turned on the digital 24V module (status: standby)
                    MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_STANDBY;
#else
                    standby.fuentesOn.digital = 1;

                    //Actualizamos el estado del standby
                    standby.state = PCB_STATE_STANDBY;

                    //Avisamos por protocolo de que hemos encendido el modulo de 24V digitales (estado: standby)
                    MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_STANDBY;
#endif
                    standby.fuentesOn.motor = 0;
                    standby.fuentesOn.apl1 = 0;
                    standby.fuentesOn.apl2 = 0;


                    //We enable AC_fail interrupt from source 1
                    //flgInt1_AC_fail = 0;
                    //flgHabInt1_AC_fail = 1;

                    // LED_Configure(LED1, 0, 2000, 0, 1); //debug: fixe off
                    //Inicialmente led rojo
                    //ledControl(LED_STANDBY_VERMELL);
                    /* INIT DISBLE RED LIGHT */
                    LED_Standby_Configure(LED_STANDBY_1, 0xFF, 2000, 100, 0, LED_STANDBY_GREEN);
                    /* END DISABLE RED LIGHT */
                    //COMENT FOR DISABLE RED RIGHT LED_Standby_Configure(LED_STANDBY_1, 0, 0, 2000, 1, LED_STANDBY_VERMELL);
                    //Avanzamos al siguiente estado
                standby.u8Motor++;

                } else if (identif.IdPCB == PCB2) {
                    //OFF
#ifdef SOLO_2_FUENTES
                    prtEN_FA_MotorN(0); //es la fuente digital
#else
                    prtEN_FA_MotorN(1); //Modulo motores (EV+manipulos)
#endif
//                    prtEN_Apl1n = 1; //Aplicador 3 (Apl3Plt1 + Apl3Plt2)
//                    prtEN_Apl2n = 1; //Aplicador 4 (Apl4Plt1 + Apl4Plt2)
                    prtEN_Apl1n(0); //Peltier module 1 and 2 ON
                    prtEN_Apl2n(0);
                    prtEN_Apl3n(0);
                    prtEN_Apl4n(0);
                    prtEN_Apl5n(0);
                    prtEN_Apl6n(0);
                    prtEN_Apl7n(0);
                    prtEN_Apl8n(0);

                    //We update the value of the enable variables
                    standby.fuentesHab.digital = 0;
#ifdef SOLO_2_FUENTES
                    standby.fuentesHab.motor = 1;
                    MemoryMap.var.fuentes.aModulos_ON_OFF[FNT_MOD_FIXED2] = 1;
#else
                    standby.fuentesHab.motor = 0;
#endif
                    standby.fuentesHab.apl1 = 0;
                    standby.fuentesHab.apl2 = 0;

                    //We update the value of the module variables on and off
                    standby.fuentesOn.digital = 0;
#ifdef SOLO_2_FUENTES
                    standby.fuentesOn.motor = 1;
                    //We updated the standby status for the pcb2 is directly wait for on
                    standby.state = PCB_STATE_WAIT_FOR_ON;

                    //We report by protocol the status of the standby (status: wait for on)
                    MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_WAIT_FOR_ON;
#else
                    standby.fuentesOn.motor = 0;//Actualizamos el estado del standby para la pcb2 es directamente wait for on
                    
                    standby.state = PCB_STATE_WAIT_FOR_ON;

                    //We report by protocol the status of the standby (status: wait for on)
                    MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_WAIT_FOR_ON;
#endif
                    standby.fuentesOn.apl1 = 0;
                    standby.fuentesOn.apl2 = 0;

                    
					//We jump to the waiting case for the main to be ready to go ON
                    //(since this pcb does not have the standby button connected)
                    standby.u8Motor = 5;

                } else {
                    //Otherwise it is none of the known pcbs we do not advance of state
                    standby.u8Motor = 0;
                    break;
                }


                //We capture the refresh time of the pushbutton on PCB1 and the wait for standby of pcb2
                standby.u32Time = TICKS_GetTicks();
                
            }
            break;

        case 1:
            //We are waiting for the button refresh time
#ifdef ARRANQUE_SIN_BOTON_ALR
            if (TICKS_DiffTicks(standby.u32Time) > 2000)
            {
                standby.u8Motor = 4;
            }
#else
            if (MemoryMap.var.fuentes.mainStatus == MAIN_STATE_READY_FOR_ON)
            {
                standby.u32Time = TICKS_GetTicks();
                standby.u8Motor = 6;    /* if they tell me to turn it on, I'll turn it on directly without looking at the button */
            }
            else if (TICKS_DiffTicks(standby.u32Time) > PULS_REFRESH_TIME) 
            {
                standby.u8Motor++;
            }
#endif
            break;

        case 2:
            //Let's see if they're pushing the button
            if (MemoryMap.var.cabecera_fw.board_status.bit_field.switchState == 0) {
                //We update the time and move on to the next state
                standby.u32Time = TICKS_GetTicks();
                standby.u8Motor++;
            } else {
                //We update the time and return to the refresh time status
                standby.u32Time = TICKS_GetTicks();
                standby.u8Motor = 1;
            }
            break;

        case 3:
            //We wait 20ms for the value of the pushbutton to be stable, in case it is a bounce or a noise do not change the state of the pushbutton
            if (TICKS_DiffTicks(standby.u32Time) > PULS_DEBOUNCE_TIME) {
                //We move on to the next state
                standby.u8Motor++;
            }
            break;


        case 4:
			// Case OFF-ON
            // Check if they are still pressing the button
#ifdef ARRANQUE_SIN_BOTON_ALR
            //faltaaa: debug
			// LED_Configure (LED1, 0xFF, 500, 500, 0); // debug: fix on
            // We change the led to flashing green
            // ledControl (LED_STANDBY_GREEN);
            LED_Standby_Configure(LED_STANDBY_1, 0xFF, 500, 500, 0, LED_STANDBY_GREEN);

			// We update the standby status
            standby.state = PCB_STATE_WAIT_FOR_ON;

			// We notify by protocol that the user has pressed the standby button to turn on the machine (status: wait for on)
            MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_WAIT_FOR_ON;
            
			// We capture the standby refresh time of the modules
            standby.u32Time = TICKS_GetTicks();

            //We move on to the next state
            standby.u8Motor++;
#else
            if (MemoryMap.var.cabecera_fw.board_status.bit_field.switchState == 0) {
                //faltaaa: debug
                // LED_Configure(LED1, 0xFF, 500, 500, 0); //debug: fixe on
                //We change the LED to flashing green
                // ledControl(LED_STANDBY_GREEN);
                LED_Standby_Configure(LED_STANDBY_1, 0xFF, 500, 500, 0, LED_STANDBY_GREEN);

                //We update the standby status
                standby.state = PCB_STATE_WAIT_FOR_ON;

                //We notify by protocol that the user has pressed the standby button to turn on the machine (status: wait for on)
                MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_WAIT_FOR_ON;

                //We capture the standby refresh time of the modules
                standby.u32Time = TICKS_GetTicks();

                //We move on to the next state
                standby.u8Motor++;
            } else {
                //We return to the initial state
                standby.u8Motor = 0;
            }
#endif

            break;


        case 5:
            //We wait for the pcbs to have done all the standby tasks
            //We read by protocol when the plates have done all their tasks
            //missing: think if it is necessary to put standby
            if (TICKS_DiffTicks(standby.u32Time) > STANDBY_WAIT_FOR_ON_TIME) {
#ifdef ARRANQUE_SIN_BOTON_ALR
                MemoryMap.var.fuentes.mainStatus = MAIN_STATE_READY_FOR_ON;
#endif
                if (MemoryMap.var.fuentes.mainStatus == MAIN_STATE_READY_FOR_ON) 
                {
                    //We move on to the next state
                    standby.u8Motor++;
                } else {
                    //We capture the standby refresh time of the modules again
                    standby.u32Time = TICKS_GetTicks();
                }

            }

            break;


        case 6:
            //We turn on all the secondary modules except the 24V digital ones which are already on
            //Let's see which source we have connected
            
            //flgHabInt_Alarma = 1;
            //flgInt_Alarma = 0;
            //flgInt1_Temp_fail = 0;
            //flgHabInt1_Temp_fail = 1;
            if (identif.IdPCB == PCB1) {
                // prtEN_FA_DigitalN = 0; //Modulo 24V digitales
                //ON
                //  prtEN_FA_MotorN = 0; //Modulo motores (bombas+fans)
                //   prtEN_Apl1n = 0; //Aplicador 1 (Apl1Plt1 + Apl1Plt2)
                //   prtEN_Apl2n = 0; //Aplicador 2 (Apl2Plt1 + Apl2Plt2)
                standby.fuentesHab.motor = 1;
                standby.fuentesHab.apl1 = 1;
                standby.fuentesHab.apl2 = 1;

                //debug
                //  standby.fuentesOn.digital = 0;
                // standby.fuentesOn.motor = 1;
                // standby.fuentesOn.apl1 = 1;
                // standby.fuentesOn.apl2 = 1;


                //We update the standby status
                standby.state = PCB_STATE_ON;

                //We announce by protocol that we have turned on the modules
                MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_ON;


                //faltaaa: debug
                //LED_Configure(LED1, 0, 0, 2000, 1); //debug: fixe on
                //We change the led to green
                // ledControl(LED_STANDBY_GREEN);
                LED_Standby_Configure(LED_STANDBY_1, 0, 0, 2000, 1, LED_STANDBY_GREEN);
                //We capture the refresh time of initialization of the modules
                standby.u32Time = TICKS_GetTicks();
                //We move on to the next state
                standby.u8Motor++;
                //            } else {
                //                //Volvemos al estado inicial
                //                standby.u8Motor = 0;
                //            }

            } else if (identif.IdPCB == PCB2) {
                //ON
                //    prtEN_FA_MotorN = 0; //Modulo motores (EV+manipulos)
                //     prtEN_Apl1n = 0; //Aplicador 3 (Apl3Plt1 + Apl3Plt2)
                //     prtEN_Apl2n = 0; //Aplicador 4 (Apl4Plt1 + Apl4Plt2)
                standby.fuentesHab.motor = 1;
                standby.fuentesHab.apl1 = 1;
                standby.fuentesHab.apl2 = 1;

                //debug
                //  standby.fuentesOn.digital = 0;
                //  standby.fuentesOn.motor = 1;
                //  standby.fuentesOn.apl1 = 1;
                //  standby.fuentesOn.apl2 = 1;

                //We update the standby status
                standby.state = PCB_STATE_ON;

                //We announce by protocol that we have turned on the modules
                MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_ON;

                //Habilitamos interrupci�n de AC_fail de la fuente 1
                //flgInt1_AC_fail = 0;
                //flgHabInt1_AC_fail = 1;


                //We update the standby status for pcb2 is directly wait for standby
                standby.state = PCB_STATE_WAIT_FOR_STANDBY;

                //We alert by standby status protocol (status: wait for standby)
                MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_WAIT_FOR_STANDBY;

                //faltaaa: debug
                //LED_Configure(LED1, 0, 0, 2000, 1); //debug: fixe on
                //Cambiamos el led a verde
                // ledControl(LED_STANDBY_GREEN);
                // LED_Standby_Configure(LED_STANDBY_1, 0, 0, 2000, 1, LED_STANDBY_GREEN);

                //We capture the refresh time waiting for the main standby
                standby.u32Time = TICKS_GetTicks();
                //Let's jump to the wait for standby case
                standby.u8Motor = 15;
                //            } else {
                //                //Volvemos al estado inicial
                //                standby.u8Motor = 0;
                //            }

            } else {
                //Otherwise it is none of the known pcbs we return to the initial state
                standby.u8Motor = 0;
            }
            break;
        case 7:
            //We are waiting for the initialization time of the modules
            if (TICKS_DiffTicks(standby.u32Time) > MODULES_INIT_TIME) {
                //Avanzamos al siguiente estado
                standby.u8Motor++;
            }
            break;

        case 8:
            //We check if they have released the button
            if (MemoryMap.var.cabecera_fw.board_status.bit_field.switchState == 1) {
                //We update the time and move on to the next state
                standby.u32Time = TICKS_GetTicks();
                standby.u8Motor++;
            } else {
				// We update the time and return to the module initialization timeout state
                // (missing: If you change this initialization time of the modules, watch because it will be the refresh time to release the button,
                // if necessary add any case)
                standby.u32Time = TICKS_GetTicks();
                standby.u8Motor = 7;
            }
            break;

        case 9:
            //We wait 20ms for the value of the pushbutton to be stable, in case it is a bounce or a noise do not change the state of the pushbutton
            if (TICKS_DiffTicks(standby.u32Time) > PULS_DEBOUNCE_TIME) {
                //We move on to the next state
                standby.u8Motor++;
            }
            break;

        case 10:
            //We check if they are still releasing the button
            if (MemoryMap.var.cabecera_fw.board_status.bit_field.switchState == 1) {
                //We capture the refresh time of initialization of the modules
                standby.u32Time = TICKS_GetTicks();
                //We move on to the next state
                standby.u8Motor++;
            } else {
                //We return to the initialization timeout state of the modules
                standby.u8Motor = 7;
            }
            break;

        case 11:
            //We are waiting for the button refresh time
            if (TICKS_DiffTicks(standby.u32Time) > PULS_REFRESH_TIME) {
                standby.u8Motor++;
            }
            break;

        case 12:
            //Let's see if they're pushing the button
            if (MemoryMap.var.cabecera_fw.board_status.bit_field.switchState == 0) {
                //We update the time and move on to the next state
                standby.u32Time = TICKS_GetTicks();
                standby.u8Motor++;
            } else {
                //We update the time and return to the refresh time status
                standby.u32Time = TICKS_GetTicks();
                standby.u8Motor = 11;
            }
            break;

        case 13:
            //We wait 20ms for the value of the pushbutton to be stable, in case it is a bounce or a noise do not change the state of the pushbutton
            if (TICKS_DiffTicks(standby.u32Time) > PULS_DEBOUNCE_TIME) {
                //We move on to the next state
                standby.u8Motor++;
            }
            break;

        case 14:
            //Caso ON-OFF
            //We check if they are still pressing the button
            if (MemoryMap.var.cabecera_fw.board_status.bit_field.switchState == 0) {
                //We update the standby status
                standby.state = PCB_STATE_WAIT_FOR_STANDBY;

                //We warn by protocol that we are going to switch to standby
                MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_WAIT_FOR_STANDBY;

                //LED_Configure(LED1, 0xFF, 500, 500, 0); //debug: intermitencies
                //Cambiamos el led a rojo intermitente
                // ledControl(LED_STANDBY_GREEN);
                /* INIT DISABLE RED LIGHT */
                LED_Standby_Configure(LED_STANDBY_1, 0xFF, 500, 500, 0, LED_STANDBY_GREEN);
                /* END DISABLE RED LIGHT */
                //COMMENT FOR DISABLE RED LIGHT LED_Standby_Configure(LED_STANDBY_1, 0xFF, 500, 500, 0, LED_STANDBY_VERMELL);
                //We capture the refresh time of initialization of the modules
                standby.u32Time = TICKS_GetTicks();
                //Avanzamos al siguiente estado
                standby.u8Motor++;
            } else {
                //We return to the pulsed standby state
                standby.u8Motor = 11;
            }
            break;

        case 15:
			//We wait until the pcbs have performed all the standby tasks
            //We read by protocol when the boards have done all their tasks
            if (TICKS_DiffTicks(standby.u32Time) > STANDBY_WAIT_FOR_STANDBY_TIME) {

                if (MemoryMap.var.fuentes.mainStatus == MAIN_STATE_READY_FOR_STANDBY) {
                    //Avanzamos al siguiente estado
                    standby.u8Motor++;
                } else {
                    //We capture the standby refresh time of the modules again
                    standby.u32Time = TICKS_GetTicks();
                }


            }

            break;


            //        case 16:
            //          //  //Comprobamos si aun estan pulsando el pulsador
            //          //  if (prtOnOffn == 0) {
            //                //Faltaaaa: Aviso por el protocolo que van a pasar a standby
            //
            //
            //                //Capturamos el tiempo de refresco de espera de inicializacion de los modulos
            //                standby.u32Time = TICKS_GetTicks();
            //                //Avanzamos al siguiente estado
            //                standby.u8Motor++;
            ////            } else {
            ////                //Volvemos al estado de espera de pulsado
            ////                standby.u8Motor = 13;
            ////            }
            //            break;
            //
            //        case 17:
            //            //Esperamos el tiempo de aviso antes de pasar a standby
            //            if (TICKS_DiffTicks(standby.u32Time) > STANDBY_WARNING_PROTOCOL_TIME) {
            //                //Avanzamos al siguiente estado
            //                standby.u8Motor++;
            //            }
            //            //faltaaa: esperamos un tiempo las respuestas de las otras placas?
            //
            //            break;

        case 16:
            //We turn off all the secondary modules except the 24V digital ones
            //Let's see what source we have connected
            if (identif.IdPCB == PCB1) {
#ifdef SOLO_2_FUENTES
                //OFF
                prtEN_FA_DigitalN(1); //Modulo 24V sin conexion
#else
                //ON
                prtEN_FA_DigitalN(0); //Modulo 24V digitales
#endif
                //OFF
                prtEN_FA_MotorN(1); //Modulo motores (bombas+fans)
//                prtEN_Apl1n = 1; //Aplicador 1 (Apl1Plt1 + Apl1Plt2)
//                prtEN_Apl2n = 1; //Aplicador 2 (Apl2Plt1 + Apl2Plt2)
                prtEN_Apl1n(0); //Peltier module 1 and 2 ON
                prtEN_Apl2n(0);
                prtEN_Apl3n(0);
                prtEN_Apl4n(0);
                prtEN_Apl5n(0);
                prtEN_Apl6n(0);
                prtEN_Apl7n(0);
                prtEN_Apl8n(0);

                //We update the value of the enable variables
#ifdef SOLO_2_FUENTES
                standby.fuentesHab.digital = 0;
#endif
                standby.fuentesHab.motor = 0;
                standby.fuentesHab.apl1 = 0;
                standby.fuentesHab.apl2 = 0;

                //We update the value of the module variables on and off
#ifdef SOLO_2_FUENTES
                standby.fuentesOn.digital = 0;
#endif
                standby.fuentesOn.motor = 0;
                standby.fuentesOn.apl1 = 0;
                standby.fuentesOn.apl2 = 0;

                //We update the standby status
                standby.state = PCB_STATE_STANDBY;

                //We notify you by protocol that we have turned off all modules except the 24V digital (status: standby)
                MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_STANDBY;

                //LED_Configure(LED1, 0, 2000, 0, 1); //debug: fixe off
                //Ponemos el led rojo
                //ledControl(LED_STANDBY_VERMELL);
                /* INIT DISABLE RED LIGHT */
                LED_Standby_Configure(LED_STANDBY_1, 0xFF, 2000, 100, 0, LED_STANDBY_GREEN);
                /* END DISABLE RED LIGHT */
                //COMENT FOR DISABLE RED LIGHT LED_Standby_Configure(LED_STANDBY_1, 0, 0, 2000, 1, LED_STANDBY_VERMELL);
                //We capture the refresh time of the button
                standby.u32Time = TICKS_GetTicks();
                //We move on to the next state
                standby.u8Motor++;
                break;


            } else if (identif.IdPCB == PCB2) {
#ifdef SOLO_2_FUENTES
                //ON
                prtEN_FA_MotorN(0); //Modulo 24V digitales
#else
                //OFF
                prtEN_FA_MotorN(1); //Modulo motores (EV+manipulos)
#endif
//                prtEN_Apl1n = 1; //Aplicador 3 (Apl3Plt1 + Apl3Plt2)
//                prtEN_Apl2n = 1; //Aplicador 4 (Apl4Plt1 + Apl4Plt2)
                prtEN_Apl1n(0); //Peltier module 1 and 2 ON
                prtEN_Apl2n(0);
                prtEN_Apl3n(0);
                prtEN_Apl4n(0);
                prtEN_Apl5n(0);
                prtEN_Apl6n(0);
                prtEN_Apl7n(0);
                prtEN_Apl8n(0);

                //We update the value of the enable variables
#ifdef SOLO_2_FUENTES
                standby.fuentesHab.motor = 1;
#else
                standby.fuentesHab.motor = 0;
#endif
                standby.fuentesHab.apl1 = 0;
                standby.fuentesHab.apl2 = 0;

                //We update the value of the module variables on and off
#ifdef SOLO_2_FUENTES
                standby.fuentesOn.motor = 1;
#else
                standby.fuentesOn.motor = 0;
#endif
                standby.fuentesOn.apl1 = 0;
                standby.fuentesOn.apl2 = 0;


                //We updated the standby status for the pcb2 is directly wait for on
                standby.state = PCB_STATE_WAIT_FOR_ON;

                //We report the status of the standby by protocol (estado: wait for on)
                MemoryMap.var.cabecera_fw.board_status.status = PCB_STATE_WAIT_FOR_ON;

                //LED_Configure(LED1, 0, 2000, 0, 1); //debug: fixe off
                //Ponemos el led rojo
                //ledControl(LED_STANDBY_VERMELL);
                // LED_Standby_Configure(LED_STANDBY_1, 0, 0, 2000, 1, LED_STANDBY_VERMELL);
                //Capturamos el tiempo de refresco del pulsador
                //standby.u32Time = TICKS_GetTicks();

                //We move on to the next state
                standby.u8Motor = 0;
                break;

            } else {
                //Otherwise it is none of the known pcbs we return to the initial state
                standby.u8Motor = 0;
                break;
            }

        case 17:
            //We are waiting for the button refresh time
            if (TICKS_DiffTicks(standby.u32Time) > PULS_REFRESH_TIME) {
                standby.u8Motor++;
            }
            break;

        case 18:
            //We check if they have released the button
            if (MemoryMap.var.cabecera_fw.board_status.bit_field.switchState == 1) {
                //We update the time and move on to the next state
                standby.u32Time = TICKS_GetTicks();
                standby.u8Motor++;
            } else {
                //We update the time and return to the module initialization timeout state
                //missing: If this module initialization time is changed, watch out for the refresh time when the button is released,
                // if necessary add any case
                standby.u32Time = TICKS_GetTicks();
                standby.u8Motor = 17;
            }
            break;

        case 19:
            //We wait 20ms for the value of the pushbutton to be stable, in case it is a bounce or a noise do not change the state of the pushbutton
            if (TICKS_DiffTicks(standby.u32Time) > PULS_DEBOUNCE_TIME) {
                //Avanzamos al siguiente estado
                standby.u8Motor++;
                
                
      
                
            }
            break;

        case 20:
            //We check if they are still releasing the button
            if (MemoryMap.var.cabecera_fw.board_status.bit_field.switchState == 1) {
                //We return to the initial state
                standby.u8Motor = 0;
            } else {
                //We return to the initialization timeout state of the modules
                standby.u8Motor = 17;
            }
            break;

        default:
            /*By default we return to the initial state*/
            standby.u8Motor = 0;
            break;
    }

    //if (identif.IdPCB == PCB1)
    {
    //We call the engine the standBy led
    LED_Standby_Motor(LED_STANDBY_1);
    }
}

void LED_Standby_Init(void) {
    uint8_t i;

    // LED1_OFF;
    // LED2_OFF;
    //Actualizamos mapa de memoria
    //MemoryMap.var.cabecera_fw.board_status.bit_field.ledState = prtDL1;
    for (i = 0; i < MAX_NUM_LED_STANDBY; i++) {
        estatledStandBy[i].u8Motor = 0;
        estatledStandBy[i].u8Repeticions = 0;
        estatledStandBy[i].u8Fixe = 0;
        estatledStandBy[i].u8Color = 0;
        estatledStandBy[i].u8NovaConfig = 0;
        estatledStandBy[i].u16TempsOFF = 0;
        estatledStandBy[i].u16TempsON = 0;
    }
}

void LED_Standby_Motor(uint8_t u8Channel) {
    switch (estatledStandBy[u8Channel].u8Motor) {
        case 0:
            //If it is a new fixed LED configuration we change it
            if (estatledStandBy[u8Channel].u8Fixe == 1 && estatledStandBy[u8Channel].u8NovaConfig == 1) {
                if (estatledStandBy[u8Channel].u16TempsON > 0 && estatledStandBy[u8Channel].u16TempsOFF == 0) {
                    LED_Standby_CanviLED(u8Channel, estatledStandBy[u8Channel].u8Color);

                } else if (estatledStandBy[u8Channel].u16TempsOFF > 0 && estatledStandBy[u8Channel].u16TempsON == 0) {
                    LED_Standby_CanviLED(u8Channel, LED_STANDBY_TURN_OFF);
                }
                estatledStandBy[u8Channel].u8NovaConfig = 0;

                //If it is a repeat configuration, we start by turning on the LED
            } else if (estatledStandBy[u8Channel].u8Fixe == 0 && estatledStandBy[u8Channel].u8Repeticions > 0) {
                estatledStandBy[u8Channel].u32Timer = TICKS_GetTicks();
                LED_Standby_CanviLED(u8Channel, estatledStandBy[u8Channel].u8Color);
                //Actualizamos mapa de memoria
                //MemoryMap.var.cabecera_fw.board_status.bit_field.ledState = prtDL1;
                estatledStandBy[u8Channel].u8Motor++;
            }
            break;
        case 1:
            if (TICKS_DiffTicks(estatledStandBy[u8Channel].u32Timer) > estatledStandBy[u8Channel].u16TempsON) {
                estatledStandBy[u8Channel].u32Timer = TICKS_GetTicks();
                LED_Standby_CanviLED(u8Channel, LED_STANDBY_TURN_OFF);
                //Actualizamos mapa de memoria
                //MemoryMap.var.cabecera_fw.board_status.bit_field.ledState = prtDL1;
                estatledStandBy[u8Channel].u8Motor++;
            }
            break;
        case 2:
            if (TICKS_DiffTicks(estatledStandBy[u8Channel].u32Timer) > estatledStandBy[u8Channel].u16TempsOFF) {
                //If we put 0xFF repeats, it will be repeated forever
                if (estatledStandBy[u8Channel].u8Repeticions != 0xFF) {
                    estatledStandBy[u8Channel].u8Repeticions--;
                }
                estatledStandBy[u8Channel].u8Motor = 0;
            }
            break;
        default:
            estatledStandBy[u8Channel].u8Motor = 0;
            break;
    }
}

void LED_Standby_Configure(uint8_t u8Channel, uint8_t u8Repeats, uint16_t u16TimeOff, uint16_t u16TimeOn, uint8_t u8Fixe, uint8_t u8Color) {
    estatledStandBy[u8Channel].u8Repeticions = u8Repeats;
    estatledStandBy[u8Channel].u8Fixe = u8Fixe;
    estatledStandBy[u8Channel].u8Color = u8Color;
    estatledStandBy[u8Channel].u16TempsOFF = u16TimeOff;
    estatledStandBy[u8Channel].u16TempsON = u16TimeOn;
    estatledStandBy[u8Channel].u8NovaConfig = 1;
}

void LED_Standby_CanviLED(uint8_t u8Channel, uint8_t u8Estat) {
    if (u8Channel == LED_STANDBY_1) {
        //prtDL1 = u8Estat;
        ledStandByColor(u8Estat);
    }
    //    else if (u8Channel == LED2) {
    //        prtDL2 = u8Estat;
    //    }
    // else { LED_G_OUT = u8Estat; }
}

void ledStandByColor(uint8_t ledColor) {

    if (ledColor == LED_STANDBY_RED) {
        /*First I turn off transients and then I turn them on*/
        prtGreenLED(0);
        prtRedLED(1);
    } else if (ledColor == LED_STANDBY_GREEN) {
        /*First I turn off transients and then I turn them on*/
        prtGreenLED(1);
        prtRedLED(0);
    } else {
        /*Led switched off*/
        prtGreenLED(0);
        prtRedLED(0);
    }

}

void SWITCH_Detect(void)
{
    if(switch_status.u8Value != prtOnOffn)
    {
        switch_status.u8Value = prtOnOffn;
        switch_status.u32Timer = TICKS_GetTicks();
    }
    else if(TICKS_DiffTicks(switch_status.u32Timer) > PULS_DEBOUNCE_TIME)
    {
        MemoryMap.var.cabecera_fw.board_status.bit_field.switchState = switch_status.u8Value;
        switch_status.u32Timer = TICKS_GetTicks();
    }
}


//uint8_t ledColorPruebas;
//
//void pruebasInit(void) {
//    //Capturamos el tiempo de refresco del pulsador
//    standby.u32Time = TICKS_GetTicks();
//    ledColorPruebas = 0;
//}
//
//void pruebasMotor(void) {
//
//    if (TICKS_DiffTicks(standby.u32Time) > 2000) {
//
//        ledControl(ledColorPruebas);
//
//        if (ledColorPruebas == 0) {
//            ledColorPruebas = 1;
//        } else if (ledColorPruebas == 1) {
//            ledColorPruebas = 2;
//        } else if (ledColorPruebas == 2) {
//            ledColorPruebas = 0;
//        }
//
//        standby.u32Time = TICKS_GetTicks();
//    }
//
//
//}

