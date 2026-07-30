/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef APP_UI_CONFIG_H_
#define APP_UI_CONFIG_H_

/** @file
 * @brief Light switch sample UI configuration (buttons and LEDs).
 *
 * Common Matter + Zigbee UI buttons and LEDs are defined in
 * @ref matter_zigbee_ui_config.h. This file adds sample-specific mappings
 * shared by the Matter and Zigbee application tasks.
 */

#include <matter_zigbee_ui_config.h>

#include <zephyr/kernel.h>

/** Button 3: toggle/dim bound lights (Zigbee) or dimmer trigger (Matter). */
#define APP_UI_BUTTON_LIGHT_TOGGLE DK_BTN3_MSK

#if defined(CONFIG_ZIGBEE_TOUCHLINK_INITIATOR)
/** Button 2 short press: Touchlink initiator (Zigbee). */
#define APP_UI_BUTTON_ZIGBEE_TOUCHLINK DK_BTN2_MSK
#endif

#if defined(CONFIG_CHIP_ICD_UAT_SUPPORT)
/** Button 2: ICD User Active Mode trigger (Matter). */
#define APP_UI_BUTTON_MATTER_ICD_UAT DK_BTN2_MSK
#endif

/** LED solid on when a controllable bulb has been found (Zigbee). */
#define APP_UI_LED_ZIGBEE_BULB_FOUND DK_LED3

/** Sampled once at boot to enable sleepy End Device behavior (Zigbee). */
#define APP_UI_BUTTON_ZIGBEE_SLEEPY DK_BTN3_MSK

/** Hold time before continuous dim-up steps start (Zigbee). */
#define APP_UI_BUTTON_ZIGBEE_LONG_PRESS_TIMEOUT K_MSEC(500)

#endif /* APP_UI_CONFIG_H_ */
