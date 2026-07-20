.. _release_notes_vTBA:

Release notes for |addon| v1.0.0
################################

.. contents::
   :local:
   :depth: 2

This page tracks changes and updates as compared to the latest official release.
For more information refer to the following section.

For the list of potential issues, see the :ref:`known_issues` page.

Changelog
*********

This is the first `supported <Software maturity levels_>`_ release of the |addon|.

* Added:

  * The integration of the `Zigbee R23 add-on`_ v1.4.0 and Matter v1.5.0 through the |NCS| v3.4.0.
  * Support for Matter and Zigbee combined solution on the nRF54L15, nRF54LM20A and nRF54LM20B SoCs.
    A default protocol is selected on first boot and the application or user can trigger a protocol switch which persists across reboots.
    While on Zigbee, Matter advertising can be enabled and the device can be commissioned by a Matter commissioner, performing protocol switch to Matter after successful commissioning.
  * The :ref:`matter_zigbee_light_switch_sample` and :ref:`matter_zigbee_light_bulb_sample` samples.
  * A snippet to support external flash usage for the samples.
  * Support for DFU over Bluetooth LE SMP. This can be optionally enabled for the samples.
