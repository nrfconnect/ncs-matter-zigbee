/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef APP_TASK_ZIGBEE_H
#define APP_TASK_ZIGBEE_H 1

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int ZigbeeStart(void);

#ifdef CONFIG_MATTER_ZIGBEE_COEXISTENCE
/**
 * @brief Sample-specific button handler for Zigbee in combined builds.
 *
 * Invoked from the common UI module when a button event is received.
 * Processes button events when Zigbee protocol is active and returns early
 * if Matter is active.
 *
 * @param button_state  Bitmask containing buttons state.
 * @param has_changed   Bitmask containing buttons that changed state.
 */
void zb_button_handler(uint32_t button_state, uint32_t has_changed);
#endif /* CONFIG_MATTER_ZIGBEE_COEXISTENCE */

#ifdef __cplusplus
}
#endif

#endif /* APP_TASK_ZIGBEE_H */
