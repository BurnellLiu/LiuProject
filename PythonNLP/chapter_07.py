# -*- coding: utf-8 -*-

import nltk


"""
1.      CC      Coordinating conjunction 连接词
2.      CD      Cardinal number  基数词
3.      DT      Determiner  限定词（如this,that,these,those,such，不定限定词：no,some,any,each,every,enough,either,neither,all,both,half,several,many,much,(a) few,(a) little,other,another.
4.      EX      Existential there 存在句
5.      FW      Foreign word 外来词
6.      IN      Preposition or subordinating conjunction 介词或从属连词
7.      JJ      Adjective 形容词或序数词
8.      JJR     Adjective, comparative 形容词比较级
9.      JJS     Adjective, superlative 形容词最高级
10.     LS      List item marker 列表标示
11.     MD      Modal 情态助动词
12.     NN      Noun, singular or mass 常用名词 单数形式
13.     NNS     Noun, plural  常用名词 复数形式
14.     NNP     Proper noun, singular  专有名词，单数形式
15.     NNPS    Proper noun, plural  专有名词，复数形式
16.     PDT     Predeterminer 前位限定词
17.     POS     Possessive ending 所有格结束词
18.     PRP     Personal pronoun 人称代词
19.     PRP$    Possessive pronoun 所有格代名词
20.     RB      Adverb 副词
21.     RBR     Adverb, comparative 副词比较级
22.     RBS     Adverb, superlative 副词最高级
23.     RP      Particle 小品词
24.     SYM     Symbol 符号
25.     TO      to 作为介词或不定式格式
26.     UH      Interjection 感叹词
27.     VB      Verb, base form 动词基本形式
28.     VBD     Verb, past tense 动词过去式
29.     VBG     Verb, gerund or present participle 动名词和现在分词
30.     VBN     Verb, past participle 过去分词
31.     VBP     Verb, non-3rd person singular present 动词非第三人称单数
32.     VBZ     Verb, 3rd person singular present 动词第三人称单数
33.     WDT     Wh-determiner 限定词（如关系限定词：whose,which.疑问限定词：what,which,whose.）
34.     WP      Wh-pronoun 代词（who whose which）
35.     WP$     Possessive wh-pronoun 所有格代词
36.     WRB     Wh-adverb   疑问代词（how where when）
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

    text = "the little yellow dog barked at the cat"
    sentence = nltk.word_tokenize(text)
    sentence_tag = nltk.pos_tag(sentence)
    print(sentence_tag)

    grammar = r"""
    NP: {<.*>+}
        }<VBD|IN>+{
    """
    cp = nltk.RegexpParser(grammar)
    tree = cp.parse(sentence_tag)
    tree.draw()


regexp_parser_sample2()
