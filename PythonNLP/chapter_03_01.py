
# -*- coding: utf-8 -*-

from urllib import urlopen
from bs4 import BeautifulSoup
import nltk
import re

"""
字符串常用方法
s.find(t)       字符串s 中包含t 的第一个索引（没找到返回-1）
s.rfind(t)      字符串s 中包含t 的最后一个索引（没找到返回-1）
s.index(t)      与s.find(t)功能类似，但没找到时引起ValueError
s.rindex(t)     与s.rfind(t)功能类似，但没找到时引起ValueError
s.join(text)    连接字符串s 与text 中的词汇
s.split(t)      在所有找到t 的位置将s 分割成链表（默认为空白符）
s.splitlines()  将s 按行分割成字符串链表
s.lower()       将字符串s 小写
s.upper()       将字符串s 大写
s.titlecase()   将字符串s 首字母大写
s.strip()       返回一个没有首尾空白字符的s 的拷贝
s.replace(t, u) 用u 替换s 中的t
"""


def collect_text_from_network():
    # 获取文本内容
    url = "http://www.gutenberg.org/files/2554/2554.txt"
    text = urlopen(url).read()

    # 进行分词
    tokens = nltk.word_tokenize(text)
    print tokens[:50]


def collect_text_from_html():
    # 获取HTML格式的文本
    url = "http://news.bbc.co.uk/2/hi/health/2284783.stm"
    html_text = urlopen(url).read()

    # 清除HTML标记
    bs = BeautifulSoup(html_text)
    text = bs.get_text()

    # 进行分词
    tokens = nltk.word_tokenize(text)
    print tokens[:50]


def exercise_re():
    """正则表达式练习
    """
    # 加载英文小写单词
    word_list = [w for w in nltk.corpus.words.words('en') if w.islower()]

    # 使用正则表达式搜索以ed结尾的单词
    # "$" 表示为单词的末尾
    word_list_ed = [w for w in word_list if re.search('ed$', w)]
    print word_list_ed

    #  "^" 表示单词的开头
    # "." 匹配任何单个字符
    word_list_2 = [ w for w in word_list if re.search('^..j..t..$', w)]
    print word_list_2

    # "?" 表示前面的字符是可选的
    # "^e-?mail$" 表示匹配email和e-mail
    print sum(1 for w in word_list if re.search('^e-?mail$', w))


exercise_re()