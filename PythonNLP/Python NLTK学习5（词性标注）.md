本系列博客为学习《用Python进行自然语言处理》一书的学习笔记。

##词性标注器##

一个词性标注器处理一个词序列，为每个词附加一个词性标记，我们先看一个示例：

    import nltk
    words = nltk.word_tokenize('And now for something completely different')
    print(words)
    word_tag = nltk.pos_tag(words)
    print(word_tag)

结果为：
> ['And', 'now', 'for', 'something', 'completely', 'different']
> 
> [('And', 'CC'), ('now', 'RB'), ('for', 'IN'), ('something', 'NN'), ('completely', 'RB'), ('different', 'JJ')]

**nltk.word_tokenize（text）**：对指定的句子进行分词，返回单词列表。

**nltk.pos_tag(words)**：对指定的单词列表进行词性标记，返回标记列表。

从结果我们可以看到something是NN，NN代表名词。