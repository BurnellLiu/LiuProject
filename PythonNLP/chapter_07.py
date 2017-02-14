# -*- coding: utf-8 -*-

import nltk


def regexp_parser_sample1():
    """
    正则表达式分块器练习1
    """
    text = "the little yellow dog barked at the cat"
    sentences = nltk.sent_tokenize(text)
    print(sentences)
    sentences = [nltk.word_tokenize(sent) for sent in sentences]
    print(sentences)
    sentences_tag = [nltk.pos_tag(sent) for sent in sentences]
    print(sentences_tag)

    # NP 名词短语块
    # DT 限定词
    # JJ 形容词
    # NN 名词
    # 这个规则是说一个NP块由一个可选的限定词后面跟着任何数目的形容词然后是一个名词组成
    grammar = "NP: {<DT>?<JJ>*<NN>}"
    cp = nltk.RegexpParser(grammar)
    for sent_tag in sentences_tag:
        result = cp.parse(sent_tag)
        print(result)
        result.draw()


def regexp_parser_sample2():
    """
    正则表达式分块器练习2
    """

    # NNP 专有名词
    # PP$ 格代名词
    # 第一条规则匹配一个可选的限定词或所有格代名词，零个或多个形容词，然后跟一个名词
    # 第二条规则匹配一个或多个专有名词
    grammar = r"""
        NP: {<DT|PP\$>?<JJ>*<NN>}
            {<NNP>+}
    """
    # $符号是正则表达式中的一个特殊字符，必须使用转义符号\来匹配PP$

    sentences = nltk.sent_tokenize("Lucy let down her long golden hair")
    print(sentences)
    sentences = [nltk.word_tokenize(sent) for sent in sentences]
    print(sentences)
    sentences_tag = [nltk.pos_tag(sent) for sent in sentences]
    print(sentences_tag)

    cp = nltk.RegexpParser(grammar)

    sentence = [("Lucy", "NNP"), ("let", "VBD"), ("down", "RP"), ("her", "PP$"),
                ("long", "JJ"), ("golden", "JJ"), ("hair", "NN")]
    result = cp.parse(sentence)
    result.draw()


regexp_parser_sample2()
