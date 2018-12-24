#!/bin/bash
echo "rlwrap-script.sh: This script wraps the input to the translator-runner-filter.sh script using rlwrap"
echo ""
echo "rlwrap-script.sh: Beginning nested script"
echo ""
rlwrap ./translator-runner-filter.sh
echo "rlwrap-script.sh: ending"
