import os
import random
import cPickle
import re

from collections import Counter

import settings
import math

def PreProcessData():

    symbols_to_be_trimmed = '${}()[].,:;+-*/&|<>=~"!'    
    reviews=[]
    
    reviewCategories = {}
    reviewCategories["+"] = "./data_raw/pos"
    reviewCategories["-"] = "./data_raw/neg"
    
    masterCounter = Counter()
    
    for category in reviewCategories:
        folder = reviewCategories[category]
        file_list = os.listdir(folder)
        
        for file_name in file_list:
            filePath = os.path.join(folder, file_name)
            content = open(filePath).read()
            
            #clean special symbols
            content = content.translate(None, symbols_to_be_trimmed) #feature_add
            
            #split into words
            content = re.sub("\s+", " ", content)# introduces one extra ' '
                                    # padding at end, removed in next line
                                    # feature_add
            if(content[-1]==' '): content = content[:-1]
            if(content[0]==' '): content = content[1:]
            lines = content.split(" ")
                        
            #make into Counter
            bag = Counter()
            index=0
            previousWord = "" #feature_add
            for word in lines:
                if word.isdigit(): continue#feature_add
                if(previousWord =="not"): word = "not_" +word #feature_add
                if(word==''):
                    print "Warning: blank word in lines at index: " +str(index)
                    print content
                bag[word] +=1.0
                masterCounter[word]+=1.0
                
                previousWord = word
                index+=1
            del bag["not"] #feature_add
            reviews.append(settings.review(bag, 1.0 if(category=="+") else -1.0))
    
    total_length = sum(masterCounter.values()) #feature_add
        
    ##TF-IDF feature_add
    #---IDF
    docOccurance = Counter()
    for word in list(masterCounter):
        for review in reviews:
            if(review.words[word]>0):
                docOccurance[word]+=1
    #---TF*IDF
    for review in reviews:
        docSize = sum(review.words.values())
        for word in list(review.words):
            review.words[word] = (review.words[word]/docSize)\
                                    *math.log(len(reviews)/docOccurance[word])
        
        #add length of reviews (feature_add)
        review.words["___length_of_review___"] = sum(review.words.values())/total_length
    
    random.shuffle(reviews)
    
    X = [Counter()]*len(reviews)
    y = [0]*len(reviews)
    
    for index in range(len(reviews)):
        X[index] = reviews[index].words
        y[index] = reviews[index].label
    
    cPickle.dump(X, open(settings.path_words, "wb"), cPickle.HIGHEST_PROTOCOL)
    cPickle.dump(y, open(settings.path_labels, "wb"), cPickle.HIGHEST_PROTOCOL)
