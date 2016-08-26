# -*- coding: utf-8 -*-

import nltk

from nltk.corpus import wordnet


def exercise_wordnet():
    # 打印motorcar的同义词集，该结果为car.n.01,意思为car的第一个名词意思
    print wordnet.synsets('motorcar')

    print wordnet.synset('car.n.01').lemma_names()

    print wordnet.synset('car.n.01').definition()

    print wordnet.synset('car.n.01').examples()


exercise_wordnet()