/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <matter_zigbee_ui_matter_status.h>

#include <app/matter_event_handler.h>
#include <app/task_executor.h>

#include <matter_zigbee_protocol_state.h>
#include <matter_zigbee_ui.h>
#include <matter_zigbee_ui_config.h>

#include <platform/CHIPDeviceLayer.h>

namespace
{

enum class DeviceState : uint8_t { Disconnected, AdvertisingBle, ConnectedBle, Provisioned };

DeviceState s_device_state = DeviceState::Disconnected;
bool s_network_provisioned;
bool s_ble_connected;
bool s_initialized;

constexpr uint32_t kDisconnectedOnMs = 50U;
constexpr uint32_t kDisconnectedOffMs = 950U;
constexpr uint32_t kBleConnectedOnMs = 100U;
constexpr uint32_t kBleConnectedOffMs = 100U;

void apply_status_led(void)
{
	if (!protocol_is_matter_active()) {
		matter_zigbee_ui_led_blink_stop(MATTER_ZIGBEE_UI_LED_MATTER);
		matter_zigbee_ui_led_set(MATTER_ZIGBEE_UI_LED_MATTER, false);
		return;
	}

	switch (s_device_state) {
	case DeviceState::Provisioned:
		matter_zigbee_ui_led_blink_stop(MATTER_ZIGBEE_UI_LED_MATTER);
		matter_zigbee_ui_led_set(MATTER_ZIGBEE_UI_LED_MATTER, true);
		break;
	case DeviceState::ConnectedBle:
		matter_zigbee_ui_led_blink(MATTER_ZIGBEE_UI_LED_MATTER, kBleConnectedOnMs, kBleConnectedOffMs);
		break;
	case DeviceState::AdvertisingBle:
	case DeviceState::Disconnected:
	default:
		matter_zigbee_ui_led_blink(MATTER_ZIGBEE_UI_LED_MATTER, kDisconnectedOnMs, kDisconnectedOffMs);
		break;
	}
}

void update_device_state(void)
{
	using namespace chip::DeviceLayer;

	DeviceState new_state;

	if (s_network_provisioned) {
		new_state = DeviceState::Provisioned;
	} else if (s_ble_connected) {
		new_state = DeviceState::ConnectedBle;
	} else if (ConnectivityMgr().IsBLEAdvertising()) {
		new_state = DeviceState::AdvertisingBle;
	} else {
		new_state = DeviceState::Disconnected;
	}

	s_device_state = new_state;
	apply_status_led();
}

void refresh_from_current_state(void)
{
	using namespace chip::DeviceLayer;

	s_ble_connected = ConnectivityMgr().NumBLEConnections() != 0;
	s_network_provisioned = ConnectivityMgrImpl().IsIPv6NetworkProvisioned() &&
				ConnectivityMgrImpl().IsIPv6NetworkEnabled();
	update_device_state();
}

void matter_status_event_handler(const chip::DeviceLayer::ChipDeviceEvent *event, intptr_t /* unused */)
{
	using namespace chip::DeviceLayer;

	switch (event->Type) {
	case DeviceEventType::kCHIPoBLEAdvertisingChange:
		s_ble_connected = ConnectivityMgr().NumBLEConnections() != 0;
		break;
	case DeviceEventType::kThreadStateChange:
	case DeviceEventType::kWiFiConnectivityChange:
		s_network_provisioned = ConnectivityMgrImpl().IsIPv6NetworkProvisioned() &&
					ConnectivityMgrImpl().IsIPv6NetworkEnabled();
		break;
	default:
		return;
	}

	Nrf::PostTask([] { update_device_state(); });
}

} /* namespace */

extern "C" void matter_zigbee_ui_matter_status_init(void)
{
	if (s_initialized) {
		return;
	}

	if (Nrf::Matter::RegisterEventHandler(matter_status_event_handler, 0) != CHIP_NO_ERROR) {
		return;
	}

	s_initialized = true;
}

extern "C" void matter_zigbee_ui_matter_status_refresh(void)
{
	if (!s_initialized) {
		return;
	}

	Nrf::PostTask([] { refresh_from_current_state(); });
}
