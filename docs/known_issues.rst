.. _known_issues:

Known issues
############

.. contents::
   :local:
   :depth: 2

Known issues listed on this page are valid for the current state of development.
Items can have one or both of the following entries:

* **Affected platforms:**

  If a known issue does not have any specific platforms listed, it is valid for all hardware platforms.

* **Workaround:**

  Some known issues have a workaround.
  Sometimes, they are discovered later and added over time.

The |addon| inherits the known issues related to the Zigbee and Matter stacks from the `Zigbee R23 add-on known issues`_ and the `nRF Connect SDK known issues`_.

List of known issues
********************

.. rst-class:: v1-0-0

KRKNWK-22334: Zigbee Touchlink commissioning does not work reliably when Zigbee FOTA is enabled
  When Zigbee Firmware-Over-the-Air (FOTA) is enabled, Zigbee Touchlink commissioning does not work reliably.
  This issue is not observed when Zigbee FOTA is disabled.

.. rst-class:: v1-0-0

KRKNWK-22335: Zigbee Touchlink initiator is not always able to pair after single button activation
  In some cases, pressing the dedicated button does not pair the :ref:`matter_zigbee_light_switch_sample`, acting as Touchlink initiator, with the :ref:`matter_zigbee_light_bulb_sample`, acting as Touchlink target.

  **Workaround:** If the devices are not paired within 5–7 seconds after pressing the button, short-press the button once again on the light switch.
  
.. _matter_zigbee_known_issue_touchlink_sleepy:

.. rst-class:: v1-0-0

KRKNWK-22343: Zigbee Touchlink initiator does not work when Zigbee Sleepy End Device behavior is enabled
  Touchlink commissioning uses inter-PAN frames that require the Zigbee End Device to keep its radio receiver active.
  Sleepy End Device behavior disables the receiver between polls to the parent, so Touchlink pairing fails when it is enabled.

  **Workaround:** Do not enable both options at the same time.
  In the :ref:`matter_zigbee_light_switch_sample`, ``CONFIG_LIGHT_SWITCH_ZIGBEE_SLEEPY`` depends on ``!CONFIG_ZIGBEE_TOUCHLINK_INITIATOR`` and is unavailable when Touchlink initiator mode is selected.
  Use Sleepy End Device behavior for low-power operation without Touchlink, or disable sleepy mode when building with Touchlink initiator support.
