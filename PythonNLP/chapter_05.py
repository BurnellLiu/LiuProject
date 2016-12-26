
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

    raw = "You are a good man, but i don't love you!"
    tokens = nltk.word_tokenize(raw)
    default_tagger = nltk.DefaultTagger('NN')
    lagged_words = default_tagger.tag(tokens)
    print(lagged_words)

    tagged_sents = brown.tagged_sents(categories='news')
    print(default_tagger.evaluate(tagged_sents))

if __name__ == '__main__':

    # 对新闻文本进行频率分布，找出新闻文本最常用的500个单词
    fd = nltk.FreqDist(brown.words(categories='news'))
    most_common_pairs = fd.most_common(500)
    most_common_words = [i[0] for i in most_common_pairs]

    # 对标记后的新闻文本进行条件频率分布，这样我们就可以找到指定单词最多的标记是哪一个
    cfd = nltk.ConditionalFreqDist(brown.tagged_words(categories='news'))

    # 找出最常用的500个单词的最多标记
    likely_tags = dict((word, cfd[word].max()) for word in most_common_words)

    # 使用(单词-标记)字典作为模型，生成查询标注器
    baseline_tagger = nltk.UnigramTagger(model=likely_tags, backoff=nltk.DefaultTagger('NN'))
    tagged_sents = brown.tagged_sents(categories='news')
    print(baseline_tagger.evaluate(tagged_sents))

    nltk.BigramTagger


