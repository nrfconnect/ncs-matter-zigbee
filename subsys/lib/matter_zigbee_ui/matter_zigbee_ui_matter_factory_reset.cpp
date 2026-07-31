/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <matter_zigbee_ui_matter_factory_reset.h>

#include <app/server/Server.h>
#include <app/task_executor.h>

#include <matter_zigbee_ui_config.h>

#include <dk_buttons_and_leds.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(matter_zigbee_ui, CONFIG_MATTER_ZIGBEE_UI_LOG_LEVEL);

namespace
{

enum class FactoryResetState : uint8_t { None, TriggerWait, CancelWindow };

/* Half of CONFIG_MATTER_ZIGBEE_UI_FACTORY_RESET_PRESS_TIME_SECONDS value is the initial hold 
 * before the cancel window opens; the other half is the cancel window duration
 * (keeping the button pressed through it confirms the reset). 
 */
constexpr uint32_t kFactoryResetTriggerTimeoutMs =
	(CONFIG_MATTER_ZIGBEE_UI_FACTORY_RESET_PRESS_TIME_SECONDS * 1000U) / 2U;
constexpr uint32_t kFactoryResetCancelWindowTimeoutMs =
	(CONFIG_MATTER_ZIGBEE_UI_FACTORY_RESET_PRESS_TIME_SECONDS * 1000U) / 2U;

FactoryResetState s_factory_reset_state = FactoryResetState::None;
bool s_factory_reset_timer_active;
k_timer s_factory_reset_timer;

void factory_reset_timer_event_handler()
{
	if (s_factory_reset_state == FactoryResetState::TriggerWait) {
		LOG_INF("Factory reset has been triggered. Release button within %u ms to cancel.",
			kFactoryResetCancelWindowTimeoutMs);
		s_factory_reset_state = FactoryResetState::CancelWindow;
		k_timer_start(&s_factory_reset_timer, K_MSEC(kFactoryResetCancelWindowTimeoutMs), K_NO_WAIT);
		return;
	}

	if (s_factory_reset_state == FactoryResetState::CancelWindow) {
		s_factory_reset_state = FactoryResetState::None;
		s_factory_reset_timer_active = false;
		chip::Server::GetInstance().ScheduleFactoryReset();
	}
}

void factory_reset_timer_timeout(k_timer *timer)
{
	ARG_UNUSED(timer);
	Nrf::PostTask([] { factory_reset_timer_event_handler(); });
}

void cancel_factory_reset_timer(void)
{
	if (!s_factory_reset_timer_active) {
		return;
	}

	k_timer_stop(&s_factory_reset_timer);
	s_factory_reset_timer_active = false;
}

} /* namespace */

extern "C" void matter_zigbee_ui_matter_factory_reset_init(void)
{
	k_timer_init(&s_factory_reset_timer, factory_reset_timer_timeout, nullptr);
}

extern "C" void matter_zigbee_ui_matter_factory_reset_button_handler(uint32_t button_state, uint32_t has_changed)
{
	if (!(has_changed & MATTER_ZIGBEE_UI_BUTTON_FACTORY_RESET)) {
		return;
	}

	const bool pressed = (button_state & MATTER_ZIGBEE_UI_BUTTON_FACTORY_RESET) != 0U;

	if (pressed) {
		if (s_factory_reset_state == FactoryResetState::None && !s_factory_reset_timer_active) {
			s_factory_reset_state = FactoryResetState::TriggerWait;
			k_timer_start(&s_factory_reset_timer, K_MSEC(kFactoryResetTriggerTimeoutMs), K_NO_WAIT);
			s_factory_reset_timer_active = true;
		}
		return;
	}

	if (s_factory_reset_state == FactoryResetState::TriggerWait && s_factory_reset_timer_active) {
		cancel_factory_reset_timer();
		s_factory_reset_state = FactoryResetState::None;
		return;
	}

	if (s_factory_reset_state == FactoryResetState::CancelWindow && s_factory_reset_timer_active) {
		cancel_factory_reset_timer();
		s_factory_reset_state = FactoryResetState::None;
		LOG_INF("Factory reset has been canceled");
	}
}
