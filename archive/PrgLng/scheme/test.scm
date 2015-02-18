(define one 100)

(define two 200)

(define (foo x y) (+ x y))

(define (test-define-func)
  (display "Should return 300") (newline)
  (foo one two))


(define (test-let)
  (let ((one 1) (two (+ one 1)))
    (display "Should return 101") (newline)
    two))


(define (test-let*)
  (let* ((one 1) (two (+ one 1)))
    (display "Should return 2") (newline)
    two))


(define (test-letrec)
  (letrec
      ((f (lambda (x) (if (= x 0) 1 (* x (g (- x 1))))))
       (g (lambda (x) (if (= x 0) 1 (* x (f (- x 1)))))))
    (display "Should return 120") (newline)
    (f 5)))


(define (test-cond)
  (display "Should return 100") (newline)
  (cond ((= one two) 3)
        ((= one (* one 1))
         (cond ((null? '(1 2 3)) 4)
               (else (- two one))
               ))
        (else 5)))

(define (test-library)
  (let* ((first (append (list (lambda (x) (+ x 1)) (lambda (y) (+ y 2)))
                        (list (lambda (x) (+ x 3)) (lambda (y) (+ y 4)))))
         (second (map (lambda (fun) (fun 3)) first))
         (third (map (lambda (num) (cadr (assoc num '((4 zebra) (5 owl) (6 giraffe) (7 lion)))))
                     second)))
    (if (or #f (equal? '(((1 2) (3 4) 5)) '(((1 2) (3 4) 5))))
        (cond ((and (equal? 1 1) (equal? 2 3)) #f)
              (else (display "Should return (zebra owl giraffe lion)") (newline)
                    third))
        #f)))