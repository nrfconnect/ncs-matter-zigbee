# Copyright (c) 2026 Nordic Semiconductor ASA
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
# CMake entry point for the ncs-matter-zigbee add-on module.

if(CONFIG_MATTER_ZIGBEE_ADD_ON)
	zephyr_include_directories(include)

	add_subdirectory(subsys)
endif()

