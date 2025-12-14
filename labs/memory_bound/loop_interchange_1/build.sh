#!/usr/bin/env bash
set -euo pipefail

# --- paths (relative to this lab dir) ---
LAB_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${LAB_DIR}/build"
RESULTS_DIR="${LAB_DIR}/results"
COMPARE_PY="${LAB_DIR}/../../../tools/benchmark/tools/compare.py"

LATEST_JSON="${RESULTS_DIR}/latest.json"
PREV_JSON="${RESULTS_DIR}/prev.json"

NPROC="${NPROC:-8}"          # override: NPROC=16 ./build.sh
MIN_TIME="${MIN_TIME:-1}"    # override: MIN_TIME=2 ./build.sh

mkdir -p "${BUILD_DIR}" "${RESULTS_DIR}"

echo "==> Configuring (Release)"
cmake -S "${LAB_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release

echo "==> Building (${NPROC} threads)"
cmake --build "${BUILD_DIR}" --parallel "${NPROC}"

echo "==> Validating"
cmake --build "${BUILD_DIR}" --target validateLab

# Determine the lab executable (most labs build 'lab' in the build dir)
LAB_EXE="${BUILD_DIR}/lab"
if [[ ! -x "${LAB_EXE}" ]]; then
  echo "ERROR: Expected executable not found: ${LAB_EXE}"
  echo "       Check what CMake generated in ${BUILD_DIR} (ls -l)."
  exit 1
fi

echo "==> Benchmarking -> ${LATEST_JSON}"
"${LAB_EXE}" \
  --benchmark_min_time="${MIN_TIME}" \
  --benchmark_out_format=json \
  --benchmark_out="${LATEST_JSON}"

if [[ -f "${PREV_JSON}" ]]; then
  if [[ ! -f "${COMPARE_PY}" ]]; then
    echo "WARNING: compare.py not found at: ${COMPARE_PY}"
    echo "         Skipping comparison."
  else
    echo "==> Comparing prev.json -> latest.json"
    python3 "${COMPARE_PY}" benchmarks "${PREV_JSON}" "${LATEST_JSON}" || true
  fi
else
  echo "==> No previous result found (${PREV_JSON}); skipping compare."
fi

# Rotate results for next run
cp -f "${LATEST_JSON}" "${PREV_JSON}"
echo "==> Saved baseline for next run: ${PREV_JSON}"
