.. _matter_zigbee_memory:

Memory requirements
###################

.. contents::
   :local:
   :depth: 2

This page provides information about the amount of flash memory and RAM that is required by the :ref:`matter_zigbee_samples`.
Use it to check if your application has enough space for a given configuration.

Unless stated otherwise, the default :file:`prj.conf` was used for debug builds, and :file:`release.conf` was added as an extra overlay for release builds.
Sample variants match the Twister scenarios defined in each sample's :file:`sample.yaml` file.

Memory layout in DTS
********************

The :ref:`matter_zigbee_samples` define flash partitions in Devicetree (DTS).
The |addon| ships ready-made partition files under :file:`dts/` in the |addon| repository.
Use them as-is, or copy and adjust them for your board.

Each layout reserves the end of internal flash for:

* ``factory_data_partition`` - Matter factory data
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
     - | nRF54LM20 DK (DK only)
     - Default layout with all partitions on internal flash.

   * - External flash (MCUboot secondary slot)
     - :file:`dts/<board>_partitions_ext_flash.dtsi`
     - | nRF54L15 DK (DK only)
       | nRF54LM20 DK (DK only)
     - Same as the internal layout, but places ``slot1_partition`` on external flash and enlarges ``slot0_partition``.
       Build with ``SNIPPET=ext_flash``.

To add an overlay file to your application, add an ``#include`` line to :file:`app.overlay` or to a board overlay in :file:`boards/`.

See an example for the nRF54LM20 DK with external-flash FOTA in the :file:`boards/nrf54lm20dk_nrf54lm20a_cpuapp.overlay` file:

.. code-block:: dts

   #include <nrf54lm20dk_cpuapp_partitions_ext_flash.dtsi>

RAM and flash memory requirements
*********************************

RAM and flash memory requirement values differ depending on the programmed sample.

The following tables and bar charts list memory requirement values for the combined Matter and Zigbee samples.
Values include the ZBOSS stack, Matter stack, MCUboot bootloader, and ZBOSS non-volatile regions where applicable.
Each row corresponds to a Twister build scenario from :file:`samples/light_bulb/sample.yaml` or :file:`samples/light_switch/sample.yaml`.
Scenarios not supported on a given board target are omitted from that board's view.

Memory layout is taken from the DTS files used by each sample variant, while memory usage is taken from the build output.

Values are provided in kilobytes (KB).
Unsupported sample variants are omitted from each board view.
``--`` indicates that a partition is not used by the sample configuration.

Table columns are grouped by internal NVM, external NVM (when used), and RAM.
Application, MCUboot, upgrade slot, and RAM cells show used and free space separated by ``/``.
Other NVM columns list the reserved partition size for that region.

.. tabs::

   .. group-tab:: Charts

      .. tabs::

         .. group-tab:: nRF54L15

            Memory requirements for samples running on the `nrf54l15dk`_.

            .. memory-board::
               :board: nrf54l15

         .. group-tab:: nRF54LM20

            Memory requirements for samples running on the `nrf54lm20dk`_ with the ``nrf54lm20dk/nrf54lm20a/cpuapp`` or ``nrf54lm20dk/nrf54lm20b/cpuapp`` board targets.

            .. memory-board::
               :board: nrf54lm20

   .. group-tab:: Tables

      .. tabs::

         .. group-tab:: nRF54L15

            The following table lists memory requirements for samples running on the `nrf54l15dk`_.

            .. memory-table::
               :board: nrf54l15

         .. group-tab:: nRF54LM20

            The following table lists memory requirements for samples running on the `nrf54lm20dk`_ with the ``nrf54lm20dk/nrf54lm20a/cpuapp`` or ``nrf54lm20dk/nrf54lm20b/cpuapp`` board targets.

            .. memory-table::
               :board: nrf54lm20
