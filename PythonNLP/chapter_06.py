
# -*- coding: utf-8 -*-

import nltk
import random
from nltk.corpus import movie_reviews
from nltk.corpus import names

# 从所有文本中取出最频繁的2000个单词作为词特征
word_freq = nltk.FreqDist(w.lower() for w in movie_reviews.words())
word_freq_most = word_freq.most_common(2000)
word_features = [word for (word, count) in word_freq_most]


def doc_features(doc):
    """
    文本特征提取
    :param doc: 文本
    :return: 文本特征
    """
    doc_words = set([w.lower() for w in doc])
    features = {}
    for w in word_features:
        features['contain(%s)' % w] = (w in doc_words)
    return features


def doc_classify():
    documents = [(list(movie_reviews.words(fileid)), category)
                 for category in movie_reviews.categories()
                 for fileid in movie_reviews.fileids(category)]
    random.shuffle(documents)

    features_set = [(doc_features(words), c) for(words, c) in documents]
    train_set_size = int(len(features_set) * 0.6)
    train_set = features_set[:train_set_size]
    test_set = features_set[train_set_size:]

    classifier = nltk.NaiveBayesClassifier.train(train_set)
    print(nltk.classify.accuracy(classifier, test_set))
    classifier.show_most_informative_features(5)

doc_classify()







