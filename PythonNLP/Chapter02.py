# -*- coding: utf-8 -*-

import nltk

# 加载古腾堡文集
from nltk.corpus import gutenberg

# 加载网络文本
from nltk.corpus import webtext

# 即时消息聊天会话语料库
from nltk.corpus import nps_chat

# 布朗语料库
from nltk.corpus import brown


def exercise1():
    # 打印古腾堡项目的文件列表
    print gutenberg.fileids()

    # 挑选一个文本： 简-奥斯丁的《爱玛》
    emma = gutenberg.words("austen-emma.txt")

    # 查看书的长度
    print len(emma)

    # 导入文本
    emma_text = nltk.Text(emma)
    emma_text.concordance("surprize")


def exercise2():
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
        print num_chars/num_words, num_words/num_sents, num_words/num_vocab, file_id


def exercise3():
    # 打印网络文本的文件名
    for file_id in webtext.fileids():
        print file_id


def exercise4():
    # 打印聊天室文本名， 名称由日期，年龄，包含的帖子数量注册
    # 如： 10-19-20s_706posts.xml，包含10月19号从20多岁聊天室收集的706个帖子
    for file_id in nps_chat.fileids():
        print file_id


def exercise5():
    # 打印布朗语料库中的分类
    print brown.categories()

exercise5()
