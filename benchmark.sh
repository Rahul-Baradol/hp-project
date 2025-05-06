#!/bin/bash

SEARCH_TERM="banana"
SEARCH_DIR="./complex_fs"
LOG_FILE="benchmark_results.log"
YOUR_TOOL="./search_tool"  # change this if it's not named like this

# Clear old log
echo "=== Benchmark Results: $(date) ===" > "$LOG_FILE"
echo "Searching for: \"$SEARCH_TERM\" in $SEARCH_DIR" >> "$LOG_FILE"
echo "" >> "$LOG_FILE"

# Function to benchmark and log time
run_benchmark() {
    local label=$1
    local cmd=$2

    echo "[$label]" >> "$LOG_FILE"
    echo "$cmd" >> "$LOG_FILE"

    # capture time only
    { time eval "$cmd" > /dev/null; } 2>> "$LOG_FILE"
    echo "" >> "$LOG_FILE"
}

# === Your tool ===
run_benchmark "Search_tool -exact" "$YOUR_TOOL $SEARCH_DIR $SEARCH_TERM" "--mode=exact"
run_benchmark "Search_tool -case" "$YOUR_TOOL $SEARCH_DIR $SEARCH_TERM" "--mode=case"
run_benchmark "Search_tool -fuzzy" "$YOUR_TOOL $SEARCH_DIR $SEARCH_TERM" "--mode=fuzzy"

# === grep ===
run_benchmark "grep -ri -l" "grep -ril \"$SEARCH_TERM\" $SEARCH_DIR"

# === ripgrep ===
run_benchmark "rg -l" "rg -l \"$SEARCH_TERM\" $SEARCH_DIR"
run_benchmark "rg -F -l" "rg -F -l \"$SEARCH_TERM\" $SEARCH_DIR"
run_benchmark "rg -F -i -l" "rg -F -i -l \"$SEARCH_TERM\" $SEARCH_DIR"
run_benchmark "rg -F -w -l" "rg -F -w -l \"$SEARCH_TERM\" $SEARCH_DIR"
run_benchmark "rg -F -l --no-ignore --no-config" "rg -F -l --no-ignore --no-config \"$SEARCH_TERM\" $SEARCH_DIR"
run_benchmark "rg -F -l --mmap" "rg -F -l --mmap \"$SEARCH_TERM\" $SEARCH_DIR"
run_benchmark "rg -F -l --no-mmap" "rg -F -l --no-mmap \"$SEARCH_TERM\" $SEARCH_DIR"
run_benchmark "rg -F -l --threads 4" "rg -F -l --threads 4 \"$SEARCH_TERM\" $SEARCH_DIR"

echo "✅ Benchmarking done. See results in $LOG_FILE"
