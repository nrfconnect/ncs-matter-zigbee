/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *
 * @brief Matter + Zigbee light bulb entry point.
 */

#include "app_task_matter.h"
#include "app_task_zigbee.h"

#include <matter_zigbee_coexistence.h>
#include <matter_zigbee_ui.h>

namespace
{

void matter_start_cb(void)
{
	AppTask::Instance().StartApp();
}

bool app_ui_button_handler(uint32_t button_state, uint32_t has_changed, active_protocol_t active_protocol)
{
	/* Handle button events only for the active protocol. */
	if (active_protocol == PROTOCOL_ZIGBEE) {
		zb_button_handler(button_state, has_changed);
	}

	if (active_protocol == PROTOCOL_MATTER) {
		AppTask::ButtonEventHandler(button_state, has_changed);
	}

	return false;
}

const struct matter_zigbee_ui_callbacks ui_callbacks = {
	.on_button = app_ui_button_handler,
};

const struct matter_zigbee_coexistence_callbacks coexistence_cb = {
	matter_start_cb,
	ZigbeeStart,
	matter_zigbee_ui_register,
};

} /* namespace */

int main(void)
{
	matter_zigbee_ui_set_callbacks(&ui_callbacks);
	return matter_zigbee_coexistence_run(&coexistence_cb);
}
