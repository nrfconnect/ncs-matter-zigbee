#
# Copyright (c) 2026 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# Global SNIPPET/SB_SNIPPET is applied to every sysbuild image. Some addon
# snippets target the application only and need a different MCUboot counterpart.
# This hook remaps those pairs on the mcuboot image so builds can pass a single
# -DSNIPPET=<app_snippet>. Extend the list below as <app_snippet> <mcuboot_snippet>.
function(${SYSBUILD_CURRENT_MODULE_NAME}_pre_cmake)
  if(NOT SB_CONFIG_BOOTLOADER_MCUBOOT)
    return()
  endif()

  zephyr_get(SB_SNIPPET)
  if(SB_SNIPPET)
    set(global_snippets ${SB_SNIPPET})
  else()
    zephyr_get(SNIPPET)
    set(global_snippets ${SNIPPET})
  endif()

  # Extend this list as needed.
  set(app_mcuboot_snippet_pairs
    ext_flash mcuboot_ext_flash
  )

  set(mcuboot_snippets ${global_snippets})
  set(remapped FALSE)
  list(LENGTH app_mcuboot_snippet_pairs pair_count)
  math(EXPR last_pair_idx "${pair_count} - 2")
  foreach(idx RANGE 0 ${last_pair_idx} 2)
    list(GET app_mcuboot_snippet_pairs ${idx} app_snippet)
    math(EXPR mcuboot_idx "${idx} + 1")
    list(GET app_mcuboot_snippet_pairs ${mcuboot_idx} mcuboot_snippet)

    if(app_snippet IN_LIST global_snippets)
      set(remapped TRUE)
      list(REMOVE_ITEM mcuboot_snippets ${app_snippet})
      if(NOT mcuboot_snippet IN_LIST mcuboot_snippets)
        list(APPEND mcuboot_snippets ${mcuboot_snippet})
      endif()
    endif()
  endforeach()

  if(remapped)
    set(mcuboot_SNIPPET ${mcuboot_snippets} CACHE STRING "" FORCE)
  endif()
endfunction()
