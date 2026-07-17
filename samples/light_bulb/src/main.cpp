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

namespace
{

void matter_start_cb(void)
{
	AppTask::Instance().StartApp();
}

const struct matter_zigbee_coexistence_callbacks coexistence_cb = {
	matter_start_cb,
	ZigbeeStart,
	zb_register_button_handler,
};

} /* namespace */

int main(void)
{
	return matter_zigbee_coexistence_run(&coexistence_cb);
}
