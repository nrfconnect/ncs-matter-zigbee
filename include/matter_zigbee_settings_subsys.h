/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file matter_zigbee_settings_subsys.h
 *  @brief Zephyr Settings keys for ncs-matter-zigbee libraries.
 *
 *  Reuses the Zigbee settings subtree from ncs-zigbee for storage layout
 *  compatibility. Only Matter+Zigbee-specific keys are defined here.
 */

#ifndef MATTER_ZIGBEE_SETTINGS_SUBSYS_H_
#define MATTER_ZIGBEE_SETTINGS_SUBSYS_H_

#include <zigbee/zigbee_settings_subsys.h>

/** Leaf segment for the persisted active-protocol value. */
#define MATTER_ZIGBEE_SETTINGS_KEY_PROTOCOL_STATE "pt"

#define MATTER_ZIGBEE_SETTINGS_FULL_NAME_PROTOCOL_STATE                                                                \
	ZIGBEE_SETTINGS_SUBSYS_NAME "/" MATTER_ZIGBEE_SETTINGS_KEY_PROTOCOL_STATE

#endif /* MATTER_ZIGBEE_SETTINGS_SUBSYS_H_ */
