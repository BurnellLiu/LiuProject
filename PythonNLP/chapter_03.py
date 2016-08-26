
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

正则表达式基本元字符:
·       通配符，匹配所有字符
^abc     匹配以abc 开始的字符串
abc$     匹配以abc 结尾的字符串
[abc]    匹配字符集合中的一个
[A-Z0-9] 匹配字符一个范围
107
ed|ing|s 匹配指定的一个字符串（析取）
*        前面的项目零个或多个，如a*, [a-z]* (也叫Kleene闭包)
+        前面的项目1 个或多个，如a+, [a-z]+
?        前面的项目零个或1 个（即：可选）如：a?, [a-z]?
{n}      重复n 次，n 为非负整数
{n,}     至少重复n 次
{,n}     重复不多于n 次
{m,n}    至少重复m 次不多于n 次
a(b|c)+  括号表示操作符的范围
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
    print [w for w in word_list if re.search('ed$', w)]

    #  "^" 表示单词的开头
    # "." 匹配任何单个字符
    print [w for w in word_list if re.search('^..j..t..$', w)]

    # "?" 表示前面的字符是可选的
    # "^e-?mail$" 表示匹配email和e-mail
    print sum(1 for w in word_list if re.search('^e-?mail$', w))

    # "[]" 中括号表示闭包, 表示匹配其中的一个字符
    # "[a-e]" 表示匹配a到e之间的一个字符
    print [w for w in word_list if re.search('^[ghi][mno][jlk][def]$', w)]

    # 加载网络聊天语料库中的词汇
    chat_words = sorted(set(w for w in nltk.corpus.nps_chat.words()))

    # "+" 表示前面的字符为一个或多个
    print [w for w in chat_words if re.search('^m+i+n+e+$', w)]

    # "*" 表示前面的字符为0个或多个
    print [w for w in chat_words if re.search('^m*i*n*e*$', w)]


def exercise_05():
    # 找出一个单词中的所有元音字符
    word = 'Hello, world! This is a python program!'
    print re.findall('[aeiou]', word)

    # 统计文本中两个或两个以上的元音序列的频率
    words = sorted(set(nltk.corpus.treebank.words()))
    fd = nltk.FreqDist(vs for word in words
                       for vs in re.findall('[aeiou]{2,}', word))
    print fd.items()

    # 使用正则表达式格式话日期
    print [int(n) for n in re.findall('[0-9]{2,}', '2009-12-31')]

exercise_05()

