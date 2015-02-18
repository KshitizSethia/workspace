from scipy.stats import binom

max_children = 12
total_families = 6115.0

observed_val = range(0,max_children+1)
observed_freq = [7,45,181,478,829,1112,1343,1033,670,286,104,24,3]

total = 0
for index in range(0,max_children+1):
    total+= observed_freq[index]*observed_val[index]

observed_mean = total/total_families
observed_prob_male_child = observed_mean/max_children

my_binomial = binom(max_children, observed_prob_male_child)

squared_error = 0.0
for index in range(0,max_children+1):
    expected_freq = my_binomial.pmf(observed_val[index])*total_families
    squared_error += ((observed_freq[index] - expected_freq)**2)/expected_freq
    

print squared_error