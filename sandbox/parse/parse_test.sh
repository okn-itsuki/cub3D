#!/bin/sh

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT_DIR=$(CDPATH= cd -- "$SCRIPT_DIR/../.." && pwd)
EXEC_FILE="$SCRIPT_DIR/DEBUG_parse_test"
MAP_DIR="$ROOT_DIR/maps"
LOG_DIR="$SCRIPT_DIR/logs"
LOG_FILE="$LOG_DIR/$(date '+%Y%m%d_%H%M%S').md"

mkdir -p "$LOG_DIR" || exit 1

first=true

find "$MAP_DIR" -type f -name '*.cub' | sort | while IFS= read -r map_file; do
	output=$("$EXEC_FILE" "$map_file" 2>&1)
	status=$?

	if [ "$first" = true ]; then
		first=false
	else
		printf '%s\n\n' '---' >> "$LOG_FILE"
	fi

	printf '## map file : %s\n' "${map_file#$ROOT_DIR/}" >> "$LOG_FILE"
	printf '## exit status : %d\n\n' "$status" >> "$LOG_FILE"
	printf '%s\n' '```sh' >> "$LOG_FILE"
	printf '%s\n' "$output" >> "$LOG_FILE"
	printf '%s\n\n' '```' >> "$LOG_FILE"
done

printf 'log 出力！\npath :%s\n' "$LOG_FILE"
