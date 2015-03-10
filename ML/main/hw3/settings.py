path_words = "./data/words.dump"
path_labels = "./data/labels.dump"
#path_corpus = "./data/corpus.dump"
path_best_weights = "./data/best_weights.dump"

num_processes = 2
max_epochs = 1000

class review:
    def __init__(self, words, label):
        self.words = words
        self.label = label