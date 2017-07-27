
本系列博客为学习《用Python进行自然语言处理》一书的学习笔记。

##命名实体识别##
命名实体识别（NER）系统的目标是识别所有文字提及的命名实体。可以分解成两个子任务：确定NE的边界和确定其类型。
命名实体识别非常适用于基于分类器类型的方法来处理的任务。
NLTK有一个已经训练好的可以识别命名实体的分类器，可以使用函数nltk.ne\_chunk()进行访问。如果我们设置参数binary=True，那么命名实体只被标注为NE。否则分类器会添加类型标签，如PERSON，ORGANIZATION以及GPE。如下：

```python
    
    # 取出语料库中的一个句子
    sent = nltk.corpus.treebank.tagged_sents()[22]
    
    # 使用NE分块器进行命名实体识别，返回Tree对象，
    # Tree对象的label()方法可以查看命名实体的标签
    for tree in nltk.ne_chunk(sent, binary=True).subtrees():
        # 过滤根树
        if tree.label() == "S":
            continue
    
        print(tree)

```

以上代码我们设置参数binary=True得到如下结果：

> (NE U.S./NNP)
> 
> (NE Brooke/NNP)
> 
> (NE University/NNP)
> 
> (NE Vermont/NNP College/NNP)
> 
> (NE Medicine/NNP)

如果设置参数binary=False则可以得到如下结果：

> (GPE U.S./NNP)
> 
> (PERSON Brooke/NNP T./NNP Mossman/NNP)
> 
> (ORGANIZATION University/NNP)
> 
> (PERSON Vermont/NNP College/NNP)
> 
> (GPE Medicine/NNP)


##关系抽取##
一旦文本中的命名实体已被识别, 我们就可以提取它们之间的关系。我们通常会寻找指定类型的命名实体之间的关系。我们可以找出所有(X, a, Y)形式的三元组。X，Y是指定类型的命名实体，a表示X和Y之间关系的字符串。我们先看如下的例子：

```python

    # 命名实体的关系是 X in Y
    re_in = re.compile(r'.*\bin\b(?!\b.+ing)')

    # ieer 是一个命名实体语料库
    for doc in nltk.corpus.ieer.parsed_docs('NYT_19980315'):
        # 我们抽取组织和位置之间的关系
        for rel in nltk.sem.extract_rels('ORGANIZATION', 'LOCATION', doc, corpus='ieer', pattern=re_in):
            print(rel['subjtext'], 'IN', rel['objtext'])

```

nltk.sem.extract_rels函数接受的参数主要是主实体名(ORGANIZATION)，宾实体名(LOCATION)，以及代表它们之间关系的正则表达式，该函数返回关系的列表，关系是字典结构。关系中的键subjtext可以取出主实体对象，关系中的键objtext可以取出宾实体对象。

以上代码的运行结果为：

> WHYY IN Philadelphia
> 
> McGlashan &AMP; Sarrail IN San Mateo
> 
> Freedom Forum IN Arlington
> 
> Brookings Institution IN Washington
> 
> Idealab IN Los Angeles
> 
> Open Text IN Waterloo
> 
> WGBH IN Boston
> 
> Bastille Opera IN Paris
> 
> Omnicom IN New York
> 
> DDB Needham IN New York
> 
> Kaplan Thaler Group IN New York
> 
> BBDO South IN Atlanta
> 
> Georgia-Pacific IN Atlanta




