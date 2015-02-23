from collections import Counter
class params:
    def __init__(self
                 , lambda_reg
                 , max_epochs
                 , checkGradient):
        self.lambda_reg = lambda_reg
        self.max_epochs = max_epochs
        self.checkGradient = checkGradient

# only works for stochastic, as we will have
# only vectors (of size: number of features)
def bag_dot(A_vec, B_vec):
    result = 0
    A_l = list(A_vec)
    B_l = list(B_vec)
    if(len(B_l)<len(A_l)): return bag_dot(B_vec, A_vec)
    return sum(B_vec.get(f, 0) * v for f, v in A_vec.items())
    #for word in A_l:
    #    result+= A_vec[word]*B_vec[word]
    #return result       

def bag_mult(scalar, vector):
    return Counter(dict([(k, v*scalar) for (k, v) in vector.iteritems()]))

def increment(d1, scale, d2):
    """
    Implements d1 += scale * d2 for sparse vectors.
    @param dict d1: the feature vector which is mutated.
    @param float scale
    @param dict d2: a feature vector.
    """
    for f, v in d2.items():
        d1[f] = d1.get(f, 0) + v * scale

def loss_hinge(X, y, theta):
    term = 0.0
    for index in range(len(y)):
        term+=max(0,(1-(y[index]*bag_dot(theta, X[index]))))
    term = term/len(y)
    #term += lambda_reg*bag_dot(theta, theta)
    return term

def loss_01(X,y,theta):
    term = 0.0
    for index in range(len(y)):
        if((y[index]*bag_dot(theta, X[index]))<0):
            term+=1
    return term*100.0/len(y)

def loss_gradient(X,y,theta, lambda_reg):
    term = bag_dot(X,theta) - y
    term = bag_dot(term, X)
    
def run(X,y,params):
    epoch=0
    t=1.0
    weights = Counter()
    weights_scale = 1
    while(epoch<params.max_epochs):
        for index in range(len(y)):
            t+=1.0
            term = bag_dot(X[index], weights)*y[index]*weights_scale
            weights_scale *= (1-(1/t))
            if(term<1):
                step_size = 1/float(t*params.lambda_reg)
                increment(weights, float(step_size*y[index]/weights_scale), X[index])
        epoch+=1
        
    return bag_mult(weights_scale, weights)
