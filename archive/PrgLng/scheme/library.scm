(define (map F X)
  (cond ((null? X) '())
  (else (cons (F (car X)) (map F (cdr X))))))


(define (equal? L1 L2)
  (cond ((and (null? L1) (null? L2)) #t)
        ((and (pair? L1) (pair? L2)) (if (equal? (car L1) (car L2)) (equal? (cdr L1) (cdr L2)) #f))
        ((and (not (pair? L1)) (not (pair? L2))) (if (eq? L1 L2) #t #f))
        (else #f)))


(define (assoc val L)
  (cond ((null? L) #f)
        ((equal? val (caar L)) (car L))
        (else (assoc val (cdr L)))))


(define (and e1 e2)
  (if e1 e2 #f))


(define (or e1 e2)
  (if e1 #t e2))


(define (append L1 L2)
  (if (null? L1) L2
      (cons (car L1) (append (cdr L1) L2))))


(define (not x)
  (if x #f #t))

(define (caadr L)
  (car (car (cdr L)))
  )

(define (cdadr L)
  (cdr (car (cdr L)))
  )

(define (caar L)
  (car (car L)))
  
(define (cadr L)
  (car (cdr L)))
  
(define (cddr L)
  (cdr (cdr L)))

(define (cadar L)
  (car (cdar L))
  )

(define (cdar L)
  (cdr (car L)))

(define (caddr L)
  (car (cdr (cdr L))))

(define (cadddr L)
  (car (cdr (cdr (cdr L)))))