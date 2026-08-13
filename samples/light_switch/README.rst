.. _matter_zigbee_light_switch_sample:

Matter and Zigbee: Light switch
###############################

.. contents::
   :local:
   :depth: 2

This sample demonstrates a combined Matter and Zigbee application on a single SoC.
It implements a Zigbee Dimmer Switch (End Device) and a Matter Dimmer Switch that controls remote Matter lights through the client-side binding cluster.

You can use it together with the Network coordinator and Light bulb samples from the `Zigbee R23 add-on`_ to set up a basic Zigbee network, or pair it directly with the :ref:`matter_zigbee_light_bulb_sample` using Touchlink commissioning.

The ZBOSS stack and OpenThread (used by Matter) share the same 802.15.4 radio, with ownership handed over at commissioning time by the :file:`matter_zigbee_coexistence` library.
The Light switch is a sleepy device in both protocols:

* A Zigbee Sleepy End Device when Zigbee is active.
* An OpenThread Minimal Thread Device (MTD) when Matter is active.

See `Low power operation`_ for the related configuration options.

Requirements
************

The sample supports the following development kits:

.. table-from-sample-yaml::

To test this sample, you also need to program the following samples:

* The `Zigbee Network Coordinator`_ sample from the `Zigbee R23 add-on`_ on one separate device.
* The :ref:`matter_zigbee_light_bulb_sample` sample on one or more separate devices.

Alternatively, if both light bulb and light switch devices support Touchlink, you can use it for Zigbee commissioning without a Zigbee Coordinator.

Overview
********

Once the light switch is successfully commissioned to a Zigbee network, it sends a broadcast message to find devices with the implemented Level Control and On/Off clusters.
The light switch remembers the device network address from the first response.
At this point, you can start using the buttons on the development kit to control the clusters on the newly found devices.

Protocol selection is time-separated and persisted across reboots:

* On first boot, the device starts on the protocol selected by the ``CONFIG_MATTER_ZIGBEE_PROTOCOL_STATE_DEFAULT_PROTOCOL`` Kconfig option (Zigbee by default) and behaves as a Zigbee End Device (Dimmer Switch, sleepy when ``CONFIG_LIGHT_SWITCH_ZIGBEE_SLEEPY`` is enabled) when Zigbee is active.
  In parallel, the Matter stack advertises for commissioning over Bluetooth LE (CHIPoBLE) for the duration configured by ``CONFIG_CHIP_BLE_ADVERTISING_DURATION`` (60 s by default).
* When a Matter commissioner completes commissioning (first CASE session established while Thread is not yet attached), the coexistence layer stops the Zigbee stack, hands the radio over to OpenThread, and persists the selected protocol.
  From this point on, the device operates as a Matter Dimmer Switch that controls remote Matter lights through the client-side binding cluster.
* On subsequent boots, the device resumes the persisted protocol.
  If Matter was selected, the Zigbee stack is skipped entirely and the radio goes directly to OpenThread.
* A Matter factory reset wipes the Zigbee network information when Zigbee was active, resets the persisted protocol to the value selected by :option:`CONFIG_MATTER_ZIGBEE_PROTOCOL_STATE_DEFAULT_PROTOCOL`, wipes Matter commissioning data, and reboots the device in that default state.

.. include:: /includes/protocol_switch.txt

Onboarding data (discriminator, passcode, QR code) is produced by the Matter factory data module (``CONFIG_CHIP_FACTORY_DATA_BUILD``) at build time.

.. _matter_zigbee_light_switch_touchlink:

Touchlink commissioning
=======================

When ``CONFIG_ZIGBEE_TOUCHLINK_INITIATOR`` is enabled, the light switch can act as a Touchlink initiator.
This lets the device commission directly with a nearby Touchlink target (for example, the :ref:`matter_zigbee_light_bulb_sample`) and form a distributed-security Zigbee network without a Zigbee Coordinator.

Short-press **Button 1** during normal operation to start Touchlink commissioning.
If the devices are not paired within 5–7 seconds, short-press **Button 1** once again.
A long press on the same button switches protocol instead; see :ref:`matter_zigbee_limitations`.

.. note::
   Touchlink in the |addon| for the |NCS| is provided as an experimental feature with basic functionality.

.. note::
   Touchlink commissioning does not work when the ``CONFIG_LIGHT_SWITCH_ZIGBEE_SLEEPY`` Kconfig option is enabled.
   The sample disables Sleepy End Device behavior automatically when ``CONFIG_ZIGBEE_TOUCHLINK_INITIATOR`` is selected.
   See :ref:`matter_zigbee_known_issue_touchlink_sleepy`.

.. note::
   Touchlink commissioning does not work reliably when the ``CONFIG_ZIGBEE_FOTA`` is enabled.
   To test Touchlink, disable Zigbee FOTA.
   See :ref:`known_isses`_ and the `Zigbee R23 add-on known issues`_ for more details.

.. _matter_zigbee_limitations:

Limitations
===========

.. include:: /includes/limitations.txt

Configuration
*************

|config|

Source file setup
=================

This sample is split into the following source files:

* The :file:`main` file is the application entry point only.
* The :file:`app_task_zigbee` file manages the application task flow, user input handling, and Zigbee-specific startup and control logic.
* The :file:`app_task_matter` file implements the Matter application task flow: button input, timers, and delegating control actions to bound lighting devices.
* The :file:`light_switch` file implements the light switch application logic and interaction with Zigbee clusters.

.. _matter_zigbee_light_switch_build_variants:

Build variants
==============

The sample provides predefined configuration files in the :file:`samples/light_switch` directory.

.. |sample limitations ref| replace:: :ref:`matter_zigbee_limitations`
.. |sample build variants ref| replace:: :ref:`matter_zigbee_light_switch_build_variants`
.. |sample testing ref| replace:: :ref:`matter_zigbee_light_switch_testing`
.. |sample bt dfu testing ref| replace:: :ref:`matter_zigbee_light_switch_testing_bt_dfu`
.. |sample dir| replace:: samples/light_switch

.. include:: /includes/build_variants.txt

For the board name to use instead of the ``board_target``, see `Programming board names`_.

See `Providing CMake options`_ in the |NCS| documentation for instructions on how to add flags to your build.
For more information about configuration files in the |NCS|, see `Build and configuration system`_ in the |NCS| documentation.

Low power operation
===================

The light switch is a sleepy device:

* When Zigbee is active, you can configure the device as a Sleepy End Device using the ``CONFIG_LIGHT_SWITCH_ZIGBEE_SLEEPY`` Kconfig option (enabled by default).
  This option is not available when ``CONFIG_ZIGBEE_TOUCHLINK_INITIATOR`` is enabled, see :ref:`matter_zigbee_known_issue_touchlink_sleepy`.
* When Matter is active, the device is a Thread Minimal Thread Device with Intermittently Connected Device support.

For low power operation, use the :file:`matter_fota_release.conf` overlay described in :ref:`matter_zigbee_light_switch_build_variants`.
In addition to stripping logging, the console, and the shell, it enables ``CONFIG_PM_DEVICE`` and ``CONFIG_PM_DEVICE_RUNTIME`` to suspend idle peripherals.
For additional power savings, disable all DK LED indications with ``CONFIG_MATTER_ZIGBEE_UI_DISABLE_LEDS``.

.. note::
   While Zigbee is active, the Matter stack advertises for commissioning over Bluetooth LE as described in the `Overview`_ section.
   During Zigbee operation, the power consumption is increased until the advertising duration configured by ``CONFIG_CHIP_BLE_ADVERTISING_DURATION`` elapses.

.. _matter_zigbee_light_switch_user_interface:

User interface
**************

.. include:: /includes/ui_common.txt

Sample-specific (light switch)
==============================

LED 3:
    **Zigbee active:** Solid on when the switch has found a controllable light bulb on the Zigbee network (Match Descriptor succeeded).
    Off before a bulb is found.
    **Matter active:** Off (not used).

Button 1:
    Short press on the light switch (in addition to the protocol-switch long
    press described above):

    * **Zigbee active:** Starts Touchlink commissioning when ``CONFIG_ZIGBEE_TOUCHLINK_INITIATOR`` is enabled (see :ref:`matter_zigbee_light_switch_touchlink`).
    * **Matter active:** Triggers ICD User Active Mode when ``CONFIG_CHIP_ICD_UAT_SUPPORT`` is enabled.

Button 2:
    **Both Matter and Zigbee:** Controls bound lights.
    Available after **LED 3** turns on (Zigbee) or after Matter binding is set up (Matter).

    * **Short press and release:** Toggle on/off.
    * **Press and hold (≥ 500 ms):** Increase the level by 15 every 500 ms while held,
      turning the light on if needed and wrapping to the minimum level after reaching
      the maximum.
      Releasing after dimming does not toggle.

Building and running
********************

.. |sample path| replace:: :file:`samples/light_switch`

|enable_zigbee_before_testing|

.. include:: /includes/build_and_run.txt

.. _matter_zigbee_light_switch_testing:

Testing
=======

After programming the sample to your development kits, complete the following steps to test basic Zigbee operation:

1. Turn on the development kit that runs the Network coordinator sample.

   When **LED 2** turns on, this development kit has become the Coordinator of the Zigbee network.

#. Turn on the development kit that runs the Zigbee-only Light bulb sample.

   When **LED 1** turns on, the light bulb has become a Router inside the network.

   .. note::
        If **LED 1** does not turn on, press **Button 0** on the Coordinator to reopen the network.

#. Turn on the development kit that runs the Light switch sample.

   When **LED 1** turns on, the light switch has become an End Device, connected directly to the Coordinator.

#. Wait until **LED 3** on the light switch node turns on.

   This LED indicates that the light switch found a light bulb to control.

You can now use buttons on the development kit to control the light bulb, as described in :ref:`matter_zigbee_light_switch_user_interface`.

Testing the full Zigbee-to-Matter flow
--------------------------------------

To test the full combined application flow, you need:

* A light switch built as described in :ref:`matter_zigbee_light_switch_build_variants`.
* A Zigbee test setup to verify Zigbee operation before Matter commissioning.
  You can use either the standard setup (a Network coordinator and a Zigbee light bulb) or, alternatively, only a Touchlink-capable light bulb (for example, the :ref:`matter_zigbee_light_bulb_sample`), in which case the Zigbee Coordinator is not needed.
* A Matter controller that can commission a Thread device over Bluetooth LE, for example `CHIP Tool`_ or an ecosystem app (Apple Home, Google Home, Amazon Alexa).
* A Thread Border Router reachable by the Matter fabric.
* Optionally, a Matter light commissioned to the same Thread fabric to be bound to the light switch (for example, the :ref:`matter_zigbee_light_bulb_sample`).

Complete the following steps:

1. Verify Zigbee operation in one of the following ways:

   * Follow the standard `Testing`_ procedure with a Zigbee Network coordinator and a Zigbee light bulb.
   * Or, skip the Zigbee Coordinator and pair the light switch directly with a Touchlink-capable light bulb:

     a. Power the light bulb (Touchlink target).
     #. Power the light switch and press the Touchlink button (see :ref:`matter_zigbee_light_switch_touchlink`).
        The two devices form a distributed-security Zigbee network and the light switch finds the bulb to control, without a Zigbee Coordinator on the network.

   While the device is still a Zigbee End Device, it also advertises for Matter commissioning over Bluetooth LE if :option:`CONFIG_MATTER_ZIGBEE_COEXISTENCE_BT_ADV_WHILE_ZIGBEE` is enabled.
#. Optionally, long-press Button 1 for :option:`CONFIG_MATTER_ZIGBEE_COEXISTENCE_SWITCH_BUTTON_PRESS_TIME_SECONDS` to switch to Matter.
   The Zigbee stack is stopped and the radio is handed to OpenThread.
   Skip the next step if you use this path and Matter was already commissioned in a previous session.
#. Commission the device using the onboarding payload produced by the Matter factory data build (QR code or manual pairing code).
   After the Matter CASE session is established, the light switch hands the radio over to Thread and stops participating in the Zigbee network.
#. Bind the light switch to a Matter light (for example, with ``chip-tool binding write binding …``) and use the dimmer button to toggle or dim the bound light over Thread.
#. To return the device to Zigbee operation, use one of the following:

   * Long-press Button 1 for :option:`CONFIG_MATTER_ZIGBEE_COEXISTENCE_SWITCH_BUTTON_PRESS_TIME_SECONDS`.
     The device reboots and resumes as a Zigbee End Device.
   * Or trigger a Matter factory reset from the controller (for example, ``chip-tool pairing unpair …``).
     The device reboots as a fresh Zigbee End Device with Matter Bluetooth LE advertising active again, and Matter storage is cleared.

.. _matter_zigbee_light_switch_testing_bt_dfu:

Testing DFU over Bluetooth SMP
------------------------------

.. |bt device name| replace:: MatterZigbeeSw
.. |matter mcumgr smp upload| replace:: mcumgr --conntype ble --hci 0 --connstring peer_name='MatterZigbeeSw' image upload build/light_switch/zephyr/zephyr.signed.bin -n 0 -w 1

.. include:: /includes/bt_dfu_testing.txt

Sample output
-------------

You can observe the sample logging output through a serial port after connecting with a terminal emulator (for example, nRF Connect Serial Terminal).
See `Testing and optimization`_ in the |NCS| documentation for the required settings and steps.

Dependencies
************

This sample uses the following |NCS| libraries and components:

* The Matter stack (``CONFIG_CHIP``) shipped with the |NCS|, including the Binding and Identify clusters and the Matter factory data module.
* OpenThread (used by Matter on 802.15.4) and the `SoftDevice Controller`_ (used for CHIPoBLE commissioning).
* The :file:`matter_zigbee_coexistence` and :file:`matter_zigbee_protocol_state` libraries, which orchestrate the 802.15.4 radio hand-over and persist the selected protocol.
* The :file:`nrf_802154_callbacks_dispatcher` (``CONFIG_NRF_802154_CALLBACKS_DISPATCHER``) with runtime re-init (``CONFIG_NRF_802154_DRV_REINIT_ENABLED``).
* The `Zigbee R23 add-on`_ for ZBOSS stack support, Zigbee application utilities, and error handling.
* `DK Buttons and LEDs`_
