/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MATTER_ZIGBEE_UI_MATTER_STATUS_H_
#define MATTER_ZIGBEE_UI_MATTER_STATUS_H_

/** @file
 * @brief Internal Matter connectivity status LED helpers.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Register the Matter connectivity status event handler (idempotent). */
void matter_zigbee_ui_matter_status_init(void);

/** @brief Re-evaluate @ref MATTER_ZIGBEE_UI_LED_MATTER from current stack state. */
void matter_zigbee_ui_matter_status_refresh(void);

#ifdef __cplusplus
}
#endif

#endif /* MATTER_ZIGBEE_UI_MATTER_STATUS_H_ */
