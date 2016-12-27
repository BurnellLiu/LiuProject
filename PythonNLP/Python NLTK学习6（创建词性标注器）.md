本系列博客为学习《用Python进行自然语言处理》一书的学习笔记。

##默认标注器##

默认标注器为每个单词分配同样的标记，尽管很平庸，但它也是有作用的，先看示例：
    
    import nltk
    raw = "You are a good man, but i don't love you!"
    tokens = nltk.word_tokenize(raw)

    default_tagger = nltk.DefaultTagger('NN')
    tagged_words = default_tagger.tag(tokens)
    print(tagged_words)

结果为：
> [('You', 'NN'), ('are', 'NN'), ('a', 'NN'), ('good', 'NN'), ('man', 'NN'), (',', 'NN'), ('but', 'NN'), ('i', 'NN'), ('do', 'NN'), ("n't", 'NN'), ('love', 'NN'), ('you', 'NN'), ('!', 'NN')]

DefaultTagger的构造函数接受一个标记字符串作为参数，生成一个默认标注器对象，从结果可以看到默认标注器把所有的词都标记为NN。

**DefaultTagger::tag(tokens)**：对指定的单词列表进行标记，返回被标记后的单词列表。

**DefaultTagger::evaluete(tagged_sents)**：使用已经被标记的句子评价标注器，返回正确率0~1.0。

    from nltk.corpus import brown
    tagged_sents = brown.tagged_sents(categories='news')
    print(default_tagger.evaluate(tagged_sents))

结果为：
> 0.13089484257215028

我们可以看到我们自己创建的默认标注器正确率仅为0.13。

##查询标注器##
默认标注器对所有单词使用同一个标记，准确率太低，我们可以考虑指定不同的单词为不同的标记，先看一个示例：

    # 对新闻文本进行频率分布，找出新闻文本最常用的100个单词
    fd = nltk.FreqDist(brown.words(categories='news'))
    most_common_pairs = fd.most_common(100)
    most_common_words = [i[0] for i in most_common_pairs]

    # 对标记后的新闻文本进行条件频率分布，这样我们就可以找到指定单词最多的标记是哪一个
    cfd = nltk.ConditionalFreqDist(brown.tagged_words(categories='news'))

    # 找出最常用的100个单词的最多标记
    likely_tags = dict((word, cfd[word].max()) for word in most_common_words)
    
    # 使用(单词-标记)字典作为模型，生成查询标注器
    baseline_tagger = nltk.UnigramTagger(model=likely_tags)
    tagged_sents = brown.tagged_sents(categories='news')
    print(baseline_tagger.evaluate(tagged_sents))

结果为：
> 0.45578495136941344

这次我们创建的标注器的准确率为0.45，比我们的默认标注器要好很多。UnigramTagger类的构造函数接受一个(单词-标记)字典作为模型，可以直接生成一个标注器。事实上UnigramTagger和DefaultTagger类都继承自TaggerI，TaggerI具有tag和evaluete方法，所以UnigramTagger也具有tag和evaluete方法。

因为我们只指定了100个单词的标记，我们来看看我们创建的标注器对未指定的单词是如何标记的。

    raw = "You are a good man, but i don't love you!"
    tokens = nltk.word_tokenize(raw)
    print(baseline_tagger.tag(tokens))

结果为：
> [('You', None), ('are', 'BER'), ('a', 'AT'), ('good', None), ('man', None), (',', ','), ('but', 'CC'), ('i', None), ('do', None), ("n't", None), ('love', None), ('you', None), ('!', None)]

许多词被分配为None标签，这是因为它们不包含在100单词中。针对这种情况，我们可以给它们一个默认标记。换句话说我们要先使用查找表，如果它不能指定一个标记就使用默认标注器，这个过程叫做回退。
    
    # 使用默认标注器作为回退
    baseline_tagger2 = nltk.UnigramTagger(model=likely_tags, backoff=nltk.DefaultTagger('NN'))
    tagged_sents = brown.tagged_sents(categories='news')
    print(baseline_tagger2.evaluate(tagged_sents))

结果为：
> 0.5817769556656125

我们可以看到正确率提升了

如果我们增大单词数量，则正确率还会提升。
    
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

结果为：
> 0.6789983491457326


##一元标注器##
一元标注器基于一个简单的统计算法：对每个标识符分配这个标识符最有可能的标记。一元标注器的行为就像一个查询标注器，但是它不需要我们提供model，我们只需要提供训练样本也就是被标记的句子列表，标注器会使用这些样本进行训练，将所有词的最可能标记存储在一个字典里，例子如下：

    import nltk
    from nltk.corpus import brown
    tagged_sents = brown.tagged_sents(categories='news')

    # 生成一元标注器
    unigram_tagger = nltk.UnigramTagger(train=tagged_sents)
    print(unigram_tagger.evaluate(tagged_sents))

结果为：
> 0.9349006503968017

这个结果比我们前面的查询标注器要好很多，并且一元标注器不需要我们自己去统计每个单词最有可能的标记。

然而使用同一个数据集作为训练集和测试集不是一个好的做法，如果我们训练出来的标注器过拟合我们也不能知道，接下来我们要分离训练集和测试集，我们把数据集的90%作为训练集，10%作为测试集。
    
    tagged_sents = brown.tagged_sents(categories='news')
    size = int(len(tagged_sents) * 0.9)
    train_sets = tagged_sents[:size]
    test_sets = tagged_sents[size:]
    
    # 生成一元标注器
    unigram_tagger = nltk.UnigramTagger(train=train_sets)
    print(unigram_tagger.evaluate(train_sets))
    print(unigram_tagger.evaluate(test_sets))

结果为：
> 0.9353630649241612
> 
> 0.8115219774743347

我们可以看到一元标注器在测试集上的准确率为0.81

##二元标注器##

尽管我们对每个标识符分配了这个标识符最有可能的标记，但是在不同的上下文环境中，标识符是有可能是其他的标记的。所以一个标识符的标记不仅仅和它自身有关系，还可能和它的前一个标识符或者更前面的标识符有关系。二元标注器就是一个可以考虑标识符自身以及前一个标识符的标注器。

    tagged_sents = brown.tagged_sents(categories='news')
    size = int(len(tagged_sents) * 0.9)
    train_sets = tagged_sents[:size]
    test_sets = tagged_sents[size:]

    # 生成二元标注器
    bigram_tagger = nltk.BigramTagger(train=train_sets)
    print(bigram_tagger.evaluate(train_sets))
    print(bigram_tagger.evaluate(test_sets))

结果为：
> 0.7890434263872471
> 
> 0.10186384929731884
    
    
