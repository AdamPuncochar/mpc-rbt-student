#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CFG="$ROOT_DIR/config.json"
BIN="$ROOT_DIR/build/receiver_node"

: "${LOG_LEVEL:=4}"

LOG_LEVEL="$LOG_LEVEL" "$BIN" "$CFG"
