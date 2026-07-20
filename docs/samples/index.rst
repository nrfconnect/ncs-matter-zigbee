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
     - Matter role
     - Touchlink
   * - :ref:`matter_zigbee_light_switch_sample`
     - End Device (Dimmer Switch)
     - Dimmer Switch (binding client)
     - Initiator (optional)
   * - :ref:`matter_zigbee_light_bulb_sample`
     - Router (Dimmable Light)
     - Dimmable Light
     - Target (optional)

Both samples share the same runtime model: the ZBOSS stack and OpenThread (used by Matter) time-share the 802.15.4 radio, with ownership handed over at commissioning time by the :file:`matter_zigbee_coexistence` library.
See :ref:`matter_zigbee_platform_design` for the platform overview and :ref:`matter_zigbee_light_switch_sample` for a detailed description of protocol selection, factory reset, and onboarding data generation.

.. toctree::
   :maxdepth: 1
   :caption: Subpages
   :glob:

   */README
