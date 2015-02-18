%CLIQUETREECALIBRATE Performs sum-product or max-product algorithm for 
%clique tree calibration.

%   P = CLIQUETREECALIBRATE(P, isMax) calibrates a given clique tree, P 
%   according to the value of isMax flag. If isMax is 1, it uses max-sum
%   message passing, otherwise uses sum-product. This function 
%   returns the clique tree where the .val for each clique in .cliqueList
%   is set to the final calibrated potentials.
%
% Copyright (C) Daphne Koller, Stanford University, 2012

function P = CliqueTreeCalibrate(P, isMax)


% Number of cliques in the tree.
N = length(P.cliqueList);

% Setting up the messages that will be passed.
% messages(i,j) represents the message going from clique i to clique j. 
messages = repmat(struct('var', [], 'card', [], 'val', []), N, N);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% We have split the coding part for this function in two chunks with
% specific comments. This will make implementation much easier.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% YOUR CODE HERE
% While there are ready cliques to pass messages between, keep passing
% messages. Use GetNextCliques to find cliques to pass messages between.
% Once you have clique i that is ready to send message to clique
% j, compute the message and put it in messages(i,j).
% Remember that you only need an upward pass and a downward pass.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% if max sum, then convert all factors to log
if(isMax==1)
	for cliqueFactor = P.cliqueList
		cliqueFactor.val = log(cliqueFactor.val);
	endfor
endif

% Calculate all messages
[i,j] = GetNextCliques(P, messages);
while(i!=0)
	messages(i,j) = P.cliqueList(i);
	incomingEdgesIntoI = P.edges(:, i);
	incomingEdgesIntoI(j)=0;
	idx = find(incomingEdgesIntoI);
	if(!isempty(idx))
		for messageSendingClique = idx(:)'
			if(isMax==0)
				messages(i,j) = FactorProduct(messages(i,j), messages(messageSendingClique, i));
			else
				messages(i,j) = FactorSum(messages(i,j), messages(messageSendingClique, i));
			endif
		endfor
	endif
	
	if(isMax==0)
		messages(i,j) = FactorMarginalization(messages(i,j), setdiff(messages(i,j).var,P.cliqueList(j).var));
		%normalize
		messages(i,j).val = messages(i,j).val ./ sum(messages(i,j).val);
	else
		messages(i,j) = FactorMaxMarginalization(messages(i,j), setdiff(messages(i,j).var,P.cliqueList(j).var));
	endif
	%initialize i,j for next iteration
	[i,j] = GetNextCliques(P, messages);
endwhile
 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% YOUR CODE HERE
%
% Now the clique tree has been calibrated. 
% Compute the final potentials for the cliques and place them in P.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

if(isMax==0)
	for cliqueIndex = 1:N
		idx = find(P.edges(:, cliqueIndex));
		for messageIndex = idx(:)'
			P.cliqueList(cliqueIndex) = FactorProduct(P.cliqueList(cliqueIndex), messages(messageIndex, cliqueIndex));
		endfor
	endfor
else	
	for cliqueIndex = 1:N
		idx = find(P.edges(:, cliqueIndex));
		for messageIndex = idx(:)'
			P.cliqueList(cliqueIndex) = FactorSum(P.cliqueList(cliqueIndex), messages(messageIndex, cliqueIndex));
		endfor
	endfor
endif

return
