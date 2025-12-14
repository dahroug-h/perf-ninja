#!/usr/bin/env bash
set -euo pipefail

# --- paths (relative to this lab dir) ---
LAB_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${LAB_DIR}/build"
RESULTS_DIR="${LAB_DIR}/results"
COMPARE_PY="${LAB_DIR}/../../../tools/benchmark/tools/compare.py"

LATEST_JSON="${RESULTS_DIR}/latest.json"
PREV_JSON="${RESULTS_DIR}/prev.json"

NPROC="${NPROC:-8}"              # override: NPROC=16 ./build.sh
MIN_TIME="${MIN_TIME:-1s}"       # google-benchmark needs suffix: 1s, 500ms, ...
BENCH_FILTER="${BENCH_FILTER:-bench1}"
BENCH_ITERS="${BENCH_ITERS:-10}"

# Toggle xctrace:
#   TRACE=1 ./build.sh
TRACE="${TRACE:-0}"
TRACE_TEMPLATE="${TRACE_TEMPLATE:-Counters}"   # may vary; see notes below

mkdir -p "${BUILD_DIR}" "${RESULTS_DIR}"

echo "==> Configuring (Release)"
cmake -S "${LAB_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release

echo "==> Building (${NPROC} threads)"
cmake --build "${BUILD_DIR}" --parallel "${NPROC}"

echo "==> Validating"
cmake --build "${BUILD_DIR}" --target validateLab

LAB_EXE="${BUILD_DIR}/lab"
if [[ ! -x "${LAB_EXE}" ]]; then
  echo "ERROR: Expected executable not found: ${LAB_EXE}"
  exit 1
fi

INPUT_PBM="${LAB_DIR}/pexels-pixabay-434334.pbm"
OUTPUT_PGM="${RESULTS_DIR}/output.pgm"

# Common args for both normal run and tracing
LAB_ARGS=(
  "${INPUT_PBM}"
  "${OUTPUT_PGM}"
  "--benchmark_filter=${BENCH_FILTER}"
  "--benchmark_min_time=${MIN_TIME}"
)

echo "==> Benchmarking -> ${LATEST_JSON}"
"${LAB_EXE}" \
  "${LAB_ARGS[@]}" \
  --benchmark_out_format=json \
  --benchmark_out="${LATEST_JSON}"

if [[ -f "${PREV_JSON}" ]]; then
  if [[ -f "${COMPARE_PY}" ]]; then
    echo "==> Comparing prev.json -> latest.json"
    python3 "${COMPARE_PY}" benchmarks "${PREV_JSON}" "${LATEST_JSON}" || true
  else
    echo "WARNING: compare.py not found at: ${COMPARE_PY} (skipping compare)"
  fi
else
  echo "==> No previous result found (${PREV_JSON}); skipping compare."
fi

cp -f "${LATEST_JSON}" "${PREV_JSON}"
echo "==> Saved baseline for next run: ${PREV_JSON}"

if [[ "${TRACE}" == "1" ]]; then
  if ! command -v xcrun >/dev/null 2>&1; then
    echo "WARNING: xcrun not found; cannot run xctrace."
    exit 0
  fi

  # Timestamped trace output (directory bundle)
  TS="$(date +%Y%m%d_%H%M%S)"
  TRACE_OUT="${RESULTS_DIR}/counters_${BENCH_FILTER}_${TS}.trace"

  echo "==> Recording counters with xctrace -> ${TRACE_OUT}"
  echo "    Template: ${TRACE_TEMPLATE}"
  echo "    Command : ${LAB_EXE} ${LAB_ARGS[*]}"

  # xctrace will launch the process; counters are in the .trace
  xcrun xctrace record \
    --template "${TRACE_TEMPLATE}" \
    --output "${TRACE_OUT}" \
    --launch -- "${LAB_EXE}" "${LAB_ARGS[@]}"

  echo "==> Done. Open trace with:"
  echo "    open \"${TRACE_OUT}\""
fi