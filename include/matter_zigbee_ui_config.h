/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MATTER_ZIGBEE_UI_CONFIG_H_
#define MATTER_ZIGBEE_UI_CONFIG_H_

/** @file
 * @defgroup matter_zigbee_ui_config common Matter + Zigbee UI configuration
 * @{
 *
 * @brief Common configuration for Matter + Zigbee UI used in samples.
 */

#include <dk_buttons_and_leds.h>
#include <zephyr/kernel.h>

#define MATTER_ZIGBEE_UI_BUTTON_SMP_DFU DK_BTN1_MSK
#define MATTER_ZIGBEE_UI_BUTTON_FACTORY_RESET DK_BTN1_MSK
#define MATTER_ZIGBEE_UI_BUTTON_PROTOCOL_SWITCH DK_BTN2_MSK
#define MATTER_ZIGBEE_UI_BUTTON_IDENTIFY DK_BTN4_MSK

#define MATTER_ZIGBEE_UI_LED_MATTER DK_LED1
#define MATTER_ZIGBEE_UI_LED_ZIGBEE DK_LED2
#define MATTER_ZIGBEE_UI_LED_IDENTIFY DK_LED4

/** @} */

#endif /* MATTER_ZIGBEE_UI_CONFIG_H_ */
