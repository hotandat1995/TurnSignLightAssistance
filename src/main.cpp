/**
 * {
 * \file       main.cpp
 * \brief      Main functionality of board
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

#include <Arduino.h>
#include "Configure/Cfg.h"
#include "DataControl/DataControl.h"
#include "StateMachine/MainState.h"

/***********************************************************************************************************************
 **                                                   DEFINES/MACROS                                                  **
 **********************************************************************************************************************/

/***********************************************************************************************************************
 **                                                 INTERNAL VARIABLES                                                **
 **********************************************************************************************************************/

long timer = 0;
long alive_led_timer = 0;

extern DataControl dataController;

/***********************************************************************************************************************
 **                                           INTERNAL FUNCTION DECLARATIONS                                          **
 **********************************************************************************************************************/

static void ToggleAliveLed();

/***********************************************************************************************************************
 **                                                FUNCTION DEFINITIONS                                               **
 **********************************************************************************************************************/

/**
 ***********************************************************************************************************************
 * \brief Setup step
 **********************************************************************************************************************/
void setup()
{
    dataController.InitPeripheral();
    StateMachine_Initialize();
}

/**
 ***********************************************************************************************************************
 * \brief Main loop.
 * - Checking to running the state machine
 * - Toggle the Alive led to inform state of system
 **********************************************************************************************************************/
void loop()
{
    if (millis() - timer > DELAY_TIME)
    {
        dataController.UpdateAndProcessData();
        StateMachine_RunOneStep();
        timer = millis();
    }

    if (millis() - alive_led_timer > ALIVE_LED_TIME)
    {
        ToggleAliveLed();
        alive_led_timer = millis();
    }
}

/**
 ***********************************************************************************************************************
 * \brief Toggle the hardware pin to inform state of system to user
 **********************************************************************************************************************/
static void ToggleAliveLed()
{
    digitalWrite(ALIVE_LED_PIN, !digitalRead(ALIVE_LED_PIN));
}

/**********************************************************************************************************************/
