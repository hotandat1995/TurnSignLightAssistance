/**
 * {
 * \file       DataControl.cpp
 * \brief      The controller for data come form peripheral
 * \copyright  (C) 2022 Cerberus team
 *             The reproduction, distribution and utilization of this file as
 *             well as the communication of its contents to others without express
 *             authorization is prohibited. Offenders will be held liable for the
 *             payment of damages. All rights reserved in the event of the grant
 *             of a patent, utility model or design.
 * }
 */

/***********************************************************************************************************************
 **                                                      INCLUDES                                                     **
 **********************************************************************************************************************/

#include "MainState.h"
#include "StateMachine.h"

/***********************************************************************************************************************
 **                                                   DEFINES/MACROS                                                  **
 **********************************************************************************************************************/


/***********************************************************************************************************************
 **                                           INTERNAL FUNCTION DECLARATIONS                                          **
 **********************************************************************************************************************/

/* List of state */
static void state_Init(void);
static void state_NormalOff(void);
static void state_BlinkLeft(void);
static void state_BlinkRight(void);
static void state_TemporaryOff(void);

/* List of transistion */


/***********************************************************************************************************************
 **                                                 INTERNAL VARIABLES                                                **
 **********************************************************************************************************************/

StateMachine machine = StateMachine();

State* S_Init = machine.addState(&state_Init);
State* S_NormalOff = machine.addState(&state_NormalOff);
State* S_BlinkLeft = machine.addState(&state_BlinkLeft);
State* S_BlinkRight = machine.addState(&state_BlinkRight);
State* S_TemporaryOff = machine.addState(&state_TemporaryOff);

/***********************************************************************************************************************
 **                                                FUNCTION DEFINITIONS                                               **
 **********************************************************************************************************************/



