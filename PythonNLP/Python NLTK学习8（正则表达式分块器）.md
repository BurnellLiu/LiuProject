本系列博客为学习《用Python进行自然语言处理》一书的学习笔记。

##引言##
一个句子在分成一个个词后，我们还可以将一个或多个连续的词分成一块。分成的块通常代表一个整体，例如在句子"We saw the yellow dog"中，"the yellow dog"可以看作是一个块，像这样的块我们一般称为名词短语块。分块是用于实体识别的基本技术。

##名词短语分块##
名词短语分块也叫做NP-分块，NP-分块信息最有用的来源之一是词性标记，所以我们一般在分块前都会进行词性标注。

##正则表达式分块器##
为了创建一个NP-块，我们需要定义NP-块的块语法。正则表达式分块器接受正则表达式规则定义的语法来对文本进行分块，我们先看一个例子：
    
    import nltk
    
    # 分词
    text = "the little yellow dog barked at the cat"
    sentence = nltk.word_tokenize(text)

    # 词性标注
    sentence_tag = nltk.pos_tag(sentence)
    print(sentence_tag)

    # 定义分块语法
    # 这个规则是说一个NP块由一个可选的限定词后面跟着任何数目的形容词然后是一个名词组成
    # NP(名词短语块) DT(限定词) JJ(形容词) NN(名词)
    grammar = "NP: {<DT>?<JJ>*<NN>}"

    # 进行分块
    cp = nltk.RegexpParser(grammar)
    tree = cp.parse(sentence_tag)
    tree.draw()

示例中我们先进行分词，词性标注，之后我们定义了一个分块语法"NP: {<DT\>?<JJ\>*<NN\>}"，该语法中的NP就是代表我们的块名称，一对{}就是一个分块规则，使用该语法构造出一个正则表达式分块器就可以对标注词性的文本进行分块，分块结果是一个Tree对象。

该示例的结果为：

> [('the', 'DT'), ('little', 'JJ'), ('yellow', 'JJ'), ('dog', 'NN'), ('barked', 'VBD'), ('at', 'IN'), ('the', 'DT'), ('cat', 'NN')]
> ![](http://www.burnelltek.com/static/img/1487148599085NP.PNG)

从分块结果中我们可以看到"the little yellow dog"和"the cat"都被正确的分块为NP-块。

##多条规则的正则表达式分块器##
上面的例子只有一条语法规则，如果面对复杂的情况就不太适用，我们可以定义多条分块规则。面对多条规则，分块器会轮流应用分块规则，依次更新块结构，所有的规则都被调用后才返回。我们先看一个示例：
    
    import nltk

    # 分词
    text = "Lucy let down her long golden hair"
    sentence = nltk.word_tokenize(text)

    # 词性标注
    sentence_tag = nltk.pos_tag(sentence)
    print(sentence_tag)

    # 定义分块语法
    # NNP(专有名词) PRP$(格代名词)
    # 第一条规则匹配可选的词（限定词或格代名词），零个或多个形容词，然后跟一个名词
    # 第二条规则匹配一个或多个专有名词
    # $符号是正则表达式中的一个特殊字符，必须使用转义符号\来匹配PP$
    grammar = r"""
        NP: {<DT|PRP\$>?<JJ>*<NN>}
            {<NNP>+}
    """

    # 进行分块
    cp = nltk.RegexpParser(grammar)
    tree = cp.parse(sentence_tag)
    tree.draw()

该示例的结果为：

> [('Lucy', 'NNP'), ('let', 'VBD'), ('down', 'RP'), ('her', 'PRP$'), ('long', 'JJ'), ('golden', 'JJ'), ('hair', 'NN')]
> ![](http://www.burnelltek.com/static/img/1487150169688NP.PNG)