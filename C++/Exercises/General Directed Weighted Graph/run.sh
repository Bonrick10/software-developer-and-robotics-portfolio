#!/bin/bash

MAKE_DIR="build"

echo "Building project in $MAKE_DIR..."
make -C "$MAKE_DIR" || { echo "Build failed. Aborting commit."; exit 1; }

cd build
timeout 5s ctest --output-on-failure
status=$?

if [ $status -eq 124 ]; then
    echo "Tests timed out"
fi
cd ..

clang-format -i src/gdwg_graph.h src/gdwg_graph.test.cpp src/gdwg_graph.cpp


git status
echo "Staging changes..."
git add -A

read -p "Commit message: " msg

git commit -m "$msg"

echo "Build and commit complete!"