# Python NLTK学习2（FreqDist对象） #

本系列博客为学习《用Python进行自然语言处理》一书的学习笔记

## 频率分布 ##

打开Python解释器，输入如下代码：
    
    import nltk
    from nltk.book import *
    fdist1 = FreqDist(text1)

我们使用Text的实例对象作为参数生成了一个FreqDist对象，FreqDist继承自dict，所以我们可以像操作字典一样操作FreqDist对象，FreqDist中的键为单词，值为单词的出现总次数

    for key in fdist1:
		print(key, fdist1[key])

这样我们就能就可以看到每个单词对应的出现次数

> ...
> 
> introduced 6

> illustrates 1

> pepper 2

> independence 1

> desirous 1

> ...

**FreqDist::plot()**：该方法接受一个数字N，会显示出现次数最多的前N项单词
例如：

    fdist1.plot(10)

![](http://www.burnelltek.com/static/img/1482070036143NLTK.PNG)

我们可以看到前10项的单词出现的次数

注意：Python3中字典的keys()方法不再返回列表

