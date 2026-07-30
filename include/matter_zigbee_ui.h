/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MATTER_ZIGBEE_UI_H_
#define MATTER_ZIGBEE_UI_H_

/** @file
 * @defgroup matter_zigbee_ui Common Matter + Zigbee UI
 * @{
 *
 * @brief Shared button and LED handling for combined Matter + Zigbee samples.
 *
 * Samples register optional callbacks from @c main() and pass
 * @ref matter_zigbee_ui_register as the coexistence
 * @c post_matter_ui_init hook once Matter server initialisation is complete.
 *
 * Factory reset (@ref MATTER_ZIGBEE_UI_BUTTON_FACTORY_RESET) and SMP DFU
 * (@ref MATTER_ZIGBEE_UI_BUTTON_SMP_DFU) may use the same physical button:
 * a short press starts SMP DFU; a long press factory-resets the active stack.
 * Matter factory-reset timing is configured by
 * @kconfig{CONFIG_MATTER_ZIGBEE_UI_FACTORY_RESET_PRESS_TIME_SECONDS} (split
 * equally between the initial hold and cancel window).
 *
 * SMP DFU requires @kconfig{CONFIG_MATTER_ZIGBEE_UI_SMP_DFU}.
 */

#include <matter_zigbee_protocol_state.h>

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Sample-supplied UI callbacks. */
struct matter_zigbee_ui_callbacks {
	/** Optional button handler invoked for every DK button event.
	 *
	 *  @param button_state Current button state bitmask.
	 *  @param has_changed  Bitmask of buttons that changed state.
	 *  @param active_protocol Persisted active protocol at event time.
	 *
	 *  @return true if the sample consumed the event, false otherwise.
	 */
	bool (*on_button)(uint32_t button_state, uint32_t has_changed, active_protocol_t active_protocol);
};

/** @brief Register sample UI callbacks.
 *
 * Must be called once from @c main() before @ref matter_zigbee_coexistence_run().
 * The pointer must remain valid for the lifetime of the application.
 *
 * @param callbacks Callback table. May be @c NULL to use common UI only.
 */
void matter_zigbee_ui_set_callbacks(const struct matter_zigbee_ui_callbacks *callbacks);

/** @brief Consume a protocol-switch short-press indication from the last button event.
 *
 * When @kconfig{CONFIG_MATTER_ZIGBEE_COEXISTENCE_BUTTON_SWITCH} is enabled, the
 * common UI module calls @ref matter_zigbee_coexistence_process_switch_button()
 * before the sample callback. If that call reports a short release, the
 * indication can be retrieved once through this function (for example to start
 * Touchlink on the light switch sample).
 *
 * @return true if the previous button event ended with a protocol-switch short
 *         release, false otherwise.
 */
bool matter_zigbee_ui_consume_protocol_switch_short_release(void);

/** @brief Initialise DK buttons and register the common UI handler.
 *
 * Intended for use as the coexistence @c post_matter_ui_init callback.
 * Calls @c dk_buttons_init() with the common dispatcher that forwards events
 * to callbacks registered via @ref matter_zigbee_ui_set_callbacks().
 *
 * Safe to call more than once; only the first successful call has effect.
 */
void matter_zigbee_ui_register(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* MATTER_ZIGBEE_UI_H_ */
