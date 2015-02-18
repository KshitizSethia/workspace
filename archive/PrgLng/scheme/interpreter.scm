                            ;;;Mini-Scheme Interpreter


;;; Your first task is to understand this. 

(define (repl)     ;;; the read-eval-print loop.
  (display "--> ") 
  (let ((exp (read)))
    (cond ((equal? exp '(exit))      ; (exit) only allowed at top level
	   'done)
	  (else  (display (top-eval exp))
		 (newline)
		 (repl))
	  )))


(define (my-load filename)       ;; don't want to redefine the Scheme LOAD
  (load-repl (open-input-file filename)))


(define (load-repl port)
  (let ((exp (read port)))
    (cond ((eof-object? exp) 'done)
	  (else (let ((res (top-eval exp)))
		  (display res)
		  (load-repl port)))
	  )))



;; insert!, below, is a destructive update of a list L, inserting the
;; parameter val onto the front of L (so that L is actually modified).
;; insert! must only be used where absolutely necessary, e.g. when an
;; environment must be destructively updated to allow for recursion
;; (see the implementation of (define ...) below).

;; As their names imply, set-car! and set-cdr! destructively modify 
;; the car field and cdr field of a cons cell, respectively. They are
;; built-in functions (see *global-env* below).

(define (insert! val L)
  (set-cdr! L (cons (car L) (cdr L)))
  (set-car! L val)
  )


;; (define ....) is only allowed at the top level and affects only the 
;; global environment. Only the basic form of define is supported here.
; TODO adding define for functions

(define (top-eval exp)
  (cond ((not (pair? exp)) (my-eval exp *global-env*))
  ((eq? (car exp) 'define)
	 (cond ((pair? (cadr exp))
             (top-eval (cons 'define
                             (cons (caadr exp)
                                   (list (cons 'lambda 
                                               (cons (cdadr exp)
                                                     (cddr exp)
                                                )
                                          )
                                    )
                              )
                        )
              )
           )
          (else (insert! (list (cadr exp) (my-eval (caddr exp) *global-env*)) *global-env*)
                (cadr exp) ; just return the symbol being defined
           )
     )
    )
	(else (my-eval exp *global-env*))
	))


(define (lookup var env)
  (let ((item (assoc var env)))  ;; assoc returns #f if var not found in env
    (cond ((not item) (display "Error: Undefined Symbol ")
		      (display var) (newline))
	  (else (cadr item))
	  )))

(define (handle-if test then-exp else-exp env)
  (if (my-eval test env)
      (my-eval then-exp env)
      (my-eval else-exp env)))


;; still missing let, let*, letrec, the syntax for (define (f x) ...),
;; cond, begin (block).
; TODO let let* letrec cond

(define (my-eval exp env)
  (cond
   ((symbol? exp) (lookup exp env))
   ((not (pair? exp)) exp)
   ((eq? (car exp) 'quote) (cadr exp))
   ((eq? (car exp) 'if)
    (handle-if (cadr exp) (caddr exp) (cadddr exp) env))
   ((eq? (car exp) 'cond)
    (handle-cond (cdr exp) env)
    )
   ((eq? (car exp) 'lambda)
    (list 'closure exp env))
   ((eq? (car exp) 'letrec)
    (handle-letrec (cadr exp) (cddr exp) env))  ;; see explanation below
   ((eq? (car exp) 'let) 
     (handle-let (cadr exp) (cddr exp) env )
     )
   ((eq? (car exp) 'let*) 
     (handle-let* (cadr exp) (cddr exp) env )
     )
   ((eq? (car exp) 'longest) 
     (handle-longest-driver (cdr exp) env )
     )
   (else
    (handle-call (map (lambda (sub-exp) (my-eval sub-exp env)) exp)))
   ))


(define (bind formals actuals)
  (cond ((null? formals) '())
	(else (cons (list (car formals) (car actuals))
		    (bind (cdr formals) (cdr actuals))))
	))

(define (handle-longest expressions env)
	(cond 
		( (null? expressions) '(0 '()) )
		(else
			(let 
				(
					(longest_expr_length_result 
						(handle-longest (cdr expressions) env)
					)
					(this_expr_length_result 
                                         (let 
                                             (
                                              (expr_to_eval (caar expressions))
                                              (this_result (cadar expressions))
                                           )
                                           (list 
                                            (length (my-eval expr_to_eval env))
                                            (my-eval this_result env)
                                            )
                                           )
					)
				)
				(if (> (car longest_expr_length_result) (car this_expr_length_result))
					longest_expr_length_result
					this_expr_length_result				)
			)
	   )
	)
 )

(define (handle-longest-driver expressions env)
  (cadr (handle-longest expressions env))
  )

(define (handle-block block env)
  (cond ((null? block) (display "Error: Can't have empty block or body"))
	((null? (cdr block)) (my-eval (car block) env))
	(else (my-eval (car block) env)
	      (handle-block (cdr block) env))
	))
    
(define (handle-call evald-exps)
  (let ((fn (car evald-exps))
	(args (cdr evald-exps)))
   (cond
    ((eq? (car fn) 'closure)
     (let ((formals (cadr (cadr fn)))
	   (body (cddr (cadr fn)))
	   (env (caddr fn)))
       (handle-block body (append (bind formals args) env))))
    ((eq? (car fn) 'primitive-function)
     (apply (cadr fn) args))
    (else (display "Error: Calling non-function"))
    )))

;;-------------------define my functions here-----------------------

;-----let-----

(define (create_let_env variables simplified_variable_values env)
  (cond ((null? variables) env)
        (else (let ((new_env (cons (list (car variables)
                                         (car simplified_variable_values)
                                    )
                                    env
                             )
                     ))
                   (create_let_env (cdr variables)
                                   (cdr simplified_variable_values)
                                    new_env)
               )
         )
   )
 )
 
( define (simplify_expressions_for_vars unevald_exps env)
   (cond ((null? unevald_exps) '()
          )
         (else (cons (my-eval (car unevald_exps) env)
                     (simplify_expressions_for_vars (cdr unevald_exps) env)
                )
          )
    )
 )
  
(define (handle-let declarations body env)
  (let ( (variables (map car declarations))
         (expressions_for_variables (map cadr declarations))
        )
       (let ((simplified_env 
                (create_let_env variables 
                                  (simplify_expressions_for_vars expressions_for_variables 
                                                                 env) 
                                  env)
             ))
            (handle-block body simplified_env))
  )
)

;-----let*-----

(define (create_let*_env variables variable_exps env)
  (cond ((null? variables) env)
        (else (let ((new_env (cons (list (car variables)
                                         (my-eval (car variable_exps) env)
                                    )
                                    env
                             )
                     ))
                   (create_let*_env (cdr variables)
                                   (cdr variable_exps)
                                    new_env)
               )
         )
   )
 )
  
(define (handle-let* declarations body env)
  (let ( (variables (map car declarations))
         (expressions_for_variables (map cadr declarations))
        )
       (let ((simplified_env 
                (create_let*_env variables 
                                  expressions_for_variables 
                                  env)
             ))
            (handle-block body simplified_env))
  )
)

;-----letrec-----

; Here's how handle-letrec should implement LETREC
; 0) The parameters are the defs,(e.g. ((f exp1) (g exp2)), and the body,
;    which is a list of expressions, e.g. ((display x) (f (g 1)))
; 1) create an association list binding the new names introducted by
;    the letrec to uninitialized values (e.g. the symbol '*uninitialized*).
;    For example, if the new names are x and y, then create 
;    ((x *uninitialized*) (y *uninitialized*))
; 2) create a new-env by appending the above association list to env.
; 3) eval the right hand side of each def using new-env
; 4) destructively modify new-env to replace the unitialized value for each
;    new name with its correspondinng value.
; 5) evaluate the body of the letrec using new-env

(define (create_placeholders variables)
	(cond ((null? variables) '())
		  (else (cons (list (car variables) '*uninitialized*)
					  (create_placeholders (cdr variables))
				 )
		   )
	)
)

(define (update_placeholders variables evald_exps env)
	(cond ((null? variables))
		  (else (let ( (variable (assoc (car variables) env))
					  )
					 (set-cdr! variable (list (car evald_exps)))
					 (update_placeholders (cdr variables) (cdr evald_exps) env)
				 )
		   )
	 )
)

(define (handle-letrec declarations body env)
  (let ( (variables (map car declarations))
         (expressions_for_variables (map cadr declarations))
        )
       (let ((simplified_env 
                (append (create_placeholders variables) env)
             ))
            (update_placeholders variables 
                                 (simplify_expressions_for_vars expressions_for_variables 
                                                                simplified_env) 
                                 simplified_env
             )
            (handle-block body simplified_env)
        )
  )
)

;-----cond-----

(define (handle-cond expressions env)
  (if (null? expressions) 
      (newline)
      (if (eq? (caar expressions) 'else) 
          (handle-block (cdar expressions) env)
          (if (eq? (my-eval (caar expressions) env) #t)
              (handle-block (cdar expressions) env)
              (handle-cond (cdr expressions) env)
              )
       )
   )
)

;-----apply-----
(define (my_apply function arguments)
  (cond ((eq? (car function) 'primitive-function) (apply (cadr function) arguments
                                                   )
         )
        (else (handle-call (cons function arguments)
               )
         )
   )
)

;;-------------------- Here is the initial global environment --------

(define *global-env*
  (list (list 'car (list 'primitive-function car))
	(list 'cdr (list 'primitive-function cdr))
	(list 'set-car! (list 'primitive-function set-car!))
	(list 'set-cdr! (list 'primitive-function set-cdr!))
	(list 'cons (list 'primitive-function cons))
	(list 'list (list 'primitive-function list))
	(list '+ (list 'primitive-function +))
	(list '- (list 'primitive-function -))
	(list '* (list 'primitive-function *))
	(list '= (list 'primitive-function =))
	(list '< (list 'primitive-function <))
	(list '> (list 'primitive-function >))
	(list '<= (list 'primitive-function  <=))
	(list '>= (list 'primitive-function >=))
	(list 'eq? (list 'primitive-function eq?))
	(list 'pair? (list 'primitive-function pair?))
	(list 'symbol? (list 'primitive-function symbol?))
	(list 'null? (list 'primitive-function null?))
	(list 'read (list 'primitive-function read))
	(list 'display (list 'primitive-function  display))
	(list 'open-input-file (list 'primitive-function open-input-file))
	(list 'close-input-port (list 'primitive-function close-input-port))
	(list 'eof-object? (list 'primitive-function eof-object?))
	(list 'load (list 'primitive-function my-load))  ;;defined above
   (list 'newline (list 'primitive-function newline))
   (list 'apply (list 'primitive-function my_apply))
   (list 'length (list 'primitive-function length))
	))