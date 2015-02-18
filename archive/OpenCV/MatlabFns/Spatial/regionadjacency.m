% REGIONADJACENCY Computes adjacency matrix for image of labeled segmented regions
%
% Usage:  [Am, Al] = regionadjacency(L, connectivity)
%
% Arguments:  L - A region segmented image, such as might be produced by a
%                 graph cut or superpixel algorithm.  All pixels in each
%                 region are labeled by an integer.
%  connectivity - 8 or 4.  If not specified connectivity defaults to 8.
%
% Returns:   Am - An adjacency matrix indicating which labeled regions are
%                 adjacent to each other, that is, they share boundaries.
%            Al - A cell array representing the adjacency list corresponding
%                 to Am.  Al{n} is an array of the region indices adjacent to
%                 region n.
%
% Regions with a label of 0 are not processed. They are considered to be
% 'background regions' that are not to be considered.  If you want to include
% these regions you should assign a new positive label to these areas using, say
% >> L(L==0) = max(L(:)) + 1;
%
% See also: CLEANUPREGIONS, RENUMBERREGIONS, SLIC

% Copyright (c) 2013 Peter Kovesi
% Centre for Exploration Targeting
% School of Earth and Environment
% The University of Western Australia
% peter.kovesi at uwa edu au
%
% Permission is hereby granted, free of charge, to any person obtaining a copy
% of this software and associated documentation files (the "Software"), to deal
% in the Software without restriction, subject to the following conditions:
% 
% The above copyright notice and this permission notice shall be included in 
% all copies or substantial portions of the Software.
%
% February 2013

function  [Am, Al] = regionadjacency(L, connectivity)

    if ~exist('connectivity', 'var'), connectivity = 8; end
    [rows,cols] = size(L);
    
    % Identify the unique labels in the image, excluding 0 as a label.
    labels = setdiff(unique(L(:))',0);

    if isempty(labels)
        warning('There are no objects in the image')
        Am = [];
        Al = {};
        return
    end

    N = max(labels);    
    Am = zeros(N);     % Allocate adjacency matrix
    
    % Strategy:  Step through the labeled image.  For 8-connectedness inspect 
    % pixels as follows and set the appropriate entries in the adjacency
    % matrix. 
    %      x - o
    %    / | \
    %  o   o   o
    %
    % For 4-connectedness we only inspect the following pixels
    %      x - o
    %      | 
    %      o  

    if connectivity == 8
        for r = 1:rows-1
            for c = 1:cols-1
                if c==1
                    Am(L(r,c), L(r  ,c+1)) = 1;
                    Am(L(r,c), L(r+1,c  )) = 1;
                    Am(L(r,c), L(r+1,c+1)) = 1;
                else
                    Am(L(r,c), L(r  ,c+1)) = 1;
                    Am(L(r,c), L(r+1,c-1)) = 1;
                    Am(L(r,c), L(r+1,c  )) = 1;
                    Am(L(r,c), L(r+1,c+1)) = 1;
                end
            end
        end
    
    elseif connectivity == 4
        for r = 1:rows-1
            for c = 1:cols-1
                Am(L(r,c), L(r  ,c+1)) = 1;
                Am(L(r,c), L(r+1,c  )) = 1;
            end
        end
    
    else
        error('Connectivity bust be 4 otr 8');
    end
    
    % Zero out the diagonal 
    for r = 1:N
        Am(r,r) = 0;
    end
    
    % Ensure connectivity both ways for all regions.
    Am = Am | Am';
    
    % If an adjacency list is requested...
    if nargout == 2
        Al = cell(N,1);
        for r = 1:N
            Al{r} = find(Am(r,:));
        end
    end
    
%    ** sort out varargout **