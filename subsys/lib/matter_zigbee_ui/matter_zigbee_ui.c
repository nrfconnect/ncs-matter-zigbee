/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <matter_zigbee_ui.h>

#include <matter_zigbee_coexistence.h>
#include <matter_zigbee_ui_config.h>
#include <matter_zigbee_ui_matter_factory_reset.h>
#if defined(CONFIG_MATTER_ZIGBEE_UI_SMP_DFU)
#include <matter_zigbee_ui_smp_dfu.h>
#endif

#include <dk_buttons_and_leds.h>
#include <zigbee/zigbee_app_utils.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(matter_zigbee_ui, CONFIG_MATTER_ZIGBEE_UI_LOG_LEVEL);

static const struct matter_zigbee_ui_callbacks *s_callbacks;
static bool s_registered;
static bool s_protocol_switch_short_release;

void matter_zigbee_ui_set_callbacks(const struct matter_zigbee_ui_callbacks *callbacks)
{
	s_callbacks = callbacks;
}

bool matter_zigbee_ui_consume_protocol_switch_short_release(void)
{
	const bool short_release = s_protocol_switch_short_release;

	s_protocol_switch_short_release = false;
	return short_release;
}

static void ui_handle_factory_reset(uint32_t button_state, uint32_t has_changed)
{
	if (protocol_state_get() == PROTOCOL_ZIGBEE) {
#if defined(CONFIG_ZIGBEE_FACTORY_RESET)
		check_factory_reset_button(button_state, has_changed);
#endif
	} else {
		matter_zigbee_ui_matter_factory_reset_button(button_state, has_changed);
	}
}

static void ui_button_handler(uint32_t button_state, uint32_t has_changed)
{
#if defined(CONFIG_MATTER_ZIGBEE_COEXISTENCE_BUTTON_SWITCH)
	s_protocol_switch_short_release = matter_zigbee_coexistence_process_switch_button(
		button_state, has_changed, MATTER_ZIGBEE_UI_BUTTON_PROTOCOL_SWITCH);
#else
	s_protocol_switch_short_release = false;
#endif

#if defined(CONFIG_MATTER_ZIGBEE_UI_SMP_DFU)
	matter_zigbee_ui_smp_dfu_button(button_state, has_changed);
#endif

	ui_handle_factory_reset(button_state, has_changed);

	if (s_callbacks != NULL && s_callbacks->on_button != NULL) {
		(void)s_callbacks->on_button(button_state, has_changed, protocol_state_get());
	}
}

void matter_zigbee_ui_register(void)
{
	int err;

	if (s_registered) {
		return;
	}

#if defined(CONFIG_ZIGBEE_FACTORY_RESET)
	register_factory_reset_button(MATTER_ZIGBEE_UI_BUTTON_FACTORY_RESET);
#endif

	matter_zigbee_ui_matter_factory_reset_init();

	err = dk_buttons_init(ui_button_handler);
	if (err) {
		LOG_ERR("Cannot init buttons (err: %d)", err);
		return;
	}

	s_registered = true;
}
