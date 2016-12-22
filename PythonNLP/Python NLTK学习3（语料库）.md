
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

**PlaintextCorpusReader::words(fileids)**：该方法接受一个或多个文本标识作为参数，返回文本单词列表。

    emma = gutenberg.words("austen-emma.txt")
    print(emma)

结果为：
> ['[', 'Emma', 'by', 'Jane', 'Austen', '1816', ']', ...]

**PlaintextCorpusReader::raw(fileids)**：该方法接受一个或多个文本标识为参数，返回文本原始字符串。

    emma_str = gutenberg.raw("austen-emma.txt")
    print(emma_str)

结果为：
> '[Emma by Jane Austen 1816]\n\nVOLUME I\n\nCHAPTER I\n\n\nEmma Woodhouse, ...'

**PlaintextCorpusReader::sents(fileids)**：该方法接受一个或多个文本标识为参数，返回文本中的句子列表。

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

##就职演说语料库##
该语料库是55个文本的集合，每个文本都是一个总统的演说。这个集合的有趣特性是它的时间维度。
    
    from nltk.corpus import inaugural

inaugural同样可以看做是一个PlaintextCorpusReader对象。
    
    print(inaugural.fileids())

结果为：
> ['1789-Washington.txt', '1793-Washington.txt', '1797-Adams.txt', '1801-Jefferson.txt', '1805-Jefferson.txt', '1809-Madison.txt', '1813-Madison.txt', '1817-Monroe.txt', '1821-Monroe.txt', '1825-Adams.txt', '1829-Jackson.txt', ...]

## 即时消息聊天会话语料库 ##
语料库被分成15个文件，每个文件包含几百个按特定日期和特定年龄的聊天室收集的帖子，例如：10-19-20s_706posts.xml包含2006年10月19日从20多岁聊天室收集的706个帖子。
    
    from nltk.corpus import nps_chat
    print(nps_chat.fileids())

结果为：
> ['10-19-20s\_706posts.xml', '10-19-30s\_705posts.xml', '10-19-40s\_686posts.xml', '10-19-adults\_706posts.xml', '10-24-40s\_706posts.xml', '10-26-teens\_706posts.xml', '11-06-adults\_706posts.xml', '11-08-20s\_705posts.xml', '11-08-40s\_706posts.xml', '11-08-adults\_705posts.xml', '11-08-teens_706posts.xml', '11-09-20s\_706posts.xml', '11-09-40s\_706posts.xml', '11-09-adults\_706posts.xml', '11-09-teens\_706posts.xml']

nps_chat可以看做是一个NPSChatCorpusReader对象。

**NPSChatCorpusReader::fileids()**：该方法返回语料库中的文本标识列表。

**NPSChatCorpusReader::posts(fileids)**：该方法接受一个或多个文本标识作为参数，返回一个包含对话的列表，每一个对话又同时是单词的列表。

    chat_room = nps_chat.posts('10-19-30s_705posts.xml')
    print(chat_room)

结果为：
> [['U11', 'lol'], ['lol', 'U11'], ['wb', 'U9'], ...]

## 布朗语料库 ##
布朗语料库是一个百万词级的英语电子语料库，这个语料库包含500个不同来源的文本，按照文体分类，如：新闻、社论等。我们可以先看看布朗语料库中包含哪些类别：

    from nltk.corpus import brown
    print(brown.categories())

结果为：
> ['adventure', 'belles\_lettres', 'editorial', 'fiction', 'government', 'hobbies', 'humor', 'learned', 'lore', 'mystery', 'news', 'religion', 'reviews', 'romance', 'science\_fiction']

brown可以看做是一个CategorizedTaggedCorpusReader对象。

**CategorizedTaggedCorpusReader::categories()**：该方法返回语料库中的类别标识。

**CategorizedTaggedCorpusReader::fileids(categories)**：该方法接受一个或多个类别标识作为参数，返回文本标识列表。
    
    print(brown.fileids(['news', 'lore']))

结果为：
> ['ca01', 'ca02', 'ca03', 'ca04', 'ca05', 'ca06', 'ca07', 'ca08', 'ca09', 'ca10', 'ca11', 'ca12', ...]

**CategorizedTaggedCorpusReader::words(fileids, categories)**：该方法接受文本标识或者类别标识作为参数，返回文本单词列表。

    news = brown.words(categories='news')
    print('news: ', news)
    ca02 = brown.words(fileids='ca02')
    print('ca02: ', ca02)

结果为：
> news:  ['The', 'Fulton', 'County', 'Grand', 'Jury', 'said', ...]

> ca02:  ['Austin', ',', 'Texas', '--', 'Committee', 'approval', ...]

**CategorizedTaggedCorpusReader::sents(fileids, categories)**：该方法接受文本标识或者类别标识作为参数，返回文本句子列表，句子本身是词列表。

##路透社语料库##
路透社语料库包含10,788个新闻文档，共计130万字。文档被分成了90个主题，按照训练和测试分为两组。路特社语料库中的类别是项目重叠的，因为新闻报道往往涉及多个主题。

    from nltk.corpus import reuters

reuters也可以看做是一个CategorizedTaggedCorpusReader对象。

    print(reuters.categories())

结果为：
> ['acq', 'alum', 'barley', 'bop', 'carcass', 'castor-oil', 'cocoa', 'coconut', 'coconut-oil', 'coffee', 'copper', 'copra-cake', 'corn', 'cotton', 'cotton-oil', 'cpi', 'cpu', 'crude', ...]

##总结##
*gutenberg、webtext和inaugural是PlaintextCorpusReader的实例对象。*

**PlaintextCorpusReader**成员方法：

- fileids()，该方法返回语料库中的文本标识列表
- words(fileids)，该方法接受一个或多个文本标识作为参数，返回文本单词列表
- raw(fileids)，该方法接受一个或多个文本标识为参数，返回文本原始字符串
- sents(fileids)，该方法接受一个或多个文本标识为参数，返回文本中的句子列表

****

*nps_chat是NPSChatCorpusReader的实例对象。*

**NPSChatCorpusReader**成员方法：

- fileids()，该方法返回语料库中的文本标识列表
- posts(fileids)，该方法接受一个或多个文本标识作为参数，返回一个包含对话的列表，每一个对话又同时是单词的列表

****

*brown和reuters是CategorizedTaggedCorpusReader的实例对象。*

**CategorizedTaggedCorpusReader**成员方法：

- categories()，该方法返回语料库中的类别标识
- fileids(categories)，该方法接受一个或多个类别标识作为参数，返回文本标识列表
- words(fileids, categories)，该方法接受文本标识或者类别标识作为参数，返回文本单词列表
- sents(fileids, categories)，该方法接受文本标识或者类别标识作为参数，返回文本句子列表，句子本身是词列表