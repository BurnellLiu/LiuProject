
# -*- coding: utf-8 -*-

import nltk

from nltk.corpus import sinica_treebank

def exercise1():
    # 进行词性标注
    text = nltk.word_tokenize("You are a good man, but i don't like you!")
    print(text)
    print(nltk.pos_tag(text))



if __name__ == '__main__':
    exercise1()