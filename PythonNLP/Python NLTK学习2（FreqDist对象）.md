# Python NLTK学习2（FreqDist对象） #

本系列博客为学习《用Python进行自然语言处理》一书的学习笔记

## 频率分布 ##

打开Python解释器，输入如下代码：
    
    import nltk
    from nltk.book import *
    fdist1 = FreqDist(text1)

我们使用Text的实例对象作为参数生成了一个FreqDist对象，FreqDist继承自dict，所以我们可以像操作字典一样操作FreqDist对象
    
    words = list(fdist1.keys())
    print(words[:50])

注意：Python3中字典的keys()方法不再返回列表
