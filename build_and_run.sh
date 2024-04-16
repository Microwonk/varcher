#!/bin/bash

# Generate build files
cmake --preset=linux-clang

# Build the project
cmake --build build

# Run the executable (assuming it's named 'my_project')
./build/app/app "$@"
