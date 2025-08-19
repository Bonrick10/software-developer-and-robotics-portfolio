#!/bin/bash

# Check number of lines
command=`git diff --stat --cached`
insertions=`echo $command | grep -Eo '[0-9]+ insertions' | sed 's/ insertions//g'`

if [[ "$insertions" -gt 50 ]]; then
  git diff --stat --cached
  echo -e "\033[0;31m"
  echo "================================================================================"
  echo "= Looks like you are trying to commit "$insertions" lines of code              ="
  echo -n "$(tput bold)"
  echo "= Commit rejected                                                              ="
  echo -n "$(tput sgr0)"
  echo -e -n "\033[0;31m"
  echo "================================================================================"
  echo -e "\033[0m"
  exit 1
fi

# Define ANSI color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
BOLD='\033[1m'
NC='\033[0m' # No Color

cmake -B build && cd build && make

if [[ "$?" -eq 0 ]]; then
  echo -e "${GREEN}${BOLD}Commit successful!${NC}"
else
  echo -e "${RED}${BOLD}Commit rejected!${NC}"
  exit 1
fi
