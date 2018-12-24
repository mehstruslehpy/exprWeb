#!/bin/sh
echo "Building random argument"
./hereDocGenerator.lisp > transfile.txt
echo "Making argument executable on the translator"
chmod +x transfile.txt
echo "Translating argument for proving"
./transfile.txt
echo "Making translated argument executable on the prover"
chmod +x output_heredoc.txt
echo "Running argument against the prover"
./output_heredoc.txt
