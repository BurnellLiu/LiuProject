
# -*- coding: utf-8 -*-

from __future__ import division
import nltk
from nltk.book import *

text1.concordance('monstrous')


# 用于寻找与搜索词语境相同的词, 比如monstrous用在the___pictures 和the___size这两个语境中，
# 还有其他哪些单词用在这些语境之中?
# 用在相同语境之中的词，意义上可能会有一些相似。
text1.similar('monstrous')

# 用来检查搜索词1和词2共同出现的语境是什么
text2.common_contexts(['monstrous', 'very'])

# 绘制词语的分布图
text4.dispersion_plot(['citizens', 'democracy', 'freedom', 'duties', 'America'])

# python中默认是截断除法, 如果要使用精确除法需要导入division
# 导入精确除法后，若要执行截断除法，可以使用"//"操作符
len(text3)/len(set(text3))

# 定义词语的频率分布
freq_dist1 = FreqDist(text1)

# 绘制最频繁的前10个词语的计数
freq_dist1.plot(10)

# 获取低频词
freq_dist1.hapaxes()