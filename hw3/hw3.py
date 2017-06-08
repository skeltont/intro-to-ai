import string
import math

def parse_training_data():
    vocab = []
    f = open("trainingSet.txt", 'r')
    lines = f.readlines()
    f.close()

    for i, line in enumerate(lines):
        classlabel = line.split('\t')[1]
        words = line.split('\t')[0].split(' ')

        for word in words:
            word = word.translate(None, string.punctuation).lower()
            if (word == ""):
                continue
            if word not in vocab:
                vocab.append(word)
    return sorted(vocab)

def build_feature_data(vocab):
    feature = []
    f = open("trainingSet.txt", 'r')
    lines = f.readlines()
    f.close()

    for i, line in enumerate(lines):
        sentence = []
        element = []
        classlabel = line.split('\t')[1]
        words = line.split('\t')[0].split(' ')

        for word in words:
            word = word.translate(None, string.punctuation).lower()
            if (word == ""):
                continue

            sentence.append(word)

        for v in vocab:
            if v in sentence:
                element.append(1)
            else:
                element.append(0)

        element.append(int(classlabel))
        feature.append(element)

    return feature

def get_prob_c(feature):
    num_pos = 0.0
    count = float(len(feature))
    for f in feature:
        if f[-1] == 1:
            num_pos += 1
    return num_pos / count

def get_word_count(index, feature, c_var = None):
    count = 0
    if index == None:
        return 0

    for f in feature:
        if c_var == None:
            if f[index] == 1:
                count += 1
        else:
            if f[index] == 1 and f[-1] == c_var:
                count += 1

    return count

def run_test(vocab, feature, prob_c):
    pred_correct = 0
    pred_count = 0


    f = open("testSet.txt", 'r')
    lines = f.readlines()
    f.close()

    for i, line in enumerate(lines):
        pos_probs = []
        neg_probs = []

        classlabel = line.split('\t')[1]
        words = line.split('\t')[0].split(' ')

        for word in words:
            pos_p = 0.0
            neg_p = 0.0

            word = word.translate(None, string.punctuation).lower()
            if (word == ""):
                continue

            index = vocab.index(word) if word in vocab else None
            word_count = get_word_count(index, feature)
            pos_word_count = get_word_count(index, feature, 1) + 1
            neg_word_count = get_word_count(index, feature, 0) + 1

            pos_prop = (float(pos_word_count) / float(word_count)) if word_count != 0 else 1.0
            neg_prop = (float(neg_word_count) / float(word_count)) if word_count != 0 else 1.0

            pos_p = pos_prop / (prob_c + float(len(vocab)))
            neg_p = neg_prop / ((1.0 - prob_c) + float(len(vocab)))
            pos_probs.append(pos_p)
            neg_probs.append(neg_p)

        pos_sent = prob_c
        for p in pos_probs:
            pos_sent *= p

        neg_sent = 1.0 - prob_c
        for p in neg_probs:
            neg_sent *= p

        pred_count += 1

        if pos_sent > neg_sent:
            if int(classlabel) == 1:
                pred_correct += 1
        else:
            if int(classlabel) == 0:
                pred_correct += 1

    print "results: "
    print str(pred_correct) +","+ str(pred_count)
    print float(pred_correct) / float(pred_count)


def main():
    vocab = parse_training_data()
    feature = build_feature_data(vocab)
    prob_c = get_prob_c(feature)
    print prob_c
    run_test(vocab, feature, prob_c)


if __name__ == "__main__":
    main()
