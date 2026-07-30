/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <matter_zigbee_ui_smp_dfu.h>

#include <app/server/Server.h>
#include <app/task_executor.h>
#include <board/board.h>

#include <dfu/smp/dfu_over_smp.h>
#include <matter_zigbee_ui_config.h>

#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(matter_zigbee_ui, CONFIG_MATTER_ZIGBEE_UI_LOG_LEVEL);

namespace
{

void start_ble_advertisement_on_button_press(void)
{
	if (chip::Server::GetInstance().GetFabricTable().FabricCount() > 0) {
		Nrf::GetDFUOverSMP().StartServer();
		return;
	}

	Nrf::Board::StartBLEAdvertisement();
}

} /* namespace */

extern "C" void matter_zigbee_ui_smp_dfu_button(uint32_t button_state, uint32_t has_changed)
{
	if (!(has_changed & MATTER_ZIGBEE_UI_BUTTON_SMP_DFU) ||
	    !(button_state & MATTER_ZIGBEE_UI_BUTTON_SMP_DFU)) {
		return;
	}

	Nrf::PostTask([] { start_ble_advertisement_on_button_press(); });
}
