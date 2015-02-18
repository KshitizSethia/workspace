%NAIVEGETNEXTCLUSTERS Takes in a node adjacency matrix and returns the indices
%   of the nodes between which the m+1th message should be passed.
%
%   Output [i j]
%     i = the origin of the m+1th message
%     j = the destination of the m+1th message
%
%   This method should iterate over the messages in increasing order where
%   messages are sorted in ascending ordered by their destination index and 
%   ties are broken based on the origin index. (note: this differs from PA4's
%   ordering)
%
%   Thus, if m is 0, [i j] will be the pair of clusters with the lowest j value
%   and (of those pairs over this j) lowest i value as this is the 'first'
%   element in our ordering. (this difference is because matlab is 1-indexed)
%
% Copyright (C) Daphne Koller, Stanford University, 2012

function [i, j] = NaiveGetNextClusters(P, m)

    i = size(P.clusterList,1);
    j = size(P.clusterList,1);
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % YOUR CODE HERE
    % Find the indices between which to pass a cluster
    % The 'find' function may be useful
    %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	
	numClusters = length(P.clusterList);
	m=m+1;
	columnNumber=1;
	while(m>0 && columnNumber<=numClusters)
		m = m-length(find(P.edges(:,columnNumber)));
		columnNumber++;
	endwhile
	
	columnNumber--;
	
	idx = find(P.edges(:,columnNumber));
	m = m+ length(idx);
	if(m<=length(idx))
		j = columnNumber;
		i = idx(m);
	endif
	%columnNum = floor((m+1)/(numClusters-1))+1;
	%rowNum =  rem(m+1, numClusters-1);
	%if(rowNum==0)
	%	rowNum = numClusters-1;
	%endif
	%
	%j = columnNum;
	%i = (setdiff(1:numClusters, j))(rowNum);
	
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

end
