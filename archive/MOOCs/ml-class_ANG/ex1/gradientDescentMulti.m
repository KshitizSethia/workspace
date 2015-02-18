function [theta, J_history] = gradientDescentMulti(X, y, theta, alpha, num_iters)
%GRADIENTDESCENTMULTI Performs gradient descent to learn theta
%   theta = GRADIENTDESCENTMULTI(x, y, theta, alpha, num_iters) updates theta by
%   taking num_iters gradient steps with learning rate alpha

% Initialize some useful values
m = length(y); % number of training examples
n = columns(X);
J_history = zeros(num_iters, 1);

for iter = 1:num_iters

    % ====================== YOUR CODE HERE ======================
    % Instructions: Perform a single gradient step on the parameter vector
    %               theta. 
    %
    % Hint: While debugging, it can be useful to print out the values
    %       of the cost function (computeCostMulti) and gradient here.
    %

	% Save the cost J in every iteration    
    J_history(iter) = computeCost(X, y, theta);
	
	%back-up theta in case of increase in cost
	theta_old = theta;
	
	%update theta for next iteration
	assumed_value = X*theta;
    for feature = 1:n
		x_for_feature = X(:, feature);
		theta(feature) = theta(feature) - ( (alpha/m) * sum ( (assumed_value - y) .* x_for_feature ) );
	end
	
	%break out if cost increases
	if(iter>1 && (J_history(iter-1) < J_history(iter)))
		theta = theta_old;
		J_history(iter) = 0;
		break;
	endif

end

end
