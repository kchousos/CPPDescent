#!/bin/bash
cmake -S . -B build
# Αυτήν την στιγμή το cppdescent δεν δουλεύει επειδή δεν έχουμε
# υλοποιήσει τις δομές.
cmake --build build --target hello_test
./build/hello_test