function [C, sigma] = dataset3Params(X, y, Xval, yval)
%EX6PARAMS returns your choice of C and sigma for Part 3 of the exercise
%where you select the optimal (C, sigma) learning parameters to use for SVM
%with RBF kernel
%   [C, sigma] = EX6PARAMS(X, y, Xval, yval) returns your choice of C and 
%   sigma. You should complete this function to return the optimal C and 
%   sigma based on a cross-validation set.
%

% You need to return the following variables correctly.
C = 1;
sigma = 0.3;

% ====================== YOUR CODE HERE ======================
% Instructions: Fill in this function to return the optimal C and sigma
%               learning parameters found using the cross validation set.
%               You can use svmPredict to predict the labels on the cross
%               validation set. For example, 
%                   predictions = svmPredict(model, Xval);
%               will return the predictions on the cross validation set.
%
%  Note: You can compute the prediction error using 
%        mean(double(predictions ~= yval))

% values of C and sigma which will be looked into
C_choices = sigma_choices = [0.01, 0.03, 0.1, 0.3, 1, 3, 10, 30];

% this variable will store the error gotten by choosing certain values of
% C and sigma, the min element's corresponding row and column will be used
% to get best combination of C and sigma
error = zeros(length(C_choices), length(sigma_choices));

% calculate and store error on each combination of C and sigma.
for C_index = 1: length(C_choices)
	for sigma_index = 1: length(sigma_choices)
		% sigma value for this iteration
		sigma_iter = sigma_choices(sigma_index);
		
		% C value for this iteration
		C_iter = C_choices(C_index);
		
		% getting model for this combo of C and sigma
		model_iter = svmTrain(X, y, C_iter, @(x1, x2) gaussianKernel(x1, x2, sigma_iter));
		
		% getting predictions for model for this iteration
		predictions_iter = svmPredict(model_iter, Xval);
		
		% cacluating and storing error values for this combo of C and sigma
		error(C_index, sigma_index) = mean(double(predictions_iter ~= yval));
	endfor
endfor

% locate the min error in error matrix

% get index in flattened error matrix
[val_temp, idx_temp] = min(error(:));

% get row and column of min element
row_temp = mod(idx_temp, size(error, 1));
column_temp = ceil(idx_temp/size(error, 1));

% fetch corresponding C and sigma values
C = C_choices(row_temp);
sigma = sigma_choices(column_temp);

% =========================================================================

end
