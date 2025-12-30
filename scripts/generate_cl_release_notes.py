#!/usr/bin/env python3
"""
Generate detailed release notes for CL prereleases.

The script scans downloaded GitHub Action artifacts, extracts the batch
manifests contained in each variant archive, and renders a Markdown body
that can be published with the prerelease. This ensures users can see
which variants and artifacts are packaged inside every batch tarball.
"""

from __future__ import annotations

import argparse
import pathlib
import tarfile
from dataclasses import dataclass
from typing import Iterable, List, Optional


@dataclass
class VariantInfo:
    archive: pathlib.Path
    manifest: Optional[List[str]]


@dataclass
class PackageInfo:
    name: str
    archive: pathlib.Path
    variants: List[VariantInfo]


def find_manifest(tf: tarfile.TarFile) -> Optional[str]:
    for member in tf.getmembers():
        member_name = member.name.rsplit("/", maxsplit=1)[-1]
        if member_name != "BATCH_MANIFEST.md":
            continue
        extracted = tf.extractfile(member)
        if extracted is None:
            continue
        return extracted.read().decode("utf-8")
    return None


def load_manifest_from_tar(path: pathlib.Path) -> Optional[List[str]]:
    try:
        with tarfile.open(path, "r:*") as tf:
            manifest = find_manifest(tf)
    except (tarfile.TarError, OSError):
        return None

    if manifest is None:
        return None
    return manifest.strip().splitlines()


def downgrade_headings(lines: Iterable[str], offset: int = 3) -> List[str]:
    downgraded: List[str] = []
    for line in lines:
        if line.startswith("#"):
            hashes = line.split(" ", 1)[0]
            content = line[len(hashes) :].lstrip()
            level = len(hashes) + offset
            level = min(level, 6)
            downgraded.append("#" * level + (" " + content if content else ""))
        else:
            downgraded.append(line)
    return downgraded


def gather_packages(artifacts_dir: pathlib.Path, packages_dir: pathlib.Path) -> List[PackageInfo]:
    packages: List[PackageInfo] = []
    if not artifacts_dir.exists():
        return packages

    for batch_dir in sorted(p for p in artifacts_dir.iterdir() if p.is_dir()):
        variants: List[VariantInfo] = []
        for variant_archive in sorted(batch_dir.glob("*.tar.gz")):
            manifest_lines = load_manifest_from_tar(variant_archive)
            variants.append(VariantInfo(archive=variant_archive, manifest=manifest_lines))

        archive = packages_dir / f"{batch_dir.name}.tar.gz"
        packages.append(PackageInfo(name=batch_dir.name, archive=archive, variants=variants))
    return packages


def render_release_body(packages: List[PackageInfo], release_sha: str) -> List[str]:
    lines: List[str] = [
        f"Automated CL prerelease for `{release_sha}`",
        "",
        "Packages:",
    ]

    if not packages:
        lines.append("- _No artifacts were found to publish._")
        return lines

    for package in packages:
        lines.append(f"- `{package.archive.name}` — {len(package.variants)} variant archive(s)")

    for package in packages:
        lines.extend(
            [
                "",
                f"## Package `{package.archive.name}`",
                f"- Contains {len(package.variants)} variant archive(s).",
                "",
            ]
        )

        if not package.variants:
            lines.append("_No variant archives were found in this package._")
            continue

        for variant in package.variants:
            lines.append(f"### `{variant.archive.name}`")
            if variant.manifest:
                lines.extend(downgrade_headings(variant.manifest))
            else:
                lines.append("_Manifest not found in this archive; contents unavailable._")
            lines.append("")

    return lines


def main() -> None:
    parser = argparse.ArgumentParser(description="Render CL prerelease notes with batch manifests.")
    parser.add_argument("--artifacts-dir", required=True, help="Path containing downloaded artifacts.")
    parser.add_argument("--packages-dir", required=True, help="Path containing packaged release tarballs.")
    parser.add_argument("--output", required=True, help="Destination Markdown file.")
    parser.add_argument("--release-sha", required=True, help="Commit SHA for the release.")
    args = parser.parse_args()

    artifacts_dir = pathlib.Path(args.artifacts_dir).resolve()
    packages_dir = pathlib.Path(args.packages_dir).resolve()
    output_path = pathlib.Path(args.output).resolve()

    packages = gather_packages(artifacts_dir, packages_dir)
    body = "\n".join(render_release_body(packages, args.release_sha)) + "\n"
    output_path.write_text(body, encoding="utf-8")
    print(f"Wrote release notes to {output_path}")


if __name__ == "__main__":
    main()
