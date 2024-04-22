#!/bin/bash

# Function to recursively list files in all folders
list_files() {
    local folder="$1"
    local prefix="$2"

    # Loop through files and folders in the current directory
    for item in "$folder"/*; do
        # If the item is a directory, recursively list files in it
        if [ -d "$item" ]; then
            list_files "$item" "$prefix$(basename "$item")/"
        # If the item is a file, append its name prefixed with the folder to the output file
        elif [ -f "$item" ]; then
            echo -n "$prefix$(basename "$item") \\" >> file_list.txt
        fi
    done
}

# Start listing files from the current directory
list_files . ""

# Sort the file list alphabetically and remove duplicates
sort -u -o file_list.txt file_list.txt
