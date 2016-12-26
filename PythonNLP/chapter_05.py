
# -*- coding: utf-8 -*-

import nltk

from nltk.corpus import sinica_treebank
from nltk.corpus import brown


def exercise1():
    # 进行词性标注
    text = nltk.word_tokenize("You are a good man, but i don't like you!")
    print(text)
    print(nltk.pos_tag(text))
    nltk.tag.pos_tag()
    nltk.DefaultTagger

if __name__ == '__main__':
    words_tag = brown.tagged_words(categories='news')
    print(words_tag[:30])
    words_tag = brown.tagged_words(categories='news', tagset='universal')
    print(words_tag[:30])
    words_tag = brown.tagged_words(categories='news', tagset='wsj')
    print(words_tag[:30])
    words_tag = brown.tagged_words(categories='news', tagset='brown')
    print(words_tag[:30])

    words_tag = sinica_treebank.tagged_sents()
    print(words_tag)