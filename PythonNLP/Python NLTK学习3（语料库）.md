# Python NLTK学习3（语料库） #

本系列博客为学习《用Python进行自然语言处理》一书的学习笔记。

##古腾堡语料库##

先看一个列子，查看古腾堡语料库包含的文件名称：

    import nltk
    from nltk.corpus import gutenberg
    print(gutenberg.fileids())

输出如下：

> ['austen-emma.txt', 'austen-persuasion.txt', 'austen-sense.txt', 'bible-kjv.txt', 'blake-poems.txt', 'bryant-stories.txt', 'burgess-busterbrown.txt', 'carroll-alice.txt', 'chesterton-ball.txt', 'chesterton-brown.txt', 'chesterton-thursday.txt', 'edgeworth-parents.txt', 'melville-moby_dick.txt', 'milton-paradise.txt', 'shakespeare-caesar.txt', 'shakespeare-hamlet.txt', 'shakespeare-macbeth.txt', 'whitman-leaves.txt'] 

gutenberg是NLTK预先帮我们加载的语料库，我们可以把gutenberg看做是一个PlaintextCorpusReader对象。

**PlaintextCorpusReader::fileids()**：该方法返回语料库中的文本标识列表。

**PlaintextCorpusReader::words()**：该方法接受一个文本标识作为参数，返回文本单词列表。

例如：

    emma = gutenberg.words("austen-emma.txt")
    print(emma)

结果为：
> ['[', 'Emma', 'by', 'Jane', 'Austen', '1816', ']', ...]

**PlaintextCorpusReader::raw()**：该方法接受一个文本标识为参数，返回文本原始字符串。
例如：

    emma_str = gutenberg.raw("austen-emma.txt")
    print(emma_str)

结果为：
> '[Emma by Jane Austen 1816]\n\nVOLUME I\n\nCHAPTER I\n\n\nEmma Woodhouse, ...'

**PlaintextCorpusReader::sents()**：该方法接受一个文本标识为参数，返回文本中的句子列表。
例如：

    emma_sents = gutenberg.sents("austen-emma.txt")
    print(emma_sents)
结果为：
> [['[', 'Emma', 'by', 'Jane', 'Austen', '1816', ']'], ['VOLUME', 'I'], ...]