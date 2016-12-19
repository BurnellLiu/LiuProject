# Python NLTK学习3（语料库） #

本系列博客为学习《用Python进行自然语言处理》一书的学习笔记。

##古腾堡语料库##
古腾堡语料库主要包含一些文学书籍。

先看一个例子，查看古腾堡语料库包含的文本名称：

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

## 网络文本语料库 ##
网络文本语料库中包括火狐交流论坛、加勒比海盗电影剧本、以及葡萄酒评论等等。
    
    from nltk.corpus import webtext
    print(webtext.fileids())

结果为：
> ['firefox.txt', 'grail.txt', 'overheard.txt', 'pirates.txt', 'singles.txt', 'wine.txt']

webtext同样可以看做是一个PlaintextCorpusReader对象。

## 即时消息聊天会话语料库 ##
语料库被分成15个文件，每个文件包含几百个按特定日期和特定年龄的聊天室收集的帖子，例如：10-19-20s_706posts.xml包含2006年10月19日从20多岁聊天室收集的706个帖子。
    
    from nltk.corpus import nps_chat
    print(nps_chat.fileids())

结果为：
> ['10-19-20s_706posts.xml', '10-19-30s_705posts.xml', '10-19-40s_686posts.xml', '10-19-adults_706posts.xml', '10-24-40s_706posts.xml', '10-26-teens_706posts.xml', '11-06-adults_706posts.xml', '11-08-20s_705posts.xml', '11-08-40s_706posts.xml', '11-08-adults_705posts.xml', '11-08-teens_706posts.xml', '11-09-20s_706posts.xml', '11-09-40s_706posts.xml', '11-09-adults_706posts.xml', '11-09-teens_706posts.xml']

nps_chat可以看做是一个NPSChatCorpusReader对象。

**NPSChatCorpusReader::fileids()**：该方法返回语料库中的文本标识列表。

**NPSChatCorpusReader::posts()**：该方法接受一个文本标识作为参数，返回一个包含对话的列表，每一个对话又同时是单词的列表。

例如：

    chat_room = nps_chat.posts('10-19-30s_705posts.xml')
    print(chat_room)

结果为：
> [['U11', 'lol'], ['lol', 'U11'], ['wb', 'U9'], ...]