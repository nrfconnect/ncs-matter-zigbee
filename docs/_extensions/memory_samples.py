"""
Copyright (c) 2026 Nordic Semiconductor ASA

SPDX-License-Identifier: LicenseRef-Nordic-5-Clause

Discover memory documentation variants from Twister sample.yaml files.
"""

from __future__ import annotations

from pathlib import Path
from typing import Any

import yaml

SAMPLES_DIR = Path(__file__).resolve().parents[2] / "samples"

PLATFORM_TO_BOARD = {
    "nrf54l15dk/nrf54l15/cpuapp": "nrf54l15",
    "nrf54lm20dk/nrf54lm20a/cpuapp": "nrf54lm20",
    "nrf54lm20dk/nrf54lm20b/cpuapp": "nrf54lm20",
}

SAMPLE_DIR_REFS: dict[str, tuple[str, str]] = {
    "light_bulb": ("Light bulb", "matter_zigbee_light_bulb_sample"),
    "light_switch": ("Light switch", "matter_zigbee_light_switch_sample"),
}


def _platforms(test: dict[str, Any], common: dict[str, Any]) -> list[str]:
    platforms = test.get("platform_allow") or common.get("platform_allow") or []
    if isinstance(platforms, str):
        return [platforms]
    return list(platforms)


def _extra_args(test: dict[str, Any], common: dict[str, Any]) -> list[str]:
    args: list[str] = []
    args.extend(str(arg) for arg in common.get("extra_args") or [])
    args.extend(str(arg) for arg in test.get("extra_args") or [])
    return args


def _layout(extra_args: list[str]) -> str:
    if any("ext_flash" in arg for arg in extra_args):
        return "ext_flash"
    return "base"


def _suffix(test_id: str, extra_args: list[str]) -> str:
    joined = " ".join(extra_args)
    if "release.conf" in joined:
        return " (release)"
    if test_id.endswith(".int_flash"):
        return " (internal flash)"
    return " (debug)"


def _sample_dir_from_test_id(test_id: str) -> str:
    parts = test_id.split(".")
    if len(parts) >= 3:
        return parts[2]
    raise ValueError(f"Unexpected Twister test id: {test_id}")


def _boards_for_test(platforms: list[str]) -> set[str]:
    boards: set[str] = set()
    for platform in platforms:
        board = PLATFORM_TO_BOARD.get(platform)
        if board is not None:
            boards.add(board)
    return boards


def _variant_label(sample_dir: str, suffix: str) -> str:
    title, ref = SAMPLE_DIR_REFS[sample_dir]
    return f":ref:`{title} <{ref}>`{suffix}"


def discover_variants() -> list[dict[str, Any]]:
    """Return Twister scenarios from all sample.yaml files under samples/."""
    variants: list[dict[str, Any]] = []

    for sample_yaml in sorted(SAMPLES_DIR.glob("*/sample.yaml")):
        with sample_yaml.open(encoding="utf-8") as handle:
            data = yaml.safe_load(handle)

        common = data.get("common", {})
        sample_dir = sample_yaml.parent.name

        for test_id, test in data.get("tests", {}).items():
            platforms = _platforms(test, common)
            extra_args = _extra_args(test, common)
            boards = _boards_for_test(platforms)
            if not boards:
                continue

            variants.append(
                {
                    "test_id": test_id,
                    "sample_dir": sample_dir,
                    "boards": boards,
                    "layout": _layout(extra_args),
                    "label": _variant_label(sample_dir, _suffix(test_id, extra_args)),
                }
            )

    return variants


def build_board_samples(
    board: str,
    usage_by_test: dict[str, Any],
) -> list[dict[str, Any]]:
    """Build memory table/chart rows for a board from sample.yaml and usage data."""
    samples: list[dict[str, Any]] = []

    for variant in discover_variants():
        if board not in variant["boards"]:
            continue

        usage = usage_by_test.get(variant["test_id"])
        if usage is None:
            continue

        sample: dict[str, Any] = {
            "label": variant["label"],
            "layout": variant["layout"],
            "usage": usage,
        }
        stack = usage.get("stack")
        if stack:
            sample["stack"] = stack
        samples.append(sample)

    return samples
