#!/bin/sh
astyle -r "*.cpp"
astyle -r "*.h"
rm -rf latex
rm -rf html
rm proofTranslator/*.orig
rm proofTranslator/*.txt
rm proofTranslator/*.png
rm proofTranslator/*.gv
rm proofTranslator/translator
rm -rf *.o *.orig main
