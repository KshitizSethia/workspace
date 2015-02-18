%COMPUTEINITIALPOTENTIALS Sets up the cliques in the clique tree that is
%passed in as a parameter.
%
%   P = COMPUTEINITIALPOTENTIALS(C) Takes the clique tree skeleton C which is a
%   struct with three fields:
%   - nodes: cell array representing the cliques in the tree.
%   - edges: represents the adjacency matrix of the tree.
%   - factorList: represents the list of factors that were used to build
%   the tree. 
%   
%   It returns the standard form of a clique tree P that we will use through 
%   the rest of the assigment. P is struct with two fields:
%   - cliqueList: represents an array of cliques with appropriate factors 
%   from factorList assigned to each clique. Where the .val of each clique
%   is initialized to the initial potential of that clique.
%   - edges: represents the adjacency matrix of the tree. 
%
% Copyright (C) Daphne Koller, Stanford University, 2012


function P = ComputeInitialPotentials(C)

% number of cliques
N = length(C.nodes);

% initialize cluster potentials 
P.cliqueList = repmat(struct('var', [], 'card', [], 'val', []), N, 1);
P.edges = zeros(N);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% YOUR CODE HERE
%
% First, compute an assignment of factors from factorList to cliques. 
% Then use that assignment to initialize the cliques in cliqueList to 
% their initial potentials. 

% C.nodes is a list of cliques.
% So in your code, you should start with: P.cliqueList(i).var = C.nodes{i};
% Print out C to get a better understanding of its structure.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

P.edges = C.edges;

usedFactors = [];

varNumbers = [];
cardForVarNumbers = [];

for cliqueIndex = 1:N
	cliqueScope = C.nodes{cliqueIndex};
	factorsInThisClique = [];
	
	for factorIndex = 1:length(C.factorList)
		factorScope = C.factorList(factorIndex).var;
		if(
			all(usedFactors != factorIndex)
			&&
			(length(factorScope) <= length(cliqueScope))
			&&
			(length(intersect(factorScope, cliqueScope)) == length(factorScope))
		  )
			factorsInThisClique(end+1) = factorIndex;
			usedFactors(end+1) = factorIndex;
		 endif
		 
		 for varIndex = 1:length(factorScope)
			if(all(varNumbers != factorScope(varIndex)))
				varNumbers(end+1)		= factorScope(varIndex);
				cardForVarNumbers(end+1)= C.factorList(factorIndex).card(varIndex);
			endif
		 endfor
		 
	endfor
	
	cliqueFactor = struct('var', [], 'card', [], 'val', []);
	if(length(factorsInThisClique)>0)
		cliqueFactor = C.factorList(factorsInThisClique(1));
		for index = 2:length(factorsInThisClique)
			cliqueFactor = FactorProduct(cliqueFactor, C.factorList(factorsInThisClique(index)));
		endfor
	else
		cliqueFactor.var = cliqueScope;
		[temp,idx] = intersect(varNumbers, cliqueFactor.var);
		cliqueFactor.card = cardForVarNumbers(idx);
		cliqueFactor.val = ones(1,prod(cliqueFactor.card));
		%cliqueFactor.val=0;
		
		P.edges(:,cliqueIndex)=0;
		P.edges(cliqueIndex,:)=0;
		
	endif
	 
	P.cliqueList(cliqueIndex) = StandardizeFactors(cliqueFactor);

endfor

end
