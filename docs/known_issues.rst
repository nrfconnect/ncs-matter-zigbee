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

The |addon| inherits the known issues related to the Zigbee and Matter stacks from the `Zigbee R32 add-on known issues`_ and the `nRF Connect SDK known issues`_.

List of known issues
********************

.. rst-class:: v1-0-0

KRKNWK-22334: Zigbee Touchlink commissioning does not work reliably when Zigbee FOTA is enabled
  When Zigbee FOTA is enabled, Zigbee Touchlink commissioning does not work reliably.
  This issue is not observed when Zigbee FOTA is disabled.

.. rst-class:: v1-0-0

KRKNWK-22335: Zigbee Touchlink initiator is not always able to pair after single button activation
  In some cases, the :ref:`matter_zigbee_light_switch_sample`, acting as Touchlink initiator, does not pair with the :ref:`matter_zigbee_light_bulb_sample`, acting as Touchlink target, after activating the dedicated button.

  **Workaround:** If the devices are not paired within 5–7 seconds after pressing the button, short-press the button once again on the light switch.
  