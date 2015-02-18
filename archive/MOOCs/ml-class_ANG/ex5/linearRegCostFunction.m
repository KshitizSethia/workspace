function [J, grad] = linearRegCostFunction(X, y, theta, lambda)
%LINEARREGCOSTFUNCTION Compute cost and gradient for regularized linear 
%regression with multiple variables
%   [J, grad] = LINEARREGCOSTFUNCTION(X, y, theta, lambda) computes the 
%   cost of using theta as the parameter for linear regression to fit the 
%   data points in X and y. Returns the cost in J and the gradient in grad

% Initialize some useful values
m = length(y); % number of training examples

% You need to return the following variables correctly 
J = 0;
grad = zeros(size(theta));

% ====================== YOUR CODE HERE ======================
% Instructions: Compute the cost and gradient of regularized linear 
%               regression for a particular choice of theta.
%
%               You should set J to the cost and grad to the gradient.

%remove bias entry from theta
theta_noBias = theta(2:end);
%predicted value
h_theta = X*theta;

%regularized cost
J = ( (sum((h_theta-y).^2))+(sum(theta_noBias.^2))*lambda )/(2*m);

%regularized gradient
%grad =[ (sum((h_theta-y)))/m ; ((sum((h_theta(2:end) - y(2:end)).*X(2:end)))/m)+ (lambda*theta(2:end)/m)];
%grad =[ (sum((h_theta-y)))/m ; ((sum((h_theta-y).*X(:, 2:end)))/m)+ (lambda*theta(2:end)/m)];

%gradient
grad = (X'*(h_theta-y))/m;

%regularized gradient
grad = [grad(1) ; grad(2:end)+(lambda*theta(2:end)/m)];

% =========================================================================

grad = grad(:);

end
