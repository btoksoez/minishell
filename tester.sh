#!/bin/bash

# Read the fixes_test.txt file
while IFS= read -r line; do
    # Check if the line is not empty and does not start with a comment
    if [[ -n "$line" && ! "$line" =~ ^\ *# ]]; then
        echo "Processing line: $line"

        # Run the command in minishell
        minishell_output=$(echo "$line" | ./minishell)
        minishell_exit_code=$?

        # Run the same command in bash
        bash_output=$(echo "$line" | bash)
        bash_exit_code=$?

        # Print a separator
        echo "--------------------------------------------"

        # Compare the exit codes and print the result
        if [[ $minishell_exit_code -eq $bash_exit_code ]]; then
            echo -e "\e[32m✓ Exit codes match\e[0m"
        else
            echo -e "\e[31m✗ Exit codes do not match\e[0m"
        fi

        # Print a separator
        echo "--------------------------------------------"
    fi
done < team/fixes_test.txt
