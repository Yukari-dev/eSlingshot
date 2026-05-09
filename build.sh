#!/usr/bin/env bash
set -euo pipefail

SRC_DIRS=(src)
BUILD_DIR="out"
INCLUDES="-Iinclude"
OUTPUT="$BUILD_DIR/out"

mkdir -p "$BUILD_DIR"

objs=()
while IFS= read -r -d '' src; do
  obj="$BUILD_DIR/$(basename "${src%.c}.o")"
  # echo "Compiling $src -> $obj"
  gcc $INCLUDES -c "$src" -o "$obj"
  objs+=("$obj")
done < <(find "${SRC_DIRS[@]}" -type f -name '*.c' -print0)

# echo "Linking -> $OUTPUT"
gcc "${objs[@]}" -o "$OUTPUT" -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# echo "Built $OUTPUT"
./"$OUTPUT"
