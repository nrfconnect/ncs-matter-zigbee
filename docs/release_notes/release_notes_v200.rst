.. _release_notes_v200:

Release notes for |addon| v2.0.0
################################

.. contents::
   :local:
   :depth: 2

This page tracks changes and updates as compared to the |addon| v1.0.0 release.
For the list of potential issues, see the :ref:`known_issues` page.

Changelog
*********

* Updated:

  * The Matter integration to use the `Matter add-on`_ v1.0.1 with Matter v1.6.0.
  * The Zigbee integration to use the `Zigbee R23 add-on`_ v2.0.0, which provides the Zigbee 4.0.
  * The Matter and Zigbee samples to use the `Matter add-on`_ v1.0.1 integration.
  * The :ref:`matter_zigbee_light_bulb_sample` and :ref:`matter_zigbee_light_switch_sample` samples to use the ZBOSS 5 callback API with the unified ``zb_cb_param_t`` callback argument.
  * The Zigbee FOTA configuration of the :ref:`matter_zigbee_light_bulb_sample` and :ref:`matter_zigbee_light_switch_sample` samples to resume interrupted downloads after a device reset.

* Removed the ``CONFIG_MATTER_ZIGBEE_COEXISTENCE_BT_ADV_WHILE_ZIGBEE`` Kconfig option.
  The Zigbee worker thread now always starts after the Matter server is initialized, and the ``CONFIG_CHIP_ENABLE_PAIRING_AUTOSTART`` Kconfig option controls Bluetooth LE commissioning advertising.
