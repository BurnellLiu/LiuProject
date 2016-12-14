
# -*- coding: utf-8 -*-

from __future__ import division
import nltk
from nltk.book import *


def exercise1():
    text1.concordance('monstrous')

    # 用于寻找与搜索词语境相同的词, 比如monstrous用在the___pictures 和the___size这两个语境中，
    # 还有其他哪些单词用在这些语境之中?
    # 用在相同语境之中的词，意义上可能会有一些相似。
    text1.similar('monstrous')

    # 用来检查搜索词1和词2共同出现的语境是什么
    text2.common_contexts(['monstrous', 'very'])

    # 绘制词语的分布图
    text4.dispersion_plot(['citizens', 'democracy', 'freedom', 'duties', 'America'])

    # 找到频繁的双连词
    text1.collocations()


def exercise2():
    # python中默认是截断除法, 如果要使用精确除法需要导入division
    # 导入精确除法后，若要执行截断除法，可以使用"//"操作符
    len(text3)/len(set(text3))


def exercise3():
    # 定义词语的频率分布
    freq_dist1 = FreqDist(text1)

    # 绘制最频繁的前10个词语的计数
    freq_dist1.plot(10)

    # 获取低频词
    freq_dist1.hapaxes()


def exercise4():
    # 获取词汇表中长度大于7并且出现次数超过7次的单词
    # 这样的次可能是文本的特征词汇
    freq_dist = FreqDist(text5)
    words_set = set(text5)
    long_words = [w for w in words_set if len(w) > 7 and freq_dist[w] > 7]
    print(sorted(long_words))


def exercise5():
    # 统计文本中词长的分布
    len_list = [len(w) for w in text1]
    freq_dist = FreqDist(len_list)
    print(freq_dist.keys())
    print(freq_dist.items())
    max_len = freq_dist.max()
    print(max_len)
    print(freq_dist[max_len])
    print(freq_dist.freq(max_len))


exercise1()
