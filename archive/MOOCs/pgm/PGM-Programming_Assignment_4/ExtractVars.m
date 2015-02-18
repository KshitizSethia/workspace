function vars = ExtractVars(factors)
	vars = [];
	
	for factor = factors(:)'
		vars = union(vars, factor.var);
	endfor
end