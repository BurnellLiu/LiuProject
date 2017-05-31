
本系列博客为学习《用Python进行自然语言处理》一书的学习笔记。

##块结构在文件中的表示##
通过[Python NLTK学习8（正则表达式分块器）](http://www.burnelltek.com/blog/24026f94f7e611e6841d00163e0c0e36)一文我们知道块在内存中是以树（Tree）的方式表示的，那么分好块的文本在文件中如何表示呢？标准的方式是**IOB标记**，在这种方式下每个单词都被用3个特殊的块标签之一标记，**I（inside，内部），O（outside，外部），B（begn，开始）**。如果一个单词被标记为B，则表示一个块的开始，块内的单词被标记为I，其他单词被标记为O。B和I标记后需要加块类型的后缀，如B-NP，I-NP。如下图所示：

![](http://www.burnelltek.com/static/img/14883349535271.png)

文本方式的IOB格式存储如下：
> We PRP B-NP
> 
> saw VBD O
> 
> the DT B-NP
> 
> yellow JJ I-NP
> 
> dog NN I-NP

##conll2000语料库##
NLTK中包含一个已经分好块的语料库conll2000，该语料库的部分内容如下：

![](http://www.burnelltek.com/static/img/14883355693401.PNG)

我们看一下如何加载该语料库：

```python
    
    import nltk
    from nltk.corpus import conll2000
    
    # 加载训练文本中的NP块，返回的结果可以当作是一个列表，列表中的元素是Tree对象
    # 每一个Tree对象就是一个被分块的句子
    test_sents = conll2000.chunked_sents("train.txt", chunk_types=["NP"])
    
    # tree2conlltags函数可以将Tree对象转换为IBO标记格式的列表
    tags = nltk.chunk.tree2conlltags(test_sents[0])
    print(tags)

```

以上代码将输出：
> [('Confidence', 'NN', 'B-NP'), ('in', 'IN', 'O'), ('the', 'DT', 'B-NP'), ('pound', 'NN', 'I-NP'), ...]

##使用conll2000语料库评估分块器##
我们先完成一个简单的正则表达式分块器，之后使用语料库来评估它的性能：

```python
    
    # 查找以名词短语标记的特征字母（如CD、DT 和JJ）开头的标记
    grammar = r"NP: {<[CDJNP].*>+}"
    cp = nltk.RegexpParser(grammar)
    
    # 加载训练文本中的NP块
    test_sents = conll2000.chunked_sents("train.txt", chunk_types=["NP"])
    print(cp.evaluate(test_sents))

```

以上代码将输出：
> ChunkParse score:
> 
> IOB Accuracy:  87.4%
>    
> Precision:     69.7%
>    
> Recall:        67.5%
>    
> F-Measure:     68.6%

我们可以看到我们的分块器有87%的准确率。

##使用一元标注器创建分块器##
在[Python NLTK学习6（创建词性标注器）](http://www.burnelltek.com/blog/60740e24d2f711e6841d00163e0c0e36)一文中我们学习了一元标注器，它可以对某个单词标注一个最有可能的词性标注。同样的我们也可以通过训练语料库找到某个词性标注最有可能的块标记。我们需要自己定义一个UnigramChunker类：

```python

    class UnigramChunker(nltk.ChunkParserI):
        """
        一元分块器，
        该分块器可以从训练句子集中找出每个词性标注最有可能的分块标记，
        然后使用这些信息进行分块
        """
        def __init__(self, train_sents):
            """
            构造函数
            :param train_sents: Tree对象列表
            """
            train_data = []
            for sent in train_sents:
                # 将Tree对象转换为IOB标记列表[(word, tag, IOB-tag), ...]
                conlltags = nltk.chunk.tree2conlltags(sent)
    
                # 找出每个词性标注对应的IOB标记
                ti_list = [(t, i) for w, t, i in conlltags]
                train_data.append(ti_list)
    
            # 使用一元标注器进行训练
            self.__tagger = nltk.UnigramTagger(train_data)
    
        def parse(self, tokens):
            """
            对句子进行分块
            :param tokens: 标注词性的单词列表
            :return: Tree对象
            """
            # 取出词性标注
            tags = [tag for (word, tag) in tokens]
            # 对词性标注进行分块标记
            ti_list = self.__tagger.tag(tags)
            # 取出IOB标记
            iob_tags = [iob_tag for (tag, iob_tag) in ti_list]
            # 组合成conll标记
            conlltags = [(word, pos, iob_tag) for ((word, pos), iob_tag) in zip(tokens, iob_tags)]

            return nltk.chunk.conlltags2tree(conlltags)

```

UnigramChunker类的构造函数接受一个训练集，训练集由Tree对象组成，在构造函数中我们会将Tree对象转换为IOB标记列表，之后取出词性标注和IOB标记之间的对应关系并使用一元标注器进行训练。
我们还完成一个parse方法，它可以给新句子进行分块，返回Tree对象。


我们现在测试UnigramChunker类的性能：

```python

    test_sents = conll2000.chunked_sents("test.txt", chunk_types=["NP"])
    train_sents = conll2000.chunked_sents("train.txt", chunk_types=["NP"])

    unigram_chunker = UnigramChunker(train_sents)
    print(unigram_chunker.evaluate(test_sents))

```

以上代码将输出：
> 
> ChunkParse score:
> 
> IOB Accuracy:  92.9%
>     
> Precision:     79.9%
>     
> Recall:        86.8%
>     
> F-Measure:     83.2%

##训练基于分类器的分快器##
无论是基于正则表达式的分块器还是n-gram分块器，决定创建什么块完全基于词性标记。有时词性标记不足以确定一个句子应如何分块，我们可能需要词的内容作为词性标记的补充。
包含词的内容信息的方法之一是创建基于分类器的分块器，我们创建如下的分块器：

```python
    
    class ClassifierChunker(nltk.ChunkParserI):
        """
        分类器分块器
        """
        @classmethod
        def generate_feature(cls, word_tag):
            """
            根据单词和词性标记生成特征
            :param word_tag: 单词-词性标记
            :return: 特征
            """
            word, tag = word_tag
            return {"tag": tag, "word": word}
    
        def __init__(self, train_sents):
            """
            构造函数
            :param train_sents: 训练句子列表的内容为：[[((word, tag), iob-tag), ...], ...]
            """
            train_set = []
            for tagged_sent in train_sents:
                for word_tag, iob_tag in tagged_sent:
                    feature = self.generate_feature(word_tag)
                    train_set.append((feature, iob_tag))
    
            self.__classifier = nltk.NaiveBayesClassifier.train(train_set)
    
        def parse(self, sentence):
            """
            对句子进行分块
            :param sentence: 标注词性的单词列表
            :return: Tree对象
            """
            # 对词性标注进行分块标记
            iob_tags = []
            for word_tag in sentence:
                feature = self.generate_feature(word_tag)
                iob_tag = self.__classifier.classify(feature)
                iob_tags.append(iob_tag)
            # 组合成conll标记
            conlltags = [(word, pos, iob_tag) for ((word, pos), iob_tag) in zip(sentence, iob_tags)]
            return nltk.chunk.conlltags2tree(conlltags)
    
```

分类器分块器根据训练集中的每个单词内容和词性标记生成特征，再使用朴素贝叶斯分类器进行训练。

测试ClassifierChunker的性能：

```python
    
    train_sents = conll2000.chunked_sents("train.txt", chunk_types=["NP"])
    test_sents = conll2000.chunked_sents("test.txt", chunk_types=["NP"])
    tagged_sents = [[((w, t), c) for (w, t, c) in nltk.chunk.tree2conlltags(sent)] for sent in train_sents]
    chunker = ClassifierChunker(tagged_sents)
    print(chunker.evaluate(test_sents))

```

以上代码输出：

> ChunkParse score:
> 
>    IOB Accuracy:  92.4%
>    
>    Precision:     74.9%
>    
>    Recall:        85.0%
>    
>    F-Measure:     79.7%



##总结##

- conll2000，分块语料库
- nltk.chunk.tree2conlltags(t)，将Tree对象转换为IOB标记列表
- nltk.chunk.conlltags2tree(sentence)，将IOB标记列表转换为Tree对象






