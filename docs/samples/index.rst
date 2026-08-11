.. _matter_zigbee_samples:

Samples
#######

The |addon| for the |NCS| provides the following samples that demonstrate a combined Matter and Zigbee application on a single SoC.
All samples are placed in the :file:`samples` directory in the |addon| repository.

.. list-table::
   :widths: auto
   :header-rows: 1

   * - Sample
     - Zigbee role
     - Zigbee device type
     - Matter device type
     - Thread role
     - Zigbee Touchlink
   * - :ref:`matter_zigbee_light_switch_sample`
     - Zigbee Sleepy End Device
     - Zigbee Dimmer Switch
     - Matter Dimmer Switch
     - Sleepy End Device
     - Initiator (optional)
   * - :ref:`matter_zigbee_light_bulb_sample`
     - Zigbee Router
     - Zigbee Dimmable Light
     - Matter Dimmable Light
     - Full Thread Device
     - Target (optional)

The samples share the same runtime model: the ZBOSS stack and OpenThread (used by Matter) time-share the 802.15.4 radio, with ownership handed over at commissioning time by the :file:`matter_zigbee_coexistence` library.
See :ref:`matter_zigbee_architectures` for the platform overview.

.. toctree::
   :maxdepth: 1
   :caption: Subpages
   :glob:

   */README
