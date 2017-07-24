# -*- coding: utf-8 -*-

import nltk
import re

from nltk.corpus import conll2000


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
    for n in tree.subtrees():
        print(n)
    tree.draw()


def regexp_parser_sample5():
    grammar = r"NP: {<[CDJNP].*>+}"
    cp = nltk.RegexpParser(grammar)

    # 加载训练文本中的NP块
    test_sents = conll2000.chunked_sents("train.txt", chunk_types=["NP"])
    print(cp.evaluate(test_sents))


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


def regexp_parser_sample6():
    test_sents = conll2000.chunked_sents("test.txt", chunk_types=["NP"])
    train_sents = conll2000.chunked_sents("train.txt", chunk_types=["NP"])

    unigram_chunker = UnigramChunker(train_sents)
    print(unigram_chunker.evaluate(test_sents))


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


def chunker_sample7():
    """
    分类器分块器示例代码
    :return: 
    """
    train_sents = conll2000.chunked_sents("train.txt", chunk_types=["NP"])
    test_sents = conll2000.chunked_sents("test.txt", chunk_types=["NP"])
    tagged_sents = [[((w, t), c) for (w, t, c) in nltk.chunk.tree2conlltags(sent)] for sent in train_sents]
    chunker = ClassifierChunker(tagged_sents)
    print(chunker.evaluate(test_sents))


def ner_sample():
    """
    命名实体识别示例
    :return: 
    """
    # 取出语料库中的一个句子
    sent = nltk.corpus.treebank.tagged_sents()[22]

    # 使用NE分块器进行命名实体识别，返回Tree对象，
    # Tree对象的label()方法可以查看命名实体的标签
    for tree in nltk.ne_chunk(sent).subtrees():
        # 过滤根树
        if tree.label() == "S":
            continue

        print(tree)


def extract_rels_sample():
    """
    关系抽取示例
    :return: 
    """
    IN = re.compile(r'.*\bin\b(?!\b.+ing)')
    for doc in nltk.corpus.ieer.parsed_docs('NYT_19980315'):
        print(doc)
        #for rel in nltk.sem.extract_rels('ORG', 'LOC', doc, corpus='ieer', pattern=IN):
            #print(rel)


extract_rels_sample()
