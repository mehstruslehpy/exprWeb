#!/bin/bash
echo "translator-runner-filter.sh: This script filters output from the translator program output and immediately runs the generated proof file on the prover."
echo ""
echo "Beginning Translator:"
echo ""
./translator | egrep  "(Please Enter a formula:)|(Premise or conclusion\?\(c/p\))|(Please Enter a conclusion:)"
chmod +x output_heredoc.txt
./output_heredoc.txt
echo "translator-runner-filter.sh: ending"
echo ""
