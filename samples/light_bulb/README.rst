.. _matter_zigbee_light_bulb_sample:

Matter and Zigbee: Light bulb
#############################

.. contents::
   :local:
   :depth: 2

This sample demonstrates a combined Matter and Zigbee application on a single SoC.
It implements a Zigbee Dimmable Light (Router) and a Matter Dimmable Light endpoint that serves the On/Off and Level Control clusters and drives the same LED through PWM.

You can use this sample with the Network coordinator and Light switch samples from the `Zigbee R23 add-on`_ to set up a basic Zigbee network, or pair it directly with the :ref:`matter_zigbee_light_switch_sample` using Touchlink commissioning.

The ZBOSS stack and OpenThread (used by Matter) share the same 802.15.4 radio between the ZBOSS stack and OpenThread.
For Thread networking in Matter mode, the light bulb acts as an OpenThread Full Thread Device (FTD).

Requirements
************

The sample supports the following development kits:

.. table-from-sample-yaml::

To test this sample, you also need to program the following samples:

* The `Zigbee Network Coordinator`_ sample from the `Zigbee R23 add-on`_ on one separate device.
* The :ref:`matter_zigbee_light_switch_sample` sample on one or more separate devices.

Alternatively, if both light bulb and light switch devices support Touchlink, you can use it for Zigbee commissioning without a Zigbee Coordinator.

Overview
********

The sample-specific behavior is:

* On first boot, the device starts on the protocol selected by :option:`CONFIG_MATTER_ZIGBEE_PROTOCOL_STATE_DEFAULT_PROTOCOL` (Zigbee by default).
  When Zigbee is active, it is a standard Zigbee Router exposing the Dimmable Light device, while the Matter stack advertises for commissioning over Bluetooth LE (CHIPoBLE).
* After successful Matter commissioning, the device is converted to a Matter Dimmable Light endpoint that serves the On/Off and Level Control clusters and drives the same LED through PWM.
  It can then be controlled by any device on the Matter fabric bound to it (for example, the :ref:`matter_zigbee_light_switch_sample`).
* On subsequent boots, the device resumes the persisted protocol.
  When Matter was selected, it starts directly as a Matter Dimmable Light.
  A Matter factory reset resets the persisted protocol to the configured default and wipes Matter commissioning data.

The radio hand-over, persistent protocol state, user-triggered protocol switch, factory-reset behavior and onboarding-data generation are common to both combined samples and are described in detail in the :ref:`matter_zigbee_light_switch_sample` documentation.

.. _matter_zigbee_light_bulb_touchlink:

Touchlink commissioning
=======================

When ``CONFIG_ZIGBEE_TOUCHLINK_TARGET`` is enabled, the light bulb acts as a Touchlink target.
This lets a nearby Touchlink initiator (for example, the :ref:`matter_zigbee_light_switch_sample`) commission the device directly and form a distributed-security Zigbee network without a Zigbee Coordinator.
No dedicated button action is required on the light bulb to enable this role.
The device responds to Touchlink scans whenever it is powered and not yet joined to a network.

.. note::
   Touchlink in the |addon| for the |NCS| is provided as an experimental feature with basic functionality.

.. note::
   Touchlink commissioning does not work reliably when the ``CONFIG_ZIGBEE_FOTA`` is enabled.
   To test Touchlink, disable Zigbee FOTA.
   See :ref:`known_isses`_ and the `Zigbee R32 add-on known issues`_ for more details.

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
  It also implements the dimmable light device behavior, including cluster handling and PWM-based brightness control.
* The :file:`app_task_matter` file implements the Matter application task flow, button input, and On/Off and Level Control cluster state for the dimmable light endpoint.

.. _matter_zigbee_light_bulb_build_variants:

Build variants
==============

The sample provides predefined configuration files in the :file:`samples/light_bulb` directory.

.. |sample limitations ref| replace:: :ref:`matter_zigbee_limitations`
.. |sample build variants ref| replace:: :ref:`matter_zigbee_light_bulb_build_variants`
.. |sample testing ref| replace:: :ref:`matter_zigbee_light_bulb_testing`
.. |sample bt dfu testing ref| replace:: :ref:`matter_zigbee_light_bulb_testing_bt_dfu`
.. |sample dir| replace:: samples/light_bulb

.. include:: /includes/build_variants.txt

For the board name to use instead of the ``board_target``, see `Programming board names`_.

See `Providing CMake options`_ in the |NCS| documentation for instructions on how to add flags to your build.

.. _matter_zigbee_light_bulb_user_interface:

User interface
**************

.. include:: /includes/ui_common.txt

Sample-specific (light bulb)
============================

LED 3:
    **Both Matter and Zigbee:** Main light output (PWM when ``CONFIG_PWM`` is enabled).
    Reflects on/off and brightness from Zigbee Level Control / On/Off clusters or from Matter On/Off and Level Control clusters.

Button 2:
    **Matter active:** Short press toggles locally the main light (LED 3).
    **Zigbee active:** Not used for light control (use the light switch or another Zigbee controller).

Building and running
********************

.. |sample path| replace:: :file:`samples/light_bulb`

|enable_zigbee_before_testing|

.. include:: /includes/build_and_run.txt

.. _matter_zigbee_light_bulb_testing:

Testing
=======

After programming the sample to your development kits, complete the following steps to test basic Zigbee operation:

1. Turn on the development kit that runs the Network coordinator sample.

   When **LED 2** turns on, this development kit has become the Coordinator of the Zigbee network and the network is established.

#. Turn on the development kit that runs the Light bulb sample.

   When **LED 1** turns on, the light bulb has become a Router inside the network.

   .. note::
      If **LED 1** does not turn on, press **Button 0** on the Coordinator to reopen the network.

#. Turn on the development kit that runs the Light switch sample.

   When **LED 1** turns on, the light switch has become an End Device, connected directly to the Coordinator.

#. Wait until **LED 3** on the development kit that runs the Light switch sample turns on.

   This LED indicates that the switch found a light bulb to control.

#. Use the buttons on the development kit that runs the :ref:`matter_zigbee_light_switch_sample` to control the light bulb.

   The result of using the buttons is reflected on the light bulb's **LED 3**.

Testing the full Zigbee-to-Matter flow
--------------------------------------

To test the full combined application flow, you need:

* A light bulb built as described in :ref:`matter_zigbee_light_bulb_build_variants`.
* A Zigbee test setup to verify Zigbee operation before Matter commissioning.
  You can use either the standard setup (a Network coordinator and a Zigbee light switch) or, alternatively, only a Touchlink-capable light switch (for example, the :ref:`matter_zigbee_light_switch_sample`), in which case the Zigbee Coordinator is not needed.
* A Matter controller that can commission a Thread device over Bluetooth LE, for example `CHIP Tool`_ or an ecosystem app (Apple Home, Google Home, Amazon Alexa).
* A Thread Border Router reachable by the Matter fabric.
* Optionally, a Matter switch on the same Thread fabric to be bound to the light bulb (for example, the :ref:`matter_zigbee_light_switch_sample`).

Complete the following steps:

1. Verify Zigbee operation in one of the following ways:

   * Follow the standard `Testing`_ procedure with a Zigbee Network coordinator and a Zigbee light switch.
   * Or, skip the Zigbee Coordinator and pair the light bulb directly with a Touchlink-capable light switch:

     a. Power the light bulb (Touchlink target).
     #. Power the light switch and press its Touchlink button (see :ref:`matter_zigbee_light_switch_touchlink`).
        The two devices form a distributed-security Zigbee network without a Zigbee Coordinator, and the light switch finds and controls the light bulb.

   While the device is still a Zigbee Router, it also advertises for Matter commissioning over Bluetooth LE.
#. Optionally, long-press Button 1 for :option:`CONFIG_MATTER_ZIGBEE_COEXISTENCE_SWITCH_BUTTON_PRESS_TIME_SECONDS` to switch to Matter.
   The Zigbee stack is stopped and the radio is handed to OpenThread.
   Skip the next step if you use this path and Matter was already commissioned in a previous session.
#. Commission the device using the onboarding payload produced by the Matter factory data build (QR code or manual pairing code).
   After the Matter CASE session is established, the light bulb hands the radio over to Thread and stops participating in the Zigbee network.
#. Drive the light bulb from a Matter peer:

   * From the controller directly, with ``chip-tool onoff toggle …`` or ``chip-tool levelcontrol move-to-level …``.
   * Or by binding a Matter switch to the light bulb and using the switch's dimmer button.

#. To return the device to Zigbee operation, use one of the following:

   * Long-press Button 1 for :option:`CONFIG_MATTER_ZIGBEE_COEXISTENCE_SWITCH_BUTTON_PRESS_TIME_SECONDS`.
     The device reboots and resumes as a Zigbee Router.
   * Or trigger a Matter factory reset from the controller (for example, ``chip-tool pairing unpair …``).
     The device reboots as a fresh Zigbee Router with Matter Bluetooth LE advertising active again, and Matter storage is cleared.

.. _matter_zigbee_light_bulb_testing_bt_dfu:

Testing DFU over Bluetooth SMP
------------------------------

.. |bt device name| replace:: MatterZigbeeLb
.. |matter mcumgr smp upload| replace:: mcumgr --conntype ble --hci 0 --connstring peer_name='MatterZigbeeLb' image upload build/light_bulb/zephyr/zephyr.signed.bin -n 0 -w 1

.. include:: /includes/bt_dfu_testing.txt

Dependencies
************

This sample uses the following |NCS| libraries and components:

* The Matter stack (``CONFIG_CHIP``) shipped with the |NCS|, including the On/Off, Level Control and Identify clusters and the Matter factory data module.
* OpenThread (used by Matter on 802.15.4) and the `SoftDevice Controller`_ (used for CHIPoBLE commissioning).
* The :file:`matter_zigbee_coexistence` and :file:`matter_zigbee_protocol_state` libraries, which orchestrate the 802.15.4 radio hand-over and persist the selected protocol.
* The :file:`nrf_802154_callbacks_dispatcher` (``CONFIG_NRF_802154_CALLBACKS_DISPATCHER``) with runtime re-init (``CONFIG_NRF_802154_DRV_REINIT_ENABLED``).
* The `Zigbee R23 add-on`_ for ZBOSS stack support, Zigbee application utilities, and error handling.
* `DK Buttons and LEDs`_
* `Pulse Width Modulation (PWM)`_
