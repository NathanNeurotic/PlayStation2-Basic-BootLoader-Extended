#!/usr/bin/env bash
set -euo pipefail

# Run cppcheck with the include paths Codacy expects.
# The PS2SDK headers may not be available in the CI image that runs static analysis,
# so we point cppcheck at lightweight shims under tools/codacy_shims/ps2sdk.

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PS2SDK_ROOT="${PS2SDK:-/usr/local/ps2dev/ps2sdk}"

INCLUDE_PATHS=(
  "-I${PS2SDK_ROOT}/ee/include"
  "-I${PS2SDK_ROOT}/iop/include"
  "-I${PS2SDK_ROOT}/common/include"
  "-I${PROJECT_ROOT}/include"
  "-I${PROJECT_ROOT}/tools/codacy_shims/ps2sdk"
  "-I${PROJECT_ROOT}/tools/codacy_shims/stdlib"
)

exec cppcheck \
  --library=std.cfg \
  --check-config \
  --enable=all \
  --std=c99 \
  --inline-suppr \
  --suppressions-list="${PROJECT_ROOT}/cppcheck-suppressions.txt" \
  --suppress=missingIncludeSystem \
  --suppress=missingInclude \
  "${INCLUDE_PATHS[@]}" \
  "${PROJECT_ROOT}/src" \
  "${PROJECT_ROOT}/include"
