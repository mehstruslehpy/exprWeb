#!/bin/sh
astyle -r "*.cpp"
astyle -r "*.h"
rm -rf latex
rm -rf html
rm proofTranslator/*.orig
rm proofTranslator/*.txt
rm proofTranslator/translator
rm calcTranslator/*.orig
rm calcTranslator/*.txt
rm calcTranslator/translator
rm -rf *.o *.orig main
