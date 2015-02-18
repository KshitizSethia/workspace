function result = GetMessagePassingMatrix(messages)

	result = zeros(size(messages));
	for row = 1:rows(messages)
		for col = 1:columns(messages)
			if(length(messages(row,col).var)!=0)
				result(row, col) = 1;
			endif
		endfor
	endfor

end