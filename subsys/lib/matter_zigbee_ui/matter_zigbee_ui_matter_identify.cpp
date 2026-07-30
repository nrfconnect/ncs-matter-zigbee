/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <matter_zigbee_ui_matter_identify.h>

#include <app/task_executor.h>
#include <matter_zigbee_ui.h>
#include <matter_zigbee_ui_config.h>

namespace matter_zigbee_ui
{

namespace
{

constexpr uint32_t kIdentifyBlinkMs = 500U;
constexpr uint32_t kTriggerEffectStartMs = 5000U;
constexpr uint32_t kTriggerEffectFinishMs = 1000U;

void stop_identify_led(void)
{
	matter_zigbee_ui_led_blink_stop(MATTER_ZIGBEE_UI_LED_IDENTIFY);
	matter_zigbee_ui_led_set(MATTER_ZIGBEE_UI_LED_IDENTIFY, false);
}

} /* namespace */

void IdentifyDelegate::OnIdentifyStart(chip::app::Clusters::IdentifyCluster &cluster)
{
	ARG_UNUSED(cluster);

	Nrf::PostTask([] {
		matter_zigbee_ui_led_blink(MATTER_ZIGBEE_UI_LED_IDENTIFY, kIdentifyBlinkMs, kIdentifyBlinkMs);
	});
}

void IdentifyDelegate::OnIdentifyStop(chip::app::Clusters::IdentifyCluster &cluster)
{
	ARG_UNUSED(cluster);

	Nrf::PostTask([] { stop_identify_led(); });
}

void IdentifyDelegate::OnTriggerEffect(chip::app::Clusters::IdentifyCluster &cluster)
{
	switch (cluster.GetEffectIdentifier()) {
	case chip::app::Clusters::Identify::EffectIdentifierEnum::kBlink:
	case chip::app::Clusters::Identify::EffectIdentifierEnum::kBreathe:
		Nrf::PostTask([] {
			matter_zigbee_ui_led_blink(MATTER_ZIGBEE_UI_LED_IDENTIFY, kTriggerEffectStartMs,
						   kTriggerEffectFinishMs);
		});
		break;
	case chip::app::Clusters::Identify::EffectIdentifierEnum::kOkay:
	case chip::app::Clusters::Identify::EffectIdentifierEnum::kChannelChange:
		Nrf::PostTask([] { matter_zigbee_ui_led_set(MATTER_ZIGBEE_UI_LED_IDENTIFY, true); });
		break;
	case chip::app::Clusters::Identify::EffectIdentifierEnum::kFinishEffect:
	case chip::app::Clusters::Identify::EffectIdentifierEnum::kStopEffect:
		Nrf::PostTask([] { stop_identify_led(); });
		break;
	default:
		break;
	}
}

} /* namespace matter_zigbee_ui */
