"using cond form"
(let ((x 3))
    (cond ((= 1 x) "first")
        ((= 2 x) "second")
        (else "third")
        )
    )

"print nth element of list"
(define (nth n list)
  (if (= n 1)
      (car list)
      (nth (- n 1) (cdr list))
      )
  )

; use
(nth 3 '(1 2 3 4))
(nth 3 '(1 3 2 4))

"print nth element of list"
(define (better_nth n list)
  (cond 
    ((null? list) "index out of bound")
    ((= n 1) (car list))
    (else (better_nth (- n 1) (cdr list)))
      )
  )
;use
(better_nth 3 '(1 3 2 4))
(better_nth 5 '(1 3 2 4))
(better_nth 6 '(1 3 2 4))
(better_nth 4 '(1 3 2 4))
(better_nth 0 '(1 3 2 4))

"reverse elements of a list O(n^2)"
(define (rev L)
  (cond 
    ((null? L) '())
    ((pair? (car L)) (append (rev (cdr L)) (list (rev (car L))) ))
    (else (append (rev (cdr L)) (list (car L))) )
        )
  )

;use
(rev '())
(rev '(1 2 3 4))
(rev '(1 2 (3 4)))

"reverse elements of a list O(n)"
(define (fast_rev L sofar)
  (cond 
    ((null? L) sofar)
    ((pair? (car L)) (fast_rev (cdr L) (cons (fast_rev_main (car L)) sofar)) )
    (else (fast_rev (cdr L) (cons (car L) sofar)))
        )
  )
; driver for fast reverse
(define (fast_rev_main L) (fast_rev L '()))

;use
(fast_rev_main '())
(fast_rev_main '(1 2 3 4))
(fast_rev_main '(1 2 (3 4)))

"own impl of append"
(define (my_append L1 L2)
  (cond
    ((null? L1) L2)
    (else (cons (car L1) (my_append (cdr L1) L2)))
    )
  )
;use
(my_append '(1 2) '(3 4))
(my_append '(1 2) '(3 (4 5)))
(my_append '(1 2) '())

"own impl of append with flattening of nested lists"
(define (my_append_flat L1 L2)
  (cond
    ((null? L1) L2)
    ((pair? (car L1)) (my_append_flat (my_append_flat (car L1) (cdr L1)) L2) )
    (else (cons (car L1) (my_append_flat (cdr L1) L2)))
    )
  )
; my_append driver (to flatten L2 as well)
(define (my_append_flatten L1 L2)
  (my_append_flat (my_append_flat L1 L2) '())
  )
;use
(my_append_flatten '(1 (2 3)) '(4 5))
(my_append_flatten '(1 2) '(3 (4 5)))
(my_append_flatten '(1 2) '())

"own impl of map"
(define (my_map f L)
  (cond
    ((null? L) L)
    (else (cons (f (car L)) (my_map f (cdr L)) ))
   )
  )
;use
(my_map (lambda (x) (* x 2)) '(1 2))

"function returning a function"
(define (func1 x) (lambda (y) (+ x y)))
(func1 5)
((func1 5) 10)

"factorial using letrec"
(letrec
  (
   (fac 
    (lambda 
      (x)
      (if (= 0 x) 1 (* x (fac (- x 1)))
      )
    )
   )
  ) 
  (fac 5) )

"Horner's Rule"
(define     
     (poly L)
      (lambda (x) (if (null? L) 0 (+ 
                                   (car L) 
                                   (* x ((poly (cdr L)) x))
                                   )
                   )
       )
  )
(define (poly_driver L) (poly (reverse L)) )
;use
(poly_driver '(2 4 3 5))
((poly_driver '(2 4 3 5)) 2)
