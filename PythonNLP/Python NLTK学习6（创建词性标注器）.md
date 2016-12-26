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
    
    # 使用默认标注汽作为回退
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

