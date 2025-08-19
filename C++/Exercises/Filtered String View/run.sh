#!/bin/bash

MAKE_DIR="build"

echo "Building project in $MAKE_DIR..."
make -C "$MAKE_DIR" || { echo "Build failed. Aborting commit."; exit 1; }

cd build
ctest --output-on-failure
cd ..

clang-format -i src/filtered_string_view.h src/filtered_string_view.test.cpp src/filtered_string_view.cpp


git status
echo "Staging changes..."
git add -A

read -p "Commit message: " msg

git commit -m "$msg"

echo "Build and commit complete!"