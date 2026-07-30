/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <matter_zigbee_ui.h>

#include <matter_zigbee_coexistence.h>
#include <matter_zigbee_ui_config.h>
#include <matter_zigbee_ui_matter_factory_reset.h>
#include <matter_zigbee_ui_matter_status.h>
#if defined(CONFIG_MATTER_ZIGBEE_UI_SMP_DFU)
#include <matter_zigbee_ui_smp_dfu.h>
#endif

#include <dk_buttons_and_leds.h>
#include <zboss_api.h>
#include <zigbee/zigbee_app_utils.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(matter_zigbee_ui, CONFIG_MATTER_ZIGBEE_UI_LOG_LEVEL);

#define MATTER_ZIGBEE_UI_MAX_LEDS 4
#define MATTER_ZIGBEE_UI_ZIGBEE_NETWORK_BLINK_MS 500U

struct ui_led_blink_ctx {
	struct k_timer timer;
	uint32_t on_ms;
	uint32_t off_ms;
	bool active;
};

static bool s_led_states[MATTER_ZIGBEE_UI_MAX_LEDS];
static struct ui_led_blink_ctx s_blink[MATTER_ZIGBEE_UI_MAX_LEDS];
static const struct matter_zigbee_ui_callbacks *s_callbacks;
static bool s_registered;
static bool s_protocol_switch_short_release;
static bool s_zigbee_network_joined;

static void zigbee_network_led_apply(void)
{
	if (!protocol_is_zigbee_active()) {
		matter_zigbee_ui_led_blink_stop(MATTER_ZIGBEE_UI_LED_ZIGBEE);
		matter_zigbee_ui_led_set(MATTER_ZIGBEE_UI_LED_ZIGBEE, false);
		return;
	}

	if (s_zigbee_network_joined) {
		matter_zigbee_ui_led_blink_stop(MATTER_ZIGBEE_UI_LED_ZIGBEE);
		matter_zigbee_ui_led_set(MATTER_ZIGBEE_UI_LED_ZIGBEE, true);
	} else {
		matter_zigbee_ui_led_blink(MATTER_ZIGBEE_UI_LED_ZIGBEE,
					   MATTER_ZIGBEE_UI_ZIGBEE_NETWORK_BLINK_MS,
					   MATTER_ZIGBEE_UI_ZIGBEE_NETWORK_BLINK_MS);
	}
}

void matter_zigbee_ui_zigbee_network_led_update(uint8_t bufid)
{
	zb_zdo_app_signal_hdr_t *signal_header = NULL;
	const zb_zdo_app_signal_type_t signal = zb_get_app_signal(bufid, &signal_header);
	const zb_ret_t status = ZB_GET_APP_SIGNAL_STATUS(bufid);

	if (!protocol_is_zigbee_active()) {
		zigbee_network_led_apply();
		return;
	}

	switch (signal) {
	case ZB_BDB_SIGNAL_DEVICE_REBOOT:
	/* fall-through */
	case ZB_BDB_SIGNAL_STEERING:
		s_zigbee_network_joined = (status == RET_OK);
		break;

	case ZB_ZDO_SIGNAL_LEAVE:
		s_zigbee_network_joined = false;
		break;

	default:
		return;
	}

	zigbee_network_led_apply();
}

void matter_zigbee_ui_protocol_leds_refresh(void)
{
	matter_zigbee_ui_matter_status_refresh();
	zigbee_network_led_apply();
}

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

static void led_apply(uint8_t led, bool on)
{
	if (led < MATTER_ZIGBEE_UI_MAX_LEDS) {
		s_led_states[led] = on;
	}

	(void)dk_set_led(led, on ? 1U : 0U);
}

static void led_blink_timer_handler(struct k_timer *timer)
{
	const uint8_t led = (uint8_t)(uintptr_t)k_timer_user_data_get(timer);

	if (led >= MATTER_ZIGBEE_UI_MAX_LEDS || !s_blink[led].active) {
		return;
	}

	led_apply(led, !s_led_states[led]);

	const uint32_t delay = s_led_states[led] ? s_blink[led].on_ms : s_blink[led].off_ms;

	k_timer_start(timer, K_MSEC(delay), K_NO_WAIT);
}

static void led_blink_timers_init(void)
{
	for (uint8_t led = 0; led < MATTER_ZIGBEE_UI_MAX_LEDS; led++) {
		k_timer_init(&s_blink[led].timer, led_blink_timer_handler, NULL);
		k_timer_user_data_set(&s_blink[led].timer, (void *)(uintptr_t)led);
	}
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
	led_blink_timers_init();

	err = dk_buttons_init(ui_button_handler);
	if (err) {
		LOG_ERR("Cannot init buttons (err: %d)", err);
		return;
	}

	err = dk_leds_init();
	if (err) {
		LOG_ERR("Cannot init LEDs (err: %d)", err);
	}

	matter_zigbee_ui_matter_status_init();
	s_zigbee_network_joined = false;
	matter_zigbee_ui_protocol_leds_refresh();

	s_registered = true;
}

void matter_zigbee_ui_led_set(uint8_t led, bool on)
{
	matter_zigbee_ui_led_blink_stop(led);
	led_apply(led, on);
}

bool matter_zigbee_ui_led_get(uint8_t led)
{
	if (led >= MATTER_ZIGBEE_UI_MAX_LEDS) {
		return false;
	}

	return s_led_states[led];
}

void matter_zigbee_ui_led_toggle(uint8_t led)
{
	matter_zigbee_ui_led_set(led, !matter_zigbee_ui_led_get(led));
}

void matter_zigbee_ui_led_blink(uint8_t led, uint32_t on_ms, uint32_t off_ms)
{
	if (led >= MATTER_ZIGBEE_UI_MAX_LEDS || on_ms == 0U || off_ms == 0U) {
		return;
	}

	matter_zigbee_ui_led_blink_stop(led);

	s_blink[led].on_ms = on_ms;
	s_blink[led].off_ms = off_ms;
	s_blink[led].active = true;

	led_apply(led, !s_led_states[led]);

	const uint32_t delay = s_led_states[led] ? on_ms : off_ms;

	k_timer_start(&s_blink[led].timer, K_MSEC(delay), K_NO_WAIT);
}

void matter_zigbee_ui_led_blink_stop(uint8_t led)
{
	if (led >= MATTER_ZIGBEE_UI_MAX_LEDS) {
		return;
	}

	k_timer_stop(&s_blink[led].timer);
	s_blink[led].active = false;
	s_blink[led].on_ms = 0U;
	s_blink[led].off_ms = 0U;
}
