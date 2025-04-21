#!/bin/bash

BASE_DIR="./simple_fs"
SEARCH_WORD="$1"

rm -rf "$BASE_DIR"
mkdir -p "$BASE_DIR"

echo "Creating simple file system at $BASE_DIR"

for i in {1..3}; do
    DIR="$BASE_DIR/dir_$i"
    mkdir -p "$DIR"
    
    for j in {1..3}; do
        FILE="$DIR/file_${j}.txt"
        echo "File $j in $DIR without anything suspicious." > "$FILE"
        
        # Randomly include the search word
        if (( RANDOM % 8 == 0 )); then
            echo "Hey, here's a $SEARCH_WORD in this one." >> "$FILE"
        fi
    done
done

# Add one guaranteed match
echo "Absolutely contains $SEARCH_WORD for testing." > "$BASE_DIR/dir_2/definite_match.txt"

echo "✅ Simple file system created."
