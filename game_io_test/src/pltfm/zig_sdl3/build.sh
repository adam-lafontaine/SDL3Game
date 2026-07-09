#!/bin/bash
# Clean zig build with colored filenames - errors in reverse order

echo "→ zig build $@ SDL3Game - SDL3"
echo "──────────────────────────────────────────"

output=$(zig build "$@" 2>&1)
exit_code=$?

CYAN='\033[0;36m'
NC='\033[0m'

# Collect error lines and print them in reverse
echo "$output" | grep -i "error:" | while read -r line; do
    filename_part=$(echo "$line" | cut -d':' -f1)
    filename=$(basename "$filename_part" 2>/dev/null || echo "$filename_part")
    rest=$(echo "$line" | cut -d':' -f2-)
    
    echo -e "${CYAN}${filename}${NC}:$rest"
done | tac   # tac reverses the lines

echo "──────────────────────────────────────────"

if [ $exit_code -eq 0 ]; then
    echo -e "\x1b[32m✓ Build succeeded\x1b[0m"
else
    echo -e "\x1b[31m✗ Build failed\x1b[0m"
fi

exit $exit_code