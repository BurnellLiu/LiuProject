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

为什么nltk.pos_tag()方法可以对单词进行词性标记？这是因为NLTK预先使用一些语料库训练出了一个词性标注器，这个词性标注器可以对单词列表进行标记。

##标注语料库##

NLTK中的很多语料库都已经标注了词性，我们之前学习过的布朗语料库就是一个被标注了词性的语料库，每个语料库使用的标记符号可以有所不同。

    from nltk.corpus import brown
    words_tag = brown.tagged_words(categories='news')
    print(words_tag[:10])

结果为：
> [('The', 'AT'), ('Fulton', 'NP-TL'), ('County', 'NN-TL'), ('Grand', 'JJ-TL'), ('Jury', 'NN-TL'), ('said', 'VBD'), ('Friday', 'NR'), ('an', 'AT'), ('investigation', 'NN'), ('of', 'IN')]

*brown可以看作是一个CategorizedTaggedCorpusReader实例对象。*

**CategorizedTaggedCorpusReader::tagged_words(fileids, categories)**：该方法接受文本标识或者类别标识作为参数，返回这些文本被标注词性后的单词列表。

**CategorizedTaggedCorpusReader::tagged_sents(fileids, categories)**：该方法接受文本标识或者类别标识作为参数，返回这些文本被标注词性后的句子列表，句子为单词列表。
    
    tagged_sents = brown.tagged_sents(categories='news')
    print(tagged_sents)

结果为：
> [[('The', 'AT'), ('Fulton', 'NP-TL'), ('County', 'NN-TL'), ('Grand', 'JJ-TL'), ('Jury', 'NN-TL'), ('said', 'VBD'), ('Friday', 'NR'), ('an', 'AT'), ('investigation', 'NN'), ('of', 'IN'), ("Atlanta's", 'NP$'), ('recent', 'JJ'), ('primary', 'NN'), ('election', 'NN'), ('produced', 'VBD'), ('\`\`', '\`\`'), ('no', 'AT'), ('evidence', 'NN'), ("''", "''"), ('that', 'CS'), ('any', 'DTI'), ('irregularities', 'NNS'), ('took', 'VBD'), ('place', 'NN'), ('.', '.')], [('The', 'AT'), ('jury', 'NN'), ('further', 'RBR'), ('said', 'VBD'), ('in', 'IN'), ('term-end', 'NN'), ('presentments', 'NNS'), ('that', 'CS'), ('the', 'AT'), ('City', 'NN-TL'), ('Executive', 'JJ-TL'), ('Committee', 'NN-TL'), (',', ','), ('which', 'WDT'), ('had', 'HVD'), ('over-all', 'JJ'), ('charge', 'NN'), ('of', 'IN'), ('the', 'AT'), ('election', 'NN'), (',', ','), ('``', '``'), ('deserves', 'VBZ'), ('the', 'AT'), ('praise', 'NN'), ('and', 'CC'), ('thanks', 'NNS'), ('of', 'IN'), ('the', 'AT'), ('City', 'NN-TL'), ('of', 'IN-TL'), ('Atlanta', 'NP-TL'), ("''", "''"), ('for', 'IN'), ('the', 'AT'), ('manner', 'NN'), ('in', 'IN'), ('which', 'WDT'), ('the', 'AT'), ('election', 'NN'), ('was', 'BEDZ'), ('conducted', 'VBN'), ('.', '.')], ...]


NLTK中还包含一个中文语料库sinica_treebank，该库使用繁体中文，该库也被标注了词性，我们来看看该库。

    from nltk.corpus import sinica_treebank
    print(sinica_treebank.fileids())

结果为：
> ['parsed']

*sinica_treebank可以看做是一个SinicaTreebankCorpusReader实例对象。*

**SinicaTreebankCorpusReader::words(fileids)**：该方法接受文本标识作为参数，返回文本的单词列表。

**SinicaTreebankCorpusReader::tagged_words(fileids)**：该方法接受文本标识作为参数，返回文本被标注词性后的单词列表。

    words = sinica_treebank.words('parsed')
    print(words[:40])
    words_tag = sinica_treebank.tagged_words('parsed')
    print(words_tag[:40])

结果为：
> ['一', '友情', '嘉珍', '和', '我', '住在', '同一條', '巷子', '我們', '是', '鄰居', '也', '是', '同班', '同學', '我們', '常常', '一起', '上學', '一起', '回家', '有一天', '上學', '時', '我', '到', '她', '家', '等候', '按', '了', '門鈴', '卻', '沒有', '任何', '動靜', '正當', '我', '想', '離開']
> 
> [('一', 'Neu'), ('友情', 'Nad'), ('嘉珍', 'Nba'), ('和', 'Caa'), ('我', 'Nhaa'), ('住在', 'VC1'), ('同一條', 'DM'), ('巷子', 'Nab'), ('我們', 'Nhaa'), ('是', 'V\_11'), ('鄰居', 'Nab'), ('也', 'Dbb'), ('是', 'V_11'), ('同班', 'Nv3'), ('同學', 'Nab'), ('我們', 'Nhaa'), ('常常', 'Dd'), ('一起', 'Dh'), ('上學', 'VA4'), ('一起', 'Dh'), ('回家', 'VA13'), ('有一天', 'DM'), ('上學', 'VA4'), ('時', 'Ng'), ('我', 'Nhaa'), ('到', 'P61'), ('她', 'Nhaa'), ('家', 'Ncb'), ('等候', 'VK2'), ('按', 'VC2'), ('了', 'Di'), ('門鈴', 'Nab'), ('卻', 'Dbb'), ('沒有', 'VJ3'), ('任何', 'Neqa'), ('動靜', 'Nad'), ('正當', 'P16'), ('我', 'Nhaa'), ('想', 'VE2'), ('離開', 'VC2')]

我们来看看哪些标记是sinica_treebank库中最常见的。

    words_tag = sinica_treebank.tagged_words('parsed')
    tag_fd = nltk.FreqDist(tag for (word, tag) in words_tag)
    tag_fd.tabulate(5)

结果为:

>  Nab   DE  Nac  Nad VH11 

> 9520 7095 4478 3873 3722 

我们可以看到Nab标记出现了9520次。

##总结##

- **nltk.word_tokenize（text）**：对指定的句子进行分词，返回单词列表

- **nltk.pos_tag(words)**：对指定的单词列表进行词性标记，返回标记列表

- **CategorizedTaggedCorpusReader::tagged_words(fileids, categories)**：该方法接受文本标识或者类别标识作为参数，返回这些文本被标注词性后的单词列表

- **CategorizedTaggedCorpusReader::tagged_sents(fileids, categories)**：该方法接受文本标识或者类别标识作为参数，返回这些文本被标注词性后的句子列表，句子为单词列表

- **SinicaTreebankCorpusReader::tagged_words(fileids)**：该方法接受文本标识作为参数，返回文本被标注词性后的单词列表

- **SinicaTreebankCorpusReader::tagged_sents(fileids)**：该方法接受文本标识作为参数，返回文本被标注词性后的句子列表，句子为单词列表
