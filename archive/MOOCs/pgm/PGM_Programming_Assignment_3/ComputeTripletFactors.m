function factors = ComputeTripletFactors (images, tripletList, K)
% This function computes the triplet factor values for one word.
%
% Input:
%   images: An array of structs containing the 'img' value for each
%     character in the word.
%   tripletList: An array of the character triplets we will consider (other
%     factor values should be 1). tripletList(i).chars gives character
%     assignment, and triplistList(i).factorVal gives the value for that
%     entry in the factor table.
%   K: The alphabet size (accessible in imageModel.K for the provided
%     imageModel).
%
% Hint: Every character triple in the word will use the same 'val' table.
%   Consider computing that array once and then resusing for each factor.
%
% Copyright (C) Daphne Koller, Stanford University, 2012


n = length(images);

% If the word has fewer than three characters, then return an empty list.
if (n < 3)
    factors = [];
    return
end

factors = repmat(struct('var', [], 'card', [], 'val', []), n - 2, 1);

% Your code here:

chars = vertcat(tripletList.chars);

for index = 1:n-2
	factors(index).var	= [index, index+1, index+2];
	factors(index).card	= [K, K, K];
	for indexForVal = 1:prod(factors(index).card)
		assignment = IndexToAssignment(indexForVal, factors(index).card);
		
		indexInTripletList = intersect(intersect(find(chars(:,1)==assignment(1)), find(chars(:,2)==assignment(2))), find(chars(:,3)==assignment(3)));
		
		if(length(indexInTripletList)!=0)
			factors(index).val(indexForVal) = tripletList(indexInTripletList).factorVal;
		else
			factors(index).val(indexForVal) = 1;
		endif
		
	endfor
endfor

end
