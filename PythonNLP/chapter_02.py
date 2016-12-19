# -*- coding: utf-8 -*-

import nltk

# 加载古腾堡文集, 该语料库主要包含免费电子图书
from nltk.corpus import gutenberg

# 加载网络文本
from nltk.corpus import webtext

# 即时消息聊天会话语料库
from nltk.corpus import nps_chat

# 布朗语料库, 该语料库按照文体分类: 新闻, 社论等
from nltk.corpus import brown

# 路透社语料库, 该语料库包含10788个新闻文档, 这些文档分成90个主题, 按照训练和测试分为两组
from nltk.corpus import reuters

# 就职演说语料库
from nltk.corpus import inaugural

# 世界人权宣言语料库, 包含不同语言版本
from nltk.corpus import udhr

# 英语词典语料库, 包含所有的英文单词
from nltk.corpus import words

# 停用词语料库, 就是那些高频词汇, 如: the, to
from nltk.corpus import stopwords

"""
语料库函数:
fileids()                 语料库中的文件
fileids([categories])     这些分类对应的语料库中的文件
categories()              语料库中的分类
categories([fileids])     这些文件对应的语料库中的分类
raw()                     语料库的原始内容
raw(fileids=[f1,f2,f3])   指定文件的原始内容
raw(categories=[c1,c2])   指定分类的原始内容
words()                   整个语料库中的词汇
words(fileids=[f1,f2,f3]) 指定文件中的词汇
words(categories=[c1,c2]) 指定分类中的词汇
sents()                   指定分类中的句子
sents(fileids=[f1,f2,f3]) 指定文件中的句子
sents(categories=[c1,c2]) 指定分类中的句子
abspath(fileid)           指定文件在磁盘上的位置
encoding(fileid)          文件的编码（如果知道的话）
open(fileid)              打开指定语料库文件的文件流
root()                    到本地安装的语料库根目录的路径

条件频率分布常用方法:
cfdist = ConditionalFreqDist(pairs)  从配对链表中创建条件频率分布
cfdist.conditions()                  将条件按字母排序
cfdist[condition]                    此条件下的频率分布
cfdist[condition][sample]            此条件下给定样本的频率
cfdist.tabulate()                    为条件频率分布制表
cfdist.tabulate(samples, conditions) 指定样本和条件限制下制表
cfdist.plot()                        为条件频率分布绘图
cfdist.plot(samples, conditions)     指定样本和条件限制下绘图
cfdist1 < cfdist2 测试样本在cfdist1  中出现次数是否小于在cfdist2 中出现次数

"""


def exercise_gutenberg():
    # 打印古腾堡项目的文件列表
    print(gutenberg.fileids())

    # 挑选一个文本： 简-奥斯丁的《爱玛》
    emma = gutenberg.words("austen-emma.txt")

    # 查看书的长度
    print(len(emma))

    # 导入文本
    emma_text = nltk.Text(emma)
    emma_text.concordance("surprize")

    for file_id in gutenberg.fileids():
        chars_list = gutenberg.raw(file_id)
        words_list = gutenberg.words(file_id)
        sents_list = gutenberg.sents(file_id)

        # 统计文件的总字符数
        num_chars = len(chars_list)
        # 统计文件的总单词数
        num_words = len(words_list)
        # 统计文件的总句子数
        num_sents = len(sents_list)
        # 统计文件的非重复单词数
        num_vocab = len(set([w.lower() for w in words_list]))
        # 打印词的平均字符数， 句子的平均单词数， 每个单词出现的平均次数， 文件名
        print(num_chars/num_words, num_words/num_sents, num_words/num_vocab, file_id)


def exercise_webtext():
    # 打印网络文本的文件名
    for file_id in webtext.fileids():
        print(file_id)


def exercise_nps_chat():
    # 打印聊天室文本名， 名称由日期，年龄，包含的帖子数量注册
    # 如： 10-19-20s_706posts.xml，包含10月19号从20多岁聊天室收集的706个帖子
    for file_id in nps_chat.fileids():
        print(file_id)


def exercise_brown():
    # 打印布朗语料库中的分类
    print(brown.categories())
    # 打印分类为新闻的文本词汇
    print(brown.words(categories='news'))
    # 打印文本'cg22'
    print(brown.words(fileids=['cg22']))
    # 打印句子
    print(brown.sents(categories=['news', 'reviews']))

    """比较不同文体中的情态动词的用法"""
    # 获取文本
    news_text = brown.words(categories='news')
    # 单词定义频率
    fdist = nltk.FreqDist([w.lower() for w in news_text])
    # 定义情态动词表
    modals = ['can', 'could', 'may', 'might', 'must', 'will']
    for m in modals:
        print(m + ':', fdist[m])


def exercise_brown2():
    """带条件的频率分布函数"""
    cfd = nltk.ConditionalFreqDist((genre, word)
                                   for genre in brown.categories()
                                   for word in brown.words(categories=genre))

    genres = ['news', 'religion', 'hobbies', 'science_fiction', 'romance', 'humor']
    modals = ['can', 'could', 'may', 'might', 'must', 'will']
    cfd.tabulate(conditions=genres, samples=modals)


def exercise_reuters():
    print(reuters.fileids())
    print(reuters.categories())
    # 查看单个文档的主题
    print(reuters.categories('training/9865'))
    # 查看多个文档的主题
    print(reuters.categories(['training/9865', 'training/9880']))
    # 查看每个主题的文档
    print(reuters.fileids('barley'))
    # 查看多个主题的文档
    print(reuters.fileids(['barley', 'corn']))
    # 查看某个文档的词汇
    print(reuters.words('training/9865'))
    # 查看多个文档的词汇
    print(reuters.words(['training/9865', 'training/9880']))
    # 查看某个主题的词汇
    print(reuters.words(categories='barley'))
    # 查看多个主题的词汇
    print(reuters.words(categories=['barley', 'corn']))


def exercise_inaugural():
    print(inaugural.fileids())
    # 提取每个演讲文本的年代名
    print([file_id[:4] for file_id in inaugural.fileids()])

    # 观察词汇america和citizen在不同年份演讲中的出现频率
    cfd = nltk.ConditionalFreqDist((target, file_id[:4])
                                   for file_id in inaugural.fileids()
                                   for w in inaugural.words(file_id)
                                   for target in ['america', 'citizen']
                                   if w.lower().startswith(target))
    cfd.plot()


def exercise_udhr():
    print(udhr.fileids())

    # 查看不同语言的世界人权宣言的字长差异
    languages = ['Chickasaw', 'English', 'German_Deutsch',
                 'Greenlandic_Inuktikut', 'Hungarian_Magyar', 'Ibibio_Efik']
    cfd = nltk.ConditionalFreqDist((lang, len(word))
                                   for lang in languages
                                   for word in udhr.words(lang + '-Latin1'))
    cfd.plot()


def exercise_cfd():
    """
    cfd = nltk.ConditionalFreqDist((genre, word)
                                   for genre in brown.categories()
                                   for word in brown.words(categories=genre))
    """
    genre_word = [(genre, word)
                  for genre in ['news', 'romance']
                  for word in brown.words(categories=genre)]
    print(len(genre_word))
    cfd = nltk.ConditionalFreqDist(genre_word)
    print(cfd.conditions())

    print(cfd['romance'])
    list(cfd['romance'])
    print(cfd['romance']['could'])


def exercise_bigrams():
    sent = ['In', 'the', 'beginning', 'God', 'created', 'the heaven', 'and', 'the earth']
    print(list(nltk.bigrams(sent)))

    text = nltk.corpus.genesis.words('english-kjv.txt')
    bigrams = nltk.bigrams(text)
    cfd = nltk.ConditionalFreqDist(bigrams)

    word = 'living'
    for i in range(15):
        print(word)
        word = cfd[word].max()


def exercise_unusual_words():
    text = gutenberg.words('austen-sense.txt')

    # 取出文本中的词汇, 去除数字, 转换为小写
    text_vocab = set(w.lower() for w in text if w.isalpha())

    # 取出词典中的词汇
    english_vocab = set(w.lower() for w in words.words())

    # 找出文本中的非常用词汇(错误词汇)
    unusual_vocab = text_vocab.difference(english_vocab)

    print(sorted(unusual_vocab))


def exercise_stopwords():
    stop_words = stopwords.words('english')
    print(stop_words)

    text_words = reuters.words()

    # 过滤文本中的停用词
    content = [w for w in text_words if w.lower() not in stop_words]


exercise_stopwords()
