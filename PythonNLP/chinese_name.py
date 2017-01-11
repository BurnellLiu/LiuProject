
import nltk
import random

from nltk.classify import apply_features
from nltk.corpus import PlaintextCorpusReader

# 从所有名字中取出最频繁的2000个字作为字特征
names_corpus = PlaintextCorpusReader('./', ['female.txt', 'male.txt'])
all_names = names_corpus.words()

ch_freq = nltk.FreqDist(ch.lower() for name in all_names for ch in name)
ch_freq_most = ch_freq.most_common(1000)
ch_features = [ch for (ch, count) in ch_freq_most]


def name_features(name):
    """
    名称特征提取
    :param name: 名称
    :return: 名称特征
    """
    name_chs = set([ch.lower() for ch in name])
    features = {}
    for ch in ch_features:
        features['contain(%s)' % ch] = (ch in name_chs)
    return features


def name_classify():
    female_names = [(name, 'female')for name in names_corpus.words('female.txt')]
    male_names = [(name, 'male')for name in names_corpus.words('male.txt')]

    total_names = female_names + male_names
    random.shuffle(total_names)
    train_set_size = int(len(total_names) * 0.6)
    train_names = total_names[:train_set_size]
    test_names = total_names[train_set_size:]

    train_set = apply_features(name_features, train_names, True)
    test_set = apply_features(name_features, test_names, True)

    classifier = nltk.NaiveBayesClassifier.train(train_set)
    print(nltk.classify.accuracy(classifier, train_set))
    print(nltk.classify.accuracy(classifier, test_set))

    for (name, tag) in test_names:
        guess = classifier.classify(name_features(name))
        if guess != tag:
            print(tag, guess, name)

    classifier.show_most_informative_features(20)

    # female_features_set = [(name_features(name), 'female') for name in female_names]
    # male_features_set = [(name_features(name), 'male') for name in male_names]
    # print(female_features_set[0])
    # print(male_features_set[0])

    # features_set = female_features_set + male_features_set
    # train_set_size = int(len(features_set) * 0.6)
    # train_set = features_set[:train_set_size]
    # test_set = features_set[train_set_size:]

    # classifier = nltk.NaiveBayesClassifier.train(train_set)
    # print(nltk.classify.accuracy(classifier, test_set))
    # classifier.show_most_informative_features(5)


name_classify()