#
# Copyright (c) 2026 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project("Matter + Zigbee Light Switch")

include(${ZEPHYR_CONNECTEDHOMEIP_MODULE_DIR}/config/nrfconnect/app/enable-gnu-std.cmake)
include(${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/cmake/source_common.cmake)
include(${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/cmake/data_model.cmake)
include(${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/cmake/zap_helpers.cmake)
ncs_get_zap_parent_dir(ZAP_PARENT_DIR)

target_include_directories(app PRIVATE
	src
	include
	${ZAP_PARENT_DIR}
)

target_compile_options(app PRIVATE -Wno-deprecated-declarations)

target_sources(app PRIVATE
	src/main.cpp
	src/app_task_matter.cpp
	src/app_task_zigbee.c
	src/zboss_mem_globals.c
	src/light_switch.cpp
	src/shell_commands.cpp
	${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/src/binding/binding_handler.cpp
)

# LTO link: zb_memconfig.o from prebuilts is plain ELF (weak gc_*); strong gc_*
# in zb_mem_config_context.h must not be LTO bytecode or ld warns about symbol size.
if(CONFIG_LTO)
	set_source_files_properties(
		src/zboss_mem_globals.c
		PROPERTIES COMPILE_OPTIONS -fno-lto
	)
endif()

ncs_configure_data_model()
