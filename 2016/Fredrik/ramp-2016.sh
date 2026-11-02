#!/bin/bash
# /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/ramp-2016.sh

# Keeping all the days seperate without for-loops or nothing is deliberate, as then I can experiment with CLI functions
# with args and kwargs and what not. Also to have possibility of solving one with only bash or something.

set -u

##### SETTINGS #####
BUILD_ALL="0"
RUN_ALL="0"

# ROOT_DIR="$HOME/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik"
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Rsync frallfiles, only Fredrik can do this
RSYNC_FRALLFILES="0"

# Building specific ones
BUILD_1="0"
BUILD_2="0"
BUILD_3="0"
BUILD_4="0"
BUILD_5="1"

# Running specific ones
RUN_1="0"
RUN_2="0"
RUN_3="0"
RUN_4="0"
RUN_5="1"

##### COMPILER FLAGS #####
# Each set is a bash array: one flag per element.
# Use in the gcc line as "${CFLAGS[@]}" (quoted, with [@]).

# Everyday set: building and stepping in gdb.
CFLAGS_DEBUG=(
    -Wall   # the common warnings (unused variable, missing return, wrong printf format)
    -Wextra # more warnings that -Wall leaves out (unused parameter, signed/unsigned compare)
    -g      # debug info, so gdb shows source lines and variable names
    -O0     # no optimization: the binary matches the source line by line (gcc default)
)

# Bug hunting: the program checks itself while running and stops with
# the source line on a memory error. About 2x slower. Not with valgrind.
CFLAGS_SANITIZE=(
    -Wall
    -Wextra
    -g # needed for source lines in the sanitizer reports
    -O0
    -fsanitize=address,undefined # address: out-of-bounds, use after free, leaks
    # undefined: integer overflow, bad shifts, null dereference
    -fno-omit-frame-pointer # complete call chains in the reports
)

# Cleanup pass: extra warnings, and any warning stops the build.
# Also applies to frallfiles.c, so one warning there stops every day.
CFLAGS_STRICT=(
    -Wall
    -Wextra
    -Wshadow     # inner variable hides an outer one with the same name
    -Wconversion # implicit conversion that can lose data (long to int, int to char)
    -Werror      # treat every warning as an error: no binary is produced
    -g
    -Og # light optimization that stays debuggable, and turns on the
    # uninitialized-variable analysis that -O0 skips
)

# Timing and slow brute force. No -g, so gdb shows no source lines.
CFLAGS_FAST=(
    -Wall
    -Wextra
    -O2 # "full" normal optimization. Can expose undefined behaviour
    # that -O0 hides; if so, find it with CFLAGS_SANITIZE
)

# The one in use. Change only this line to switch active build.
CFLAGS=("${CFLAGS_FAST[@]}")

################### START ###################
echo "ramp started for advent of code 2016"
TEMP_VAR=""
cd "$ROOT_DIR" || exit 1

# Rsync frallfiles
if [[ "$RSYNC_FRALLFILES" == 1 ]] && [[ -f "$HOME/programming/c/frallfiles.c" ]]; then
    ./rsync_frallfiles.sh || exit 1
elif [[ "$RSYNC_FRALLFILES" == 0 ]]; then
    echo ""
else
    echo "Skipping rsync, using the lib/ in the repo"
fi

################### 1 ###################

# Building aoc1
if [[ "$BUILD_ALL" == 1 ]] || [[ "$BUILD_1" == 1 ]]; then
    cd "$ROOT_DIR"/01 || exit
    echo ""
    echo "Building aoc01a"
    mkdir -p "$ROOT_DIR/01/build"
    # if gcc "${CFLAGS_FAST[@]}" -I"$ROOT_DIR/lib" \
    if gcc "${CFLAGS[@]}" -I"$ROOT_DIR/lib" \
        -o "$ROOT_DIR/01/build/aoc01a_taxi_distances" \
        "$ROOT_DIR/01/aoc01a_taxi_distances.c" \
        "$ROOT_DIR/lib/frallfiles.c"; then
        echo "  OK"
    else
        echo "  FAILED" >&2
        exit 1
    fi
    echo "Building aoc01b"
    # if gcc "${CFLAGS_FAST[@]}" -I"$ROOT_DIR/lib" \
    if gcc "${CFLAGS[@]}" -I"$ROOT_DIR/lib" \
        -o "$ROOT_DIR/01/build/aoc01b_taxi_distances" \
        "$ROOT_DIR/01/aoc01b_taxi_distances.c" \
        "$ROOT_DIR/lib/frallfiles.c"; then
        echo "  OK"
    else
        echo "  FAILED" >&2
        exit 1
    fi
fi

# Running aoc1
if [[ "$RUN_ALL" == 1 ]] || [[ "$RUN_1" == 1 ]]; then
    cd "$ROOT_DIR"/01 || exit
    echo ""
    echo "Running aoc01a"
    echo ""
    TEMP_VAR="$("$ROOT_DIR"/01/build/aoc01a_taxi_distances)"
    echo "$TEMP_VAR"
    echo ""
    echo "Running aoc01b"
    echo ""
    TEMP_VAR="$("$ROOT_DIR"/01/build/aoc01b_taxi_distances)"
    echo "$TEMP_VAR"
fi
echo ""

################### 2 ###################

# Building aoc2
if [[ "$BUILD_ALL" == 1 ]] || [[ "$BUILD_2" == 1 ]]; then
    cd "$ROOT_DIR"/02 || exit
    echo ""
    echo "Building aoc02a"
    mkdir -p "$ROOT_DIR/02/build"
    if gcc "${CFLAGS[@]}" -I"$ROOT_DIR/lib" \
        -o "$ROOT_DIR/02/build/aoc02a_keypad" \
        "$ROOT_DIR/02/aoc02a_keypad.c" \
        "$ROOT_DIR/lib/frallfiles.c"; then
        echo "  OK"
    else
        echo "  FAILED" >&2
        exit 1
    fi
    echo "Building aoc02b"
    if gcc "${CFLAGS[@]}" -I"$ROOT_DIR/lib" \
        -o "$ROOT_DIR/02/build/aoc02b_keypad" \
        "$ROOT_DIR/02/aoc02b_keypad.c" \
        "$ROOT_DIR/lib/frallfiles.c"; then
        echo "  OK"
    else
        echo "  FAILED" >&2
        exit 1
    fi
fi

# Running aoc2
if [[ "$RUN_ALL" == 1 ]] || [[ "$RUN_2" == 1 ]]; then
    cd "$ROOT_DIR"/02 || exit
    echo ""
    echo "Running aoc02a"
    echo ""
    TEMP_VAR="$("$ROOT_DIR"/02/build/aoc02a_keypad)"
    echo "$TEMP_VAR"
    echo ""
    echo "Running aoc02b"
    echo ""
    TEMP_VAR="$("$ROOT_DIR"/02/build/aoc02b_keypad)"
    echo "$TEMP_VAR"
fi
echo ""

################### 3 ###################

# Building aoc3
if [[ "$BUILD_ALL" == 1 ]] || [[ "$BUILD_3" == 1 ]]; then
    cd "$ROOT_DIR"/03 || exit
    echo ""
    echo "Building aoc03a"
    mkdir -p "$ROOT_DIR/03/build"
    if gcc "${CFLAGS[@]}" -I"$ROOT_DIR/lib" \
        -o "$ROOT_DIR/03/build/aoc03a_triangles" \
        "$ROOT_DIR/03/aoc03a_triangles.c" \
        "$ROOT_DIR/lib/frallfiles.c"; then
        echo "  OK"
    else
        echo "  FAILED" >&2
        exit 1
    fi
    echo "Building aoc03b"
    if gcc "${CFLAGS[@]}" -I"$ROOT_DIR/lib" \
        -o "$ROOT_DIR/03/build/aoc03b_triangles" \
        "$ROOT_DIR/03/aoc03b_triangles.c" \
        "$ROOT_DIR/lib/frallfiles.c"; then
        echo "  OK"
    else
        echo "  FAILED" >&2
        exit 1
    fi
fi

# Running aoc3
if [[ "$RUN_ALL" == 1 ]] || [[ "$RUN_3" == 1 ]]; then
    cd "$ROOT_DIR"/03 || exit
    echo ""
    echo "Running aoc03a"
    echo ""
    TEMP_VAR="$("$ROOT_DIR"/03/build/aoc03a_triangles)"
    echo "$TEMP_VAR"
    echo ""
    echo "Running aoc03b"
    echo ""
    TEMP_VAR="$("$ROOT_DIR"/03/build/aoc03b_triangles)"
    echo "$TEMP_VAR"
fi
echo ""

################### 4 ###################

# Building aoc4
if [[ "$BUILD_ALL" == 1 ]] || [[ "$BUILD_4" == 1 ]]; then
    cd "$ROOT_DIR"/04 || exit
    echo ""
    echo "Building aoc04a"
    mkdir -p "$ROOT_DIR/04/build"
    if gcc "${CFLAGS[@]}" -I"$ROOT_DIR/lib" \
        -o "$ROOT_DIR/04/build/aoc04a_decode_decoys" \
        "$ROOT_DIR/04/aoc04a_decode_decoys.c" \
        "$ROOT_DIR/lib/frallfiles.c"; then
        echo "  OK"
    else
        echo "  FAILED" >&2
        exit 1
    fi
    echo "Building aoc04b"
    if gcc "${CFLAGS[@]}" -I"$ROOT_DIR/lib" \
        -o "$ROOT_DIR/04/build/aoc04b_decode_decoys" \
        "$ROOT_DIR/04/aoc04b_decode_decoys.c" \
        "$ROOT_DIR/lib/frallfiles.c"; then
        echo "  OK"
    else
        echo "  FAILED" >&2
        exit 1
    fi
fi

# Running aoc4
if [[ "$RUN_ALL" == 1 ]] || [[ "$RUN_4" == 1 ]]; then
    cd "$ROOT_DIR"/04 || exit
    echo ""
    echo "Running aoc04a"
    echo ""
    TEMP_VAR="$("$ROOT_DIR"/04/build/aoc04a_decode_decoys)"
    echo "$TEMP_VAR"
    echo ""
    echo "Running aoc04b"
    echo ""
    TEMP_VAR="$("$ROOT_DIR"/04/build/aoc04b_decode_decoys)"
    echo "$TEMP_VAR"
fi
echo ""

################### 5 ###################

# Building aoc5
if [[ "$BUILD_ALL" == 1 ]] || [[ "$BUILD_5" == 1 ]]; then
    cd "$ROOT_DIR"/05 || exit
    echo ""
    echo "Building aoc05a"
    mkdir -p "$ROOT_DIR/05/build"
    if gcc "${CFLAGS[@]}" -I"$ROOT_DIR/lib" \
        -o "$ROOT_DIR/05/build/aoc05a_hash" \
        "$ROOT_DIR/05/aoc05a_hash.c" \
        "$ROOT_DIR/lib/frallfiles.c"; then
        echo "  OK"
    else
        echo "  FAILED" >&2
        exit 1
    fi
    # echo "Building aoc04b"
    # if gcc "${CFLAGS[@]}" -I"$ROOT_DIR/lib" \
    #     -o "$ROOT_DIR/04/build/aoc04b_decode_decoys" \
    #     "$ROOT_DIR/04/aoc04b_decode_decoys.c" \
    #     "$ROOT_DIR/lib/frallfiles.c"; then
    #     echo "  OK"
    # else
    #     echo "  FAILED" >&2
    #     exit 1
    # fi
fi

# Running aoc5
if [[ "$RUN_ALL" == 1 ]] || [[ "$RUN_5" == 1 ]]; then
    cd "$ROOT_DIR"/05 || exit
    echo ""
    echo "Running aoc05a"
    echo ""
    TEMP_VAR="$("$ROOT_DIR"/05/build/aoc05a_hash)"
    echo "$TEMP_VAR"
    echo ""
    # echo "Running aoc04b"
    # echo ""
    # TEMP_VAR="$("$ROOT_DIR"/04/build/aoc04b_decode_decoys)"
    # echo "$TEMP_VAR"
fi
echo ""
