%ComputeMarginal Computes the marginal over a set of given variables
%   M = ComputeMarginal(V, F, E) computes the marginal over variables V
%   in the distribution induced by the set of factors F, given evidence E
%
%   M is a factor containing the marginal over variables V
%   V is a vector containing the variables in the marginal e.g. [1 2 3] for
%     X_1, X_2 and X_3.
%   F is a vector of factors (struct array) containing the factors 
%     defining the distribution
%   E is an N-by-2 matrix, each row being a variable/value pair. 
%     Variables are in the first column and values are in the second column.
%     If there is no evidence, pass in the empty matrix [] for E.


function M = ComputeMarginal(V, F, E)

% Check for empty factor list
if (numel(F) == 0)
      warning('Warning: empty factor list');
      M = struct('var', [], 'card', [], 'val', []);      
      return;
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% YOUR CODE HERE:
% M should be a factor
% Remember to renormalize the entries of M!
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% zero variables disagreeing with E
F = ObserveEvidence(F, E);

for chosenFactor = 1:length(F)
	% normalize result
	F(chosenFactor).val = F(chosenFactor).val./(sum(F(chosenFactor).val));
	
	if(rows(E)>0)
		[dummy1, indxOfVarsToRemove, dummy2] = intersect(F(chosenFactor).var, E(:, 1));
		%indxOfVarsToKeep = 1-indxOfVarsToRemove;
		
		%F(chosenFactor).var(:,indxOfVarsToRemove) = [];
		F(chosenFactor).card(:,indxOfVarsToRemove) = 1;
	end
	
	%[dummy1, indxOfValsToRemove, dummy2] = intersect(F(chosenFactor).val,[0]);
	%F(chosenFactor).val(:, indxOfValsToRemove) = [];
	
	F(chosenFactor).val = F(chosenFactor).val(:, F(chosenFactor).val(1,:)!=0);
	
	%F(chosenFactor).var = setdiff(F(chosenFactor).var,F(chosenFactor).var(indxOfVarsToRemove,:));
	%F(chosenFactor).card = setdiff(F(chosenFactor).card,F(chosenFactor).card(indxOfVarsToRemove,:));
end

M = ComputeJointDistribution(F);
M.val = M.val./sum(M.val);
M = FactorMarginalization(M, setdiff(M.var, V));

%M = struct('var', [], 'card', [], 'val', []); % Returns empty factor. Change this.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

M = StandardizeFactors(M);

end
