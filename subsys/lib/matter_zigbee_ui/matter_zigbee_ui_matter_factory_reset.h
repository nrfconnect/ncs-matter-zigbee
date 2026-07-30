/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MATTER_ZIGBEE_UI_MATTER_FACTORY_RESET_H_
#define MATTER_ZIGBEE_UI_MATTER_FACTORY_RESET_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void matter_zigbee_ui_matter_factory_reset_init(void);
void matter_zigbee_ui_matter_factory_reset_button(uint32_t button_state, uint32_t has_changed);

#ifdef __cplusplus
}
#endif

#endif /* MATTER_ZIGBEE_UI_MATTER_FACTORY_RESET_H_ */
