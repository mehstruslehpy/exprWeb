#!/bin/sbcl --script
;load the correct files
(load "common.lisp")
;seed the random number generator
(setf *random-state* (make-random-state t))
;build a random argmument with up to 20 premises
;(build-translator-heredoc (+ (random 5) 1) 500)
(build-translator-heredoc (+ (random 5) 1) 5)
