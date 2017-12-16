import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        with open(positives) as f:
            self.positives = f.readlines()[35:]
        for i in range(len(self.positives)):
            self.positives[i] = self.positives[i].strip('\n')
        with open(negatives) as f:
            self.negatives = f.readlines()[35:]
        for i in range(len(self.negatives)):
            self.negatives[i] = self.negatives[i].strip('\n')            

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        score = 0
        for word in tokens:
            if word.lower() in self.positives:
                score += 1
            if word.lower() in self.negatives:
                score -= 1
        return score


def main():
    analyze = Analyzer('./positive-words.txt', './negative-words.txt')
    print(analyze.analyze('I hate to love you'))
    
if __name__ == '__main__':
    main()