#
# Copyright (c) 2026 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

add_subdirectory_ifdef(CONFIG_MATTER_ZIGBEE_COEXISTENCE matter_zigbee_coexistence)
add_subdirectory_ifdef(CONFIG_MATTER_ZIGBEE_PROTOCOL_STATE matter_zigbee_protocol_state)
add_subdirectory_ifdef(CONFIG_MATTER_ZIGBEE_SMP_DFU matter_zigbee_smp_dfu)
