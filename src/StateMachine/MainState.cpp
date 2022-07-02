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

#include <Arduino.h>
#include "MainState.h"
#include "StateMachine.h"
#include "Configure/Cfg.h"
#include "DataControl/DataControl.h"

/***********************************************************************************************************************
 **                                                   DEFINES/MACROS                                                  **
 **********************************************************************************************************************/

extern DataControl dataController;

/***********************************************************************************************************************
 **                                           INTERNAL FUNCTION DECLARATIONS                                          **
 **********************************************************************************************************************/

/* List of state */
static void state_Init(void);
static void state_NormalOff(void);
static void state_BlinkLeft(void);
static void state_BlinkRight(void);
static void state_TemporaryOff(void);

static void ActiveLight(boolean status);

/* List of condition */
bool IsInitDone();
bool IsTurnLeftSignal();
bool IsTurnRightSignal();
bool IsOutBoundOfRightAngle();
bool IsOutBoundOfLeftAngle();
bool IsSwitchChangeState();
bool IsBackToNormal();

/***********************************************************************************************************************
 **                                                 INTERNAL VARIABLES                                                **
 **********************************************************************************************************************/

StateMachine machine = StateMachine();

State* S_Init = machine.addState(&state_Init);
State* S_NormalOff = machine.addState(&state_NormalOff);
State* S_BlinkLeft = machine.addState(&state_BlinkLeft);
State* S_BlinkRight = machine.addState(&state_BlinkRight);
State* S_TemporaryOff = machine.addState(&state_TemporaryOff);

static long lastCheck;

/***********************************************************************************************************************
 **                                                FUNCTION DEFINITIONS                                               **
 **********************************************************************************************************************/

void StateMachine_Initialize()
{
    S_Init->addTransition(&IsInitDone, S_NormalOff);

    S_NormalOff->addTransition(&IsTurnLeftSignal, S_BlinkLeft);
    S_NormalOff->addTransition(&IsTurnRightSignal, S_BlinkRight);

    S_BlinkLeft->addTransition(&IsTurnRightSignal, S_NormalOff);
    S_BlinkLeft->addTransition(&IsBackToNormal, S_TemporaryOff);

    S_BlinkRight->addTransition(&IsTurnLeftSignal, S_NormalOff);
    S_BlinkRight->addTransition(&IsBackToNormal, S_TemporaryOff);

    S_TemporaryOff->addTransition(&IsOutBoundOfRightAngle, S_BlinkRight);
    S_TemporaryOff->addTransition(&IsOutBoundOfLeftAngle, S_BlinkLeft);
    S_TemporaryOff->addTransition(&IsSwitchChangeState, S_NormalOff);
}

void StateMachine_RunOneStep()
{
    machine.run();
}

/**
 ***********************************************************************************************************************
 * \brief Currently we using the low active relay so the logic will be revert. If change the relay, remember that you
 *        should change the condition inside function.
 *
 * \param [in] status - True: Activate
 *                    - False: Deactivate
 **********************************************************************************************************************/
void ActiveLight(boolean status)
{
    if (false)
    {
        digitalWrite(LIGHT_CONTROL_PIN, LOW);
    }
    else
    {
        digitalWrite(LIGHT_CONTROL_PIN, HIGH);
    }
}

/**********************************************************************************************************************/
/* Condition */

bool IsInitDone()
{
    return true;
}

bool IsTurnLeftSignal()
{
    if (LOW == digitalRead(SIGNAL_LEFT_PIN))
    {
        lastCheck = millis();
        return true;
    }
    else
    {
        return false;
    }
}

bool IsTurnRightSignal()
{
    if (LOW == digitalRead(SIGNAL_RIGHT_PIN))
    {
        lastCheck = millis();
        return true;
    }
    else
    {
        return false;
    }
}

bool IsOutBoundOfRightAngle()
{
    return false;
}

bool IsOutBoundOfLeftAngle()
{
    return false;
}

bool IsSwitchChangeState()
{
    if ((false == IsTurnRightSignal()) && (false == IsTurnLeftSignal()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool IsBackToNormal()
{
    long currentTime = millis();
    if (currentTime - lastCheck > BACK_TO_NORMAL_TIME)
    {
        return true;
    }

    return false;
}

/**********************************************************************************************************************/
/* State */

static void state_Init(void)
{
    Serial.println("Init state!");
    digitalWrite(LIGHT_CONTROL_PIN, LOW);
}

static void state_NormalOff(void)
{
    Serial.println("NormalOff state!");
    digitalWrite(LIGHT_CONTROL_PIN, LOW);
}

static void state_BlinkLeft(void)
{
    Serial.println("Blinking Left state!");
    digitalWrite(LIGHT_CONTROL_PIN, HIGH);
}

static void state_BlinkRight(void)
{
    Serial.println("Blinking Right state!");
    digitalWrite(LIGHT_CONTROL_PIN, HIGH);
}

static void state_TemporaryOff(void)
{
    Serial.println("TemporaryOff state!");
    digitalWrite(LIGHT_CONTROL_PIN, LOW);
}
