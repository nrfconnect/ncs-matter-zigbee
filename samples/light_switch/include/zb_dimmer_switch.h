/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef ZB_DIMMER_SWITCH_H
#define ZB_DIMMER_SWITCH_H 1

/**
 *  @defgroup ZB_DEFINE_DEVICE_DIMMER_SWITCH Dimmer Switch
 *  @{
 *  @details
 *      - @ref ZB_ZCL_BASIC \n
 *      - @ref ZB_ZCL_IDENTIFY \n
 *      - @ref ZB_ZCL_SCENES \n
 *      - @ref ZB_ZCL_GROUPS \n
 *      - @ref ZB_ZCL_ON_OFF \n
 *      - @ref ZB_ZCL_LEVEL_CONTROL
 */

/**
 * @brief Declare cluster list for Dimmer Switch device
 * @param cluster_list_name - cluster list variable name
 * @param basic_server_attr_list - attribute list for Basic cluster (server role)
 * @param identify_client_attr_list - attribute list for Identify cluster (client role)
 * @param identify_server_attr_list - attribute list for Identify cluster (server role)
 * @param scenes_client_attr_list - attribute list for Scenes cluster (client role)
 * @param groups_client_attr_list - attribute list for Groups cluster (client role)
 * @param on_off_client_attr_list - attribute list for On/Off cluster (client role)
 * @param level_control_client_attr_list - attribute list for Level Control cluster (client role)
 */
#define ZB_DECLARE_DIMMER_SWITCH_CLUSTER_LIST(cluster_list_name, basic_server_attr_list, identify_client_attr_list,    \
					      identify_server_attr_list, scenes_client_attr_list,                      \
					      groups_client_attr_list, on_off_client_attr_list,                        \
					      level_control_client_attr_list)                                          \
	zb_zcl_cluster_desc_t cluster_list_name[] = {                                                                  \
		ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_BASIC, ZB_ZCL_ARRAY_SIZE(basic_server_attr_list, zb_zcl_attr_t), \
				    (basic_server_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_MANUF_CODE_INVALID),  \
		ZB_ZCL_CLUSTER_DESC(                                                                                   \
			ZB_ZCL_CLUSTER_ID_IDENTIFY, ZB_ZCL_ARRAY_SIZE(identify_server_attr_list, zb_zcl_attr_t),       \
			(identify_server_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_MANUF_CODE_INVALID),           \
		ZB_ZCL_CLUSTER_DESC(                                                                                   \
			ZB_ZCL_CLUSTER_ID_IDENTIFY, ZB_ZCL_ARRAY_SIZE(identify_client_attr_list, zb_zcl_attr_t),       \
			(identify_client_attr_list), ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_MANUF_CODE_INVALID),           \
		ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_SCENES,                                                          \
				    ZB_ZCL_ARRAY_SIZE(scenes_client_attr_list, zb_zcl_attr_t),                         \
				    (scenes_client_attr_list), ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_MANUF_CODE_INVALID), \
		ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_GROUPS,                                                          \
				    ZB_ZCL_ARRAY_SIZE(groups_client_attr_list, zb_zcl_attr_t),                         \
				    (groups_client_attr_list), ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_MANUF_CODE_INVALID), \
		ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_ON_OFF,                                                          \
				    ZB_ZCL_ARRAY_SIZE(on_off_client_attr_list, zb_zcl_attr_t),                         \
				    (on_off_client_attr_list), ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_MANUF_CODE_INVALID), \
		ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,                                                   \
				    ZB_ZCL_ARRAY_SIZE(level_control_client_attr_list, zb_zcl_attr_t),                  \
				    (level_control_client_attr_list), ZB_ZCL_CLUSTER_CLIENT_ROLE,                      \
				    ZB_ZCL_MANUF_CODE_INVALID)                                                         \
	}

/** @} */

#endif /* ZB_DIMMER_SWITCH_H */
