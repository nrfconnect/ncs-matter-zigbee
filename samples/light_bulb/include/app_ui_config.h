/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef APP_UI_CONFIG_H_
#define APP_UI_CONFIG_H_

/** @file
 * @brief Light bulb sample UI configuration (buttons and LEDs).
 *
 * Common Matter + Zigbee UI buttons and LEDs are defined in
 * @ref matter_zigbee_ui_config.h. This file adds sample-specific mappings
 * shared by the Matter and Zigbee application tasks.
 */

#include <matter_zigbee_ui_config.h>

#include <zephyr/devicetree.h>

/** Button 3: toggle main light (Matter). */
#define APP_UI_BUTTON_LIGHT_TOGGLE DK_BTN3_MSK

/** Main light output on DK_LED4 (onboard LED 3), driven by PWM when enabled. */
#define APP_UI_LED_MAIN_LIGHT DK_LED4

/** PWM devicetree node for the main light (board overlay: pwm-led1 alias). */
#define APP_UI_PWM_LIGHT_NODE DT_ALIAS(pwm_led1)

#endif /* APP_UI_CONFIG_H_ */
