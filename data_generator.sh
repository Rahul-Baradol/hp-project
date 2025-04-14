#!/bin/bash

# Set the root directory where the dummy data will be generated
root_dir="test_data"
mkdir -p $root_dir

# Function to generate random text containing a set of keywords
generate_random_text() {
  local file_path=$1
  local keywords=("target" "apple" "banana" "cloud" "search" "keyword")
  
  # Generate a random number of lines (between 5 and 10)
  num_lines=$((RANDOM % 6 + 5))
  
  # Generate random text with keywords inserted in random lines
  for i in $(seq 1 $num_lines); do
    line="This is a random line."

    # Randomly choose whether to include a keyword in the line
    if (( RANDOM % 2 )); then
      keyword="${keywords[$((RANDOM % ${#keywords[@]}))]}"
      line="This line contains the keyword: $keyword"
    fi
    
    # Add the generated line to the file
    echo "$line" >> "$file_path"
  done
}

# Function to create a random folder structure with files
create_random_data() {
  # Number of folders and files to generate
  num_folders=5
  num_files_per_folder=3

  for i in $(seq 1 $num_folders); do
    # Create a folder
    folder_name="$root_dir/folder_$i"
    mkdir -p $folder_name
    
    # Create files inside the folder
    for j in $(seq 1 $num_files_per_folder); do
      file_name="$folder_name/file_$j.txt"
      
      # Create a random file and add content
      generate_random_text $file_name
    done
  done
}

# Create the random folder structure with files
create_random_data

# Print the directory structure
echo "Dummy data generated under '$root_dir'"
find $root_dir

