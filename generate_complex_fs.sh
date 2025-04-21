#!/bin/bash

BASE_DIR="./complex_fs"
SEARCH_WORD="$1"

rm -rf "$BASE_DIR"
mkdir -p "$BASE_DIR"

echo "Creating complex file system at $BASE_DIR"

for i in {1..5}; do
    DIR1="$BASE_DIR/level1_dir_$i"
    mkdir -p "$DIR1"
    
    for j in {1..3}; do
        DIR2="$DIR1/level2_dir_$j"
        mkdir -p "$DIR2"
        
        for k in {1..4}; do
            FILE="$DIR2/file_${k}.txt"
            echo "This is file $k in dir $DIR2" > "$FILE"
            
            # Randomly include the search word
            if (( RANDOM % 15 == 0 )); then
                echo "This line has the $SEARCH_WORD in it." >> "$FILE"
            fi
        done
    done
done

# Add one obvious hit
echo "This file contains the $SEARCH_WORD clearly." > "$BASE_DIR/level1_dir_3/level2_dir_2/obvious_hit.txt"

echo "✅ Complex file system created."
