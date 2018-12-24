#!/bin/sbcl --script
;load the correct files
(load "common.lisp")
;seed the random number generator
(setf *random-state* (make-random-state t))
(princ "Can you prove or disprove the following argument?")
;build a random argmument with up to 20 premises
;(build-pretty-argument (+ (random 20) 1) 300)
(build-pretty-argument (+ (random 20) 1) 5)
