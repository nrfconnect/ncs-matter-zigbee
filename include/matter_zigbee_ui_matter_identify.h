/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MATTER_ZIGBEE_UI_MATTER_IDENTIFY_H_
#define MATTER_ZIGBEE_UI_MATTER_IDENTIFY_H_

/** @file
 * @brief Matter Identify cluster using @ref MATTER_ZIGBEE_UI_LED_IDENTIFY.
 */

#include <app/DefaultTimerDelegate.h>
#include <app/clusters/identify-server/IdentifyCluster.h>
#include <app/server-cluster/ServerClusterInterfaceRegistry.h>
#include <data-model-providers/codegen/CodegenDataModelProvider.h>
#include <lib/support/TimerDelegate.h>

/** @brief Identify delegate that blinks @ref MATTER_ZIGBEE_UI_LED_IDENTIFY. */
class IdentifyDelegate : public chip::app::Clusters::IdentifyDelegate {
public:
	explicit IdentifyDelegate(bool isTriggerEffectEnabled = false)
		: mIsTriggerEffectEnabled(isTriggerEffectEnabled)
	{
	}

	void OnIdentifyStart(chip::app::Clusters::IdentifyCluster &cluster) override;
	void OnIdentifyStop(chip::app::Clusters::IdentifyCluster &cluster) override;
	void OnTriggerEffect(chip::app::Clusters::IdentifyCluster &cluster) override;
	bool IsTriggerEffectEnabled() const override { return mIsTriggerEffectEnabled; }

private:
	bool mIsTriggerEffectEnabled = false;
};

/** @brief Matter Identify cluster wired to the common UI identify LED. */
class MatterIdentifyCluster {
public:
	explicit MatterIdentifyCluster(
		chip::EndpointId endpoint, bool isTriggerEffectEnabled = false,
		chip::app::Clusters::Identify::IdentifyTypeEnum identifyType =
			chip::app::Clusters::Identify::IdentifyTypeEnum::kVisibleIndicator)
		: mEndpointId(endpoint), mIdentifyDelegate(isTriggerEffectEnabled),
		  mIdentifyCluster(chip::app::Clusters::IdentifyCluster::Config(endpoint, mTimerDelegate)
					   .WithIdentifyType(identifyType)
					   .WithDelegate(&mIdentifyDelegate))
	{
	}

	CHIP_ERROR Init()
	{
		if (chip::app::CodegenDataModelProvider::Instance().Registry().Get(
			    { mEndpointId, chip::app::Clusters::Identify::Id }) != nullptr) {
			return CHIP_NO_ERROR;
		}

		return chip::app::CodegenDataModelProvider::Instance().Registry().Register(
			mIdentifyCluster.Registration());
	}

private:
	chip::EndpointId mEndpointId;
	IdentifyDelegate mIdentifyDelegate;
	chip::app::RegisteredServerCluster<chip::app::Clusters::IdentifyCluster> mIdentifyCluster;
	chip::app::DefaultTimerDelegate mTimerDelegate;
};

#endif /* MATTER_ZIGBEE_UI_MATTER_IDENTIFY_H_ */
