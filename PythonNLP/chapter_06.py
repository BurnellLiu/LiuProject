
# -*- coding: utf-8 -*-

import nltk
import random
from nltk.corpus import names


def gender_feature1(name):
    return {'last_letter': name[-1]}


def exercise1():
    # 获取名字，性别数据列表
    male_names = [(name, 'male') for name in names.words('male.txt')]
    female_names = [(name, 'female') for name in names.words('female.txt')]
    total_names = male_names + female_names
    random.shuffle(total_names)

    # 生成性别特征集
    feature_set = [(gender_feature1(n), g) for (n, g) in total_names]

    # 将特征集拆分为训练集和测试集
    train_set_size = int(len(feature_set) * 0.6)
    train_set = feature_set[:train_set_size]
    test_set = feature_set[:train_set_size]

    classifier = nltk.NaiveBayesClassifier.train(train_set)
    # print(classifier.prob_classify(gender_feature1('Burnell')).prob('male'))
    print(nltk.classify.accuracy(classifier, test_set))


def gender_feature2(name):
    feature = {}
    for letter in 'abcdefghijklmnopqrstuvwxyz':
        feature[letter] = letter in name.lower()

    return feature


def exercise2():
    male_names = [(name, 'male') for name in names.words('male.txt')]
    female_names = [(name, 'female') for name in names.words('female.txt')]
    total_names = male_names + female_names
    random.shuffle(total_names)

    # 生成性别特征集
    feature_set = [(gender_feature2(n), g) for (n, g) in total_names]

    # 将特征集拆分为训练集和测试集
    train_set_size = int(len(feature_set) * 0.6)
    train_set = feature_set[:train_set_size]
    test_set = feature_set[:train_set_size]

    classifier = nltk.NaiveBayesClassifier.train(train_set)
    # print(classifier.prob_classify(gender_feature2('Burnell')).prob('male'))
    print(nltk.classify.accuracy(classifier, test_set))


exercise1()

exercise2()