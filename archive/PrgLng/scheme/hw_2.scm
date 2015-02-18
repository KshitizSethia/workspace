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
                                            (length (eval expr_to_eval env))
                                            (eval this_result env)
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

(
	handle-longest-driver 
	'(
		(
			'(1 2 3) 
			(+ 1 2 3)
		)
		(
			(cdr '(1 2 3 4 5)) 
			'yes
		)
		(
			(cons 8 (list 9 10)) 
			'no
		)
	)
    (scheme-report-environment 5)
)

