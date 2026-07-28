/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MATTER_ZIGBEE_SMP_DFU_H_
#define MATTER_ZIGBEE_SMP_DFU_H_

/** @file
 * @defgroup matter_zigbee_smp_dfu Matter + Zigbee SMP DFU button handler
 * @{
 *
 * @brief Replaces the default Matter board Bluetooth-advertising button
 * handler with logic that works when Thread is not running (for example while
 * Zigbee owns the active protocol).
 *
 * Requires @kconfig{CONFIG_NCS_SAMPLE_MATTER_CUSTOM_BLUETOOTH_ADVERTISING}.
 * Call @ref matter_zigbee_smp_dfu_init() from the Matter application after
 * @c Nrf::GetBoard().Init() completes.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Register the SMP DFU button handler.
 *
 * Safe to call more than once; only the first call has effect.
 */
void matter_zigbee_smp_dfu_init(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* MATTER_ZIGBEE_SMP_DFU_H_ */
