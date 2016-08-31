
# -*- coding: utf-8 -*-

import nltk


def exercise1():
    # 进行词性标注
    text = nltk.word_tokenize("And now for something completely different")
    print nltk.pos_tag(text)


if __name__ == '__main__':
    exercise1()