.. _matter_zigbee_memory:

Memory requirements
###################

.. contents::
   :local:
   :depth: 2

This page provides information about the amount of flash memory and RAM that is required by the :ref:`matter_zigbee_samples`, as well as stack memory requirements for ``main`` and ``zboss`` threads.
Use it to check if your application has enough space for a given configuration.

Unless stated otherwise, the default :file:`prj.conf` was used for debug builds, and :file:`matter_fota_release.conf` was added as an extra overlay for release builds.

Memory layout in DTS
********************

The :ref:`matter_zigbee_samples` define flash partitions in Devicetree (DTS).
The |addon| ships ready-made partition files under :file:`dts/` in the |addon| repository.
Use them as-is, or copy and adjust them for your board.

Each layout reserves the end of internal flash for:

* ``storage_partition`` - Zephyr settings storage
* ``zboss_nvram`` - ZBOSS non-volatile data
* ``zboss-product-config`` - ZBOSS product configuration

The following table lists the available files, which targets they cover, and when to use each variant:

.. list-table:: DTS partition layout variants
   :header-rows: 1

   * - Variant
     - Path
     - Targets
     - When to use

   * - Internal flash
     - :file:`dts/<board>_partitions.dtsi`
     - | nRF54L15 DK (DK only)
       | nRF54LM20 DK (DK only)
     - Default layout with ``boot_partition``, ``slot1_partition``, and ``factory_data_partition`` on internal flash.

   * - External flash (MCUboot secondary slot)
     - :file:`dts/<board>_partitions_ext_flash.dtsi`
     - | nRF54L15 DK (DK only)
       | nRF54LM20 DK (DK only)
     - Same as the internal layout, but places ``slot1_partition`` on external flash and enlarges ``slot0_partition``.
       Build with ``SNIPPET=matter_fota_ext_flash``.

To add an overlay file to your application, add an ``#include`` line to :file:`app.overlay` or to a board overlay in :file:`boards/`.

See an example for the nRF54LM20 DK with external-flash FOTA in the :file:`boards/nrf54lm20dk_nrf54lm20a_cpuapp.overlay` file:

.. code-block:: dts

   #include <nrf54lm20dk_cpuapp_partitions_ext_flash.dtsi>

Sample memory requirements
**************************

The following tables list memory requirements for the combined Matter and Zigbee samples.
Values include the ZBOSS stack, Matter stack, MCUboot bootloader, and ZBOSS non-volatile regions where applicable.

.. tabs::

   .. group-tab:: nRF54L15

      The following table lists memory requirements for samples running on the `nrf54l15dk`_.

      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | Sample                                                                                                                 | ROM, ZBOSS stack + App   | ROM, MCUboot bootloader   | ROM, ZBOSS non-volatile memory   | ROM, ZBOSS product config   | Total ROM   | RAM, ZBOSS stack + App   | Total RAM   |
      +========================================================================================================================+==========================+===========================+==================================+=============================+=============+==========================+=============+
      | :ref:`Light bulb <matter_zigbee_light_bulb_sample>` (debug)                                                            | 1055                     | 56                        | 32                               | 4                           | 1147        | 228                      | 228         |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | :ref:`Light bulb <matter_zigbee_light_bulb_sample>` (release)                                                          | 962                      | 56                        | 32                               | 4                           | 1054        | 228                      | 228         |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | :ref:`Light switch <matter_zigbee_light_switch_sample>` (debug)                                                        | 927                      | 56                        | 32                               | 4                           | 1019        | 200                      | 200         |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | :ref:`Light switch <matter_zigbee_light_switch_sample>` (release)                                                      | 834                      | 56                        | 32                               | 4                           | 926         | 200                      | 200         |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+

   .. group-tab:: nRF54LM20

      The following table lists memory requirements for samples running on the `nrf54lm20dk`_ with the ``nrf54lm20dk/nrf54lm20a/cpuapp`` or ``nrf54lm20dk/nrf54lm20b/cpuapp`` board targets.

      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | Sample                                                                                                                 | ROM, ZBOSS stack + App   | ROM, MCUboot bootloader   | ROM, ZBOSS non-volatile memory   | ROM, ZBOSS product config   | Total ROM   | RAM, ZBOSS stack + App   | Total RAM   |
      +========================================================================================================================+==========================+===========================+==================================+=============================+=============+==========================+=============+
      | :ref:`Light bulb <matter_zigbee_light_bulb_sample>` (debug)                                                            | 1070                     | 42                        | 32                               | 4                           | 1148        | 228                      | 228         |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | :ref:`Light bulb <matter_zigbee_light_bulb_sample>` (release)                                                          | 979                      | 42                        | 32                               | 4                           | 1057        | 228                      | 228         |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | :ref:`Light switch <matter_zigbee_light_switch_sample>` (debug)                                                        | 940                      | 42                        | 32                               | 4                           | 1018        | 201                      | 201         |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
      | :ref:`Light switch <matter_zigbee_light_switch_sample>` (release)                                                      | 849                      | 42                        | 32                               | 4                           | 927         | 201                      | 201         |
      +------------------------------------------------------------------------------------------------------------------------+--------------------------+---------------------------+----------------------------------+-----------------------------+-------------+--------------------------+-------------+
