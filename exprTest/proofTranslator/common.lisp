;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;globals and spevial variables

; constants to use similar to C enums
; CEXPR := a conditional
(defparameter *cexpr* 0)
; OEXPR := an or
(defparameter *oexpr* 1)
; AEXPR := an and
(defparameter *aexpr* 2)
; NEXPR := a not
(defparameter *nexpr* 3)
; PEXPR := a proposition
; (defparameter *pexpr* 4)

;more depth then you would really most likely want while still being a
;vaguely sane choice
(defparameter *default-depth* 20)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;functions for picking random propositional letters

; use any upper case letter
(defun random-prop ()
 (code-char (+ (random 25) 65)))
; use any of the first six or so letters of the alphabet
(defun restricted-prop ()
 (code-char (+ (random 7) 65)))
; only use @ mainly for debugging
(defun single-prop ()
 (code-char 64))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;functions to print random logical formulas, these generate a style warning
;due to being mutually recursive I am not sure how to disable these warnings

;pick one of the various kinds of binary operators and recurse
(defun generate-binary (exprtype iterdepth prop-func)
 (cond
  ((= exprtype *cexpr*)
   (princ "(")
	 (print-expr iterdepth prop-func)
	 (princ "->")
	 (print-expr iterdepth prop-func)
   (princ ")"))
  ((= exprtype *aexpr*)
   (princ "(")
	 (print-expr iterdepth prop-func)
	 (princ "&")
	 (print-expr iterdepth prop-func)
   (princ ")"))
	((= exprtype *oexpr*)
   (princ "(")
	 (print-expr iterdepth prop-func)
	 (princ "v")
	 (print-expr iterdepth prop-func)
   (princ ")"))
  ((zerop iterdepth)
	 (princ (funcall prop-func)))
	((princ (funcall prop-func))))
 t)

;pick a unary operator or a proposition and recurse
(defun generate-unary (exprtype iterdepth prop-func)
 (cond
  ((zerop exprtype)
   (princ (funcall prop-func)))
	((not (zerop exprtype))
	 (princ "~")
	 (print-expr iterdepth prop-func))))

;print a full propositional formula
(defun print-expr (iterdepth prop-func)
 (let ((arity (random 2)))
  (cond
   ((or (< iterdepth 0) (zerop iterdepth))
	  (princ (funcall prop-func)))
	 ((zerop arity)
	  (generate-binary (random 4) (- iterdepth 1) prop-func))
   ((not (zerop arity))
	  (generate-unary (random 2) (- iterdepth 1) prop-func)))
 t))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;print a nicely formatted argument
(defun build-pretty-argument (iter &optional (depth *default-depth*)(counter 0))
 (cond
  ((zerop iter)
   (fresh-line)
	 (princ "|- ")
	 (print-expr depth #'restricted-prop)
   (fresh-line))
  ((not(zerop iter))
   (fresh-line)
   (princ counter)
	 (princ ") ")
	 (print-expr depth #'restricted-prop)
   (fresh-line)
	 (build-pretty-argument (- iter 1) depth (+ counter 1)))))
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; helper function for build-translator-heredoc
(defun build-arg-iter (iter &optional (depth *default-depth*))
 (cond
  ((zerop iter)
   (fresh-line)
	 (print-expr depth #'restricted-prop)
	 (fresh-line))
  ((not(zerop iter))
   (fresh-line)
	 (print-expr depth #'restricted-prop)
   (fresh-line)
	 (format t "p")
	 (fresh-line)
	 (build-arg-iter (- iter 1) depth))))
; print a heredoc formatted for the proof translator
(defun build-translator-heredoc (premises &optional (depth *default-depth*))
 (princ "./translator <<-ENDOFMESSAGE")
 (build-arg-iter premises depth)
 (fresh-line)
 (format t "c")
 (fresh-line)
 (print-expr depth #'restricted-prop)
 (fresh-line)
 (princ "ENDOFMESSAGE")
 t)
