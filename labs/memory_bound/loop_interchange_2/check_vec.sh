#!/usr/bin/env bash
set -euo pipefail

LAB_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${LAB_DIR}/build-vec"
OUT_DIR="${LAB_DIR}/vec_reports"
NPROC="${NPROC:-8}"

mkdir -p "${BUILD_DIR}" "${OUT_DIR}"

# Clang vectorization remarks + (optional) structured optimization records:
CXX_FLAGS=(
  "-O3"
  "-DNDEBUG"
  "-Rpass=loop-vectorize"
  "-Rpass-missed=loop-vectorize"
  "-Rpass-analysis=loop-vectorize"
  "-fsave-optimization-record"
)

echo "==> Configure"
cmake -S "${LAB_DIR}" -B "${BUILD_DIR}" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_CXX_FLAGS="$(printf " %s" "${CXX_FLAGS[@]}")" \
  >/dev/null

echo "==> Build (capturing remarks)"
# Capture *all* build output, but we'll filter it after.
cmake --build "${BUILD_DIR}" --parallel "${NPROC}" 2>&1 | tee "${OUT_DIR}/build_full.log"

echo
echo "==> Filtered vectorization remarks (solution.cpp only)"
grep -n "solution\.cpp" "${OUT_DIR}/build_full.log" | tee "${OUT_DIR}/solution_vec.log" || true

echo
echo "==> Summary"
echo "  Full log:        ${OUT_DIR}/build_full.log"
echo "  solution.cpp log:${OUT_DIR}/solution_vec.log"
echo "  Opt records:     ${BUILD_DIR}/*.opt.yaml (if any)"