本系列博客为学习《用Python进行自然语言处理》一书的学习笔记。

##前文复习##
前面的博客中我们学习了一些语料库，我们知道布朗语料库是一个按照文体分类的语料库。我们还学习频率分布对象FreqDist。我们指定单词列表变量mylist，FreqDist(mylist)会计算列表中每个项目出现的次数。本章我们将学习条件频率分布对象（ConditionalFreqDist），条件频率分布是频率分布的集合，每个频率分布有一个不同的条件，这个条件通常是文本的类别。

##ConditionalFreqDist##
先看一个示例：

    import nltk
    from nltk.corpus import brown
    pairs = [(genre, word) for genre in brown.categories() for word in brown.words(categories=genre)]
    cfd = nltk.ConditionalFreqDist(pairs)

条件频率分布需要处理的是配对列表，每对的形式是（条件，事件），在示例中条件为文体类别，事件为单词。

**ConditionalFreqDist::conditions()**：返回条件列表。
    
    print(cfd.conditions())

结果为：
> ['religion', 'mystery', 'editorial', 'science\_fiction', 'learned', 'hobbies', 'government', 'adventure', 'reviews', 'belles\_lettres', 'fiction', 'lore', 'news', 'romance', 'humor']

**ConditionalFreqDist::tabulate(conditions, samples)**：根据指定的条件和样本，打印条件频率分布表格。
    
    genres = ['news', 'religion', 'hobbies', 'science_fiction', 'romance', 'humor']
    modals = ['can', 'could', 'may', 'might', 'must', 'will']
    cfd.tabulate(conditions=genres, samples=modals)

结果为：

>                       can could   may might  must  will 

>                news    93    86    66    38    50   389 
         
>            religion    82    59    78    12    54    71 
      
>             hobbies   268    58   131    22    83   264 
       
>     science_fiction    16    49     4    12     8    16 

>             romance    74   193    11    51    45    43 
      
>               humor    16    30     8     8     9    13 

我们可以看到不同情态动词在不同类别下的出现次数。

**ConditionalFreqDist::plot(conditions, samples)**：根据给定的条件和样本，绘制条件频率分布图。

    cfd.plot(conditions=genres, samples=modals)

结果为：

![](http://www.burnelltek.com/static/img/1482386967743NLTK.PNG)

##双连词##

**nltk.bigrams(words)**：根据给定的单词列表，生成所有的双连词组。

    sent = ['I', 'am', 'a', 'good', 'man']
    print(list(nltk.bigrams(sent)))

结果为：
> [('I', 'am'), ('am', 'a'), ('a', 'good'), ('good', 'man')]

如果我们对一个文本使用bigrams方法，那么我们便得到该文本的所有双连词，如果我们对所有的双连词进行条件频率分布处理，那么我们就可以知道单词的后续词的频率分布。

    text = brown.words(categories='news')
    bigrams_words = nltk.bigrams(text)
    cfd = nltk.ConditionalFreqDist(bigrams_words)
    fd = cfd['can']
    fd.plot(10)

ConditionalFreqDist继承自dict，ConditionalFreqDist对象的每个键对应的值为FreqDist对象，代码中我们使用了FreqDist的plot方法。

结果为：

![](http://www.burnelltek.com/static/img/1482390303621NLTK.PNG)

我们可以看到can单词最常见的后续词是be。

##总结##

**nltk.bigrams(words)**：根据给定的单词列表，生成所有的双连词组

**ConditionalFreqDist类**

- 继承自dict
- 构造函数接受一个配对列表，每对的形式是（条件，事件）
- 键为条件，值为FreqDist对象

**ConditionalFreqDist成员方法**：

- conditions()，返回条件列表
- tabulate(conditions, samples)，根据指定的条件和样本，打印条件频率分布表格
- plot(conditions, samples)，根据给定的条件和样本，绘制条件频率分布图
