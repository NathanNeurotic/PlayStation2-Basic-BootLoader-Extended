#!/usr/bin/env python3
"""
Generate a markdown manifest that lists every artifact produced in a build batch.

The manifest is intended to be pasted directly into release or pre-release notes
so users can quickly see which downloads belong to each batch.
"""

from __future__ import annotations

import argparse
import pathlib
from typing import Dict, Iterable, List, Tuple


IGNORED_CONFIG_KEYS = {
    "OUTDIR",
    "BASENAME",
    "MAKEOVERRIDES",
    "FLAGS",
    "CFLAGS",
    "DEFINES",
    "UTC_TIME",
    "COMMIT_HASH",
}


def parse_config(config_path: pathlib.Path) -> Dict[str, str]:
    config: Dict[str, str] = {}
    if not config_path.exists():
        return config
    for line in config_path.read_text().splitlines():
        line = line.strip()
        if not line or line == "OPTIONS":
            continue
        if "=" not in line:
            continue
        key, value = line.split("=", 1)
        config[key.strip()] = value.strip()
    return config


def describe_file(path: pathlib.Path) -> str:
    name = path.name
    if name.lower() == "build_config.txt":
        return "Build configuration metadata for this batch"
    if name.startswith("COMPRESSED_"):
        return "Compressed ELF (ps2-packer)"
    if name.endswith(".KELF"):
        return "Encrypted KELF image"
    if name.endswith(".ELF"):
        return "ELF binary"
    return "Artifact"


def human_size(size: int) -> str:
    units = ["B", "KB", "MB", "GB"]
    for unit in units:
        if size < 1024 or unit == units[-1]:
            return f"{size:.0f}{unit}"
        size /= 1024.0
    return f"{size:.0f}B"


def gather_files(outdir: pathlib.Path, dest: pathlib.Path) -> Iterable[pathlib.Path]:
    for entry in sorted(outdir.iterdir()):
        if entry.is_file() and entry.resolve() != dest.resolve():
            yield entry


def render_header(batch_name: str, outdir: pathlib.Path, config: Dict[str, str]) -> List[str]:
    lines: List[str] = [f"# Batch contents — {batch_name}", ""]
    variant = config.get("VARIANT")
    devices = config.get("DEVICES")
    global_mode = config.get("GLOBAL_MODE")
    device_modes = config.get("DEVICE_MODES")

    lines.append(f"- **Output directory:** `{outdir}`")
    if variant:
        lines.append(f"- **Variant:** `{variant}`")
    if devices:
        lines.append(f"- **Devices:** `{devices}`")
    if global_mode:
        lines.append(f"- **Mode:** `{global_mode}`")
    if device_modes:
        lines.append(f"- **Device modes:** `{device_modes}`")

    option_flags = sorted(
        f"{key}={value}"
        for key, value in config.items()
        if key not in IGNORED_CONFIG_KEYS and key not in {"VARIANT", "DEVICES", "GLOBAL_MODE", "DEVICE_MODES"}
    )
    if option_flags:
        lines.append(f"- **Options:** {', '.join(option_flags)}")

    lines.append("")
    return lines


def render_table(files: Iterable[pathlib.Path]) -> List[str]:
    rows: List[Tuple[str, str, str]] = []
    for file_path in files:
        rows.append(
            (
                f"`{file_path.name}`",
                human_size(file_path.stat().st_size),
                describe_file(file_path),
            )
        )
    if not rows:
        return ["No artifacts were found for this batch."]

    lines: List[str] = ["| File | Size | Description |", "| --- | --- | --- |"]
    for name, size, desc in rows:
        lines.append(f"| {name} | {size} | {desc} |")
    return lines


def write_manifest(batch_name: str, outdir: pathlib.Path, dest: pathlib.Path) -> None:
    config = parse_config(outdir / "BUILD_CONFIG.txt")
    parts: List[str] = []
    parts.extend(render_header(batch_name, outdir, config))
    parts.extend(render_table(gather_files(outdir, dest)))
    dest.parent.mkdir(parents=True, exist_ok=True)
    dest.write_text("\n".join(parts) + "\n")


def main() -> None:
    parser = argparse.ArgumentParser(description="Generate a markdown manifest for release batches.")
    parser.add_argument("--outdir", required=True, help="Build output directory to scan.")
    parser.add_argument("--dest", required=True, help="Destination markdown file.")
    parser.add_argument("--name", default=None, help="Human-friendly batch name (defaults to directory name).")
    args = parser.parse_args()

    outdir = pathlib.Path(args.outdir).resolve()
    dest = pathlib.Path(args.dest).resolve()
    batch_name = args.name or outdir.name

    write_manifest(batch_name, outdir, dest)
    print(f"Wrote batch manifest: {dest}")


if __name__ == "__main__":
    main()
