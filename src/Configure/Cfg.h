/**
 * {
 * \file       Cfg.h
 * \brief      Main functionality of board
 * \copyright  (C) 2022 Cerberus team
 *             The reproduction, distribution and utilization of this file as
 *             well as the communication of its contents to others without express
 *             authorization is prohibited. Offenders will be held liable for the
 *             payment of damages. All rights reserved in the event of the grant
 *             of a patent, utility model or design.
 * }
 */

#ifndef __CFG_H__
#define __CFG_H__

/* Feature switch */
#define MONITOR_DATA_TO_PC

/* Configure for feature */
#define DELAY_TIME              (100)
#define BACK_TO_NORMAL_TIME     (3000)
#define ALIVE_LED_TIME          (500)
#define TURN_ANGLE              (20)

/* Hardware pin */
#define ALIVE_LED_PIN           (9)
#define SIGNAL_RIGHT_PIN        (10)
#define SIGNAL_LEFT_PIN         (11)
#define LIGHT_CONTROL_PIN       (12)

#endif
