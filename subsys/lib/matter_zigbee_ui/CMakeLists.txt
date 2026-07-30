#
# Copyright (c) 2026 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

zephyr_library()
zephyr_library_sources(matter_zigbee_ui.c)
zephyr_library_sources(matter_zigbee_ui_matter_factory_reset.cpp)
zephyr_library_sources_ifdef(CONFIG_MATTER_ZIGBEE_UI_SMP_DFU matter_zigbee_ui_smp_dfu.cpp)
