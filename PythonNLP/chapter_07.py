# -*- coding: utf-8 -*-

import nltk


"""
     CC      连接词
     CD      基数词
     DT      限定词
     EX      存在
     FW      外来词
     IN      介词或从属连词
     JJ      形容词或序数词
     JJR     形容词比较级
     JJS     形容词最高级
     LS      列表标示
     MD      情态助动词
     NN      常用名词 单数形式
     NNS     常用名词 复数形式
     NNP     专有名词 单数形式
     NNPS    专有名词 复数形式
     PDT     前位限定词
     POS     所有格结束词
     PRP     人称代词
     PRP$    所有格代名词
     RB      副词
     RBR     副词比较级
     RBS     副词最高级
     RP      小品词
     SYM     符号
     TO      to 作为介词或不定式格式
     UH      感叹词
     VB      动词基本形式
     VBD     动词过去式
     VBG     动名词和现在分词
     VBN     过去分词
     VBP     动词非第三人称单数
     VBZ     动词第三人称单数
     WDT     限定词
     WP      代词
     WP$     所有格代词
     WRB     疑问代词
"""


def regexp_parser_sample1():
    """
    正则表达式分块器练习1
    """

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


def regexp_parser_sample2():
    """
    正则表达式分块器练习2
    """

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


def regexp_parser_sample3():
    """
    探索文本语料库
    """

    # 规则为匹配动词+to+动词
    cp = nltk.RegexpParser("CHUNK: {<V.*><TO><V.*>}")

    brown = nltk.corpus.brown
    for sent in brown.tagged_sents():
        tree = cp.parse(sent)
        for subtree in tree.subtrees():
            if subtree.label() == "CHUNK":
                print(subtree)


def regexp_parser_sample4():
    """
    加缝隙
    """

    # 分词
    text = "the little yellow dog barked at the cat"
    sentence = nltk.word_tokenize(text)

    # 词性标注
    sentence_tag = nltk.pos_tag(sentence)
    print(sentence_tag)

    # 定义缝隙语法
    # 第一个规则匹配整个句子
    # 第二个规则匹配一个或多个动词或介词
    # 一对}{就代表把其中语法匹配到的词作为缝隙
    grammar = r"""
    NP: {<.*>+}
        }<VBD|IN>+{
    """
    cp = nltk.RegexpParser(grammar)

    # 分块
    tree = cp.parse(sentence_tag)
    tree.draw()


regexp_parser_sample4()
