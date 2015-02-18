function card = GetCardinality(factorList, varNumber)
	for index = 1:length(factorList)
		[temp, idx] = intersect(factorList(index).var, varNumber);
		if(length(temp)>0)
			card = factorList(index).card(idx);
			return;
		endif
	endfor
end