本系列博客为学习《用Python进行自然语言处理》一书的学习笔记。

##引言##
分类就是对事物进行归类，举个栗子：男人站左边，女人站右边，人妖站中间。文本分类就是对文本进行归类，举个栗子：垃圾邮件过滤。

那么程序是如何分辨出一个邮件是垃圾邮件还是普通邮件的呢？
这个问题可以类比于别人问你如何分辨一个人是男人还是女人，你可能会说：男人有胡子，女人没胡子，男人皮肤粗糙，女人皮肤细腻等等。有胡子，皮肤粗糙这是一个人的一些特征，我们可以根据一个人的一些特征来分辨他（她）的性别，同理我们也可以根据一封邮件的特征来分辨它是否是垃圾邮件。人的特征好理解，那么一封邮件的特征是什么呢？邮件就是一段文本，一段文本的特征就是里面包含的单词。但是不是所有的单词（特征）都对我们的分类有帮助呢？答案是否定的，就好像一个人的其中一个特征是说东北话，这个特征对我们性别分辨就没有帮助，所以要挑选有用的特征。

解决了特征的问题你可能会问：男人有胡子，女人没胡子这种判断是你妈妈告诉你的，那么程序的判断依据是什么呢？这其实是模型的问题，男人有胡子这种模型可能是你妈妈告诉你的，或者你自己观察总结出来的。针对邮件分类，我们很难直接给出一个模型。但是我们可以对大量的已经分类好的邮件数据集使用机器学习算法来生成这种模型。

以上其实就是一个有监督分类，有监督分类的框架如下图：

![](http://www.burnelltek.com/static/img/1484036666651classify.png)

接下来我们会分别对英文姓名和中文昵称做性别分类。

##性别鉴定（英文）##
根据上面的模型我们可以考虑根据人名（英文）来预测人的性别。

先写一个特征提取函数，这个特征提取函数只把人名的最后一个字母最为特征：
    
    def gender_feature(name):
        return {'last_letter': name[-1]}

获取训练和测试使用的数据集，NLTK中有一个姓名的语料库names，它分别收集了大约5000个女性人名和3000个男性人名：

	import nltk
	import random
	from nltk.corpus import names
    
    # 获取名字，性别数据列表
    male_names = [(name, 'male') for name in names.words('male.txt')]
    female_names = [(name, 'female') for name in names.words('female.txt')]
    total_names = male_names + female_names
    random.shuffle(total_names)

    # 生成性别特征集
    feature_set = [(gender_feature(n), g) for (n, g) in total_names]

    # 将特征集拆分为训练集和测试集
    train_set_size = int(len(feature_set) * 0.6)
    train_set = feature_set[:train_set_size]
    test_set = feature_set[:train_set_size]

使用朴素贝叶斯分类器训练模型，对人名'Neo'进行鉴定并且使用测试集测试模型的准确度：

    classifier = nltk.NaiveBayesClassifier.train(train_set)
    
    print(classifier.classify(gender_feature('Neo')))
    print(nltk.classify.accuracy(classifier, test_set))

结果为：
> male
> 
> 0.7597566093159882

我们可以看到'Neo'被正确的分类为男性，关于朴素贝叶斯分类器的具体实现，我们以后再介绍。我们还可以检查分类器，看看姓名中哪些特征值对区分性别是最有效的：

    classifier.show_most_informative_features()

结果为：

> Most Informative Features
> 
> last_letter = 'a'            female : male   =     30.1 : 1.0
>             
> last_letter = 'k'              male : female =     19.5 : 1.0
> 
> last_letter = 'f'              male : female =     16.7 : 1.0
> 
> last_letter = 'p'              male : female =      9.8 : 1.0
> 
> last_letter = 'o'              male : female =      9.6 : 1.0
> 
> last_letter = 'd'              male : female =      9.3 : 1.0
> 
> last_letter = 'm'              male : female =      8.3 : 1.0
> 
> last_letter = 'g'              male : female =      7.4 : 1.0
> 
> last_letter = 'v'              male : female =      7.3 : 1.0
> 
> last_letter = 'w'              male : female =      6.6 : 1.0

我们可以看到最后字母为a的人名女性的概率是男性的30倍，最后字母为k的人名男性的概率是女性的20倍。

##性别鉴定（中文）##
上面我们针对英文姓名做性别判定，现在我们针对中文昵称做性别判定。NLTK中没有中文的昵称语料库，只能我们自己收集。笔者通过新浪微博，收集到大约5000个男性昵称和5000个女性昵称。点击下面的链接，大家就可以在浏览器中看到昵称数据，右击另存为就可以保存数据。

[女性昵称](http://www.burnelltek.com/static/corpus/names/female.txt)

[男性昵称](http://www.burnelltek.com/static/corpus/names/male.txt)

将下载好的昵称数据保存在工作目录下，然后加载数据。加载数据需要使用到PlaintextCorpusReader类，它的构造函数的第一个参数用于指定数据文件所在目录，第二个参数用于指定数据文件名：

    import nltk
    import random

    from nltk.classify import apply_features
    from nltk.corpus import PlaintextCorpusReader
    
    names_corpus = PlaintextCorpusReader('./', ['female.txt', 'male.txt'])
	all_names = names_corpus.words()

现在我们需要考虑昵称的特征，我们可以为每个字定义一个特性表示该昵称是否包含该字。为了限制特征的数目，我们可以只考虑整个昵称数据中最频繁使用的字，所以我们把昵称中最频繁出现的1000个字作为特征：

    ch_freq = nltk.FreqDist(ch.lower() for name in all_names for ch in name)
	ch_freq_most = ch_freq.most_common(1000)
	ch_features = [ch for (ch, count) in ch_freq_most]

打印看看哪些字是最频繁的：
    
    print(ch_freq_most)

结果为：

> [('a', 1992), ('e', 1836), ('i', 1704), ('n', 1580), ('o', 1371), ('s', 1070), ('l', 1057), ('r', 1055), ('_', 814), ('c', 805), ('t', 793), ('m', 740), ('y', 737), ('-', 700), ('h', 677), ('u', 652), ('小', 626), ('g', 581), ('的', 518), ('d', 493), ('k', 413), ('大', 413), ...]

从结果中我们可以看到一些常见的英文或中文字符。接下来我们写一个特征提取函数：
    
    def name_features(name):
    	"""
    	名称特征提取
    	:param name: 名称
    	:return: 名称特征
    	"""
    	name_chs = set([ch.lower() for ch in name])
    	features = {}
    	for ch in ch_features:
        	features['contain(%s)' % ch] = (ch in name_chs)
    	return features

构建训练数据集和测试数据集，因为数据集稍微有点大，我们使用生成器来构建训练集和测试集：

    female_names = [(name, 'female')for name in names_corpus.words('female.txt')]
    male_names = [(name, 'male')for name in names_corpus.words('male.txt')]

    total_names = female_names + male_names
    random.shuffle(total_names)
    train_set_size = int(len(total_names) * 0.6)
    train_names = total_names[:train_set_size]
    test_names = total_names[train_set_size:]

    train_set = apply_features(name_features, train_names, True)
    test_set = apply_features(name_features, test_names, True)

使用贝叶斯分类器训练模型：

    classifier = nltk.NaiveBayesClassifier.train(train_set)

查看分类器在训练集上和测试集上的正确率：

    print(nltk.classify.accuracy(classifier, train_set))
    print(nltk.classify.accuracy(classifier, test_set))

结果为：
>0.7068422559404524
>
>0.648346930012881

可以看到我们训练的模型在测试集上正确率不太高，可能是训练数据太少或其他原因。我们可以先看看哪些特征字对性别区分有帮助：
    
    classifier.show_most_informative_features(20)

结果为：
> Most Informative Features
> 
> contain(娜) = True  female : male   =     12.8 : 1.0
> 
> contain(姐) = True  female : male   =     12.5 : 1.0
> 
> contain(女) = True  female : male   =     11.9 : 1.0
> 
> contain(浩) = True    male : female =     11.7 : 1.0
> 
> contain(娘) = True  female : male   =     11.3 : 1.0
> 
> contain(伊) = True  female : male   =     11.3 : 1.0
> 
> contain(丽) = True  female : male   =     10.1 : 1.0
> 
> contain(颖) = True  female : male   =      8.1 : 1.0
> 
> contain(妍) = True  female : male   =      8.1 : 1.0
> 
> contain(股) = True    male : female =      7.7 : 1.0
> 
> contain(妈) = True  female : male   =      7.5 : 1.0
> 
> contain(杰) = True    male : female =      7.4 : 1.0
> 
> contain(仙) = True  female : male   =      7.4 : 1.0
> 
> contain(鸡) = True  female : male   =      7.4 : 1.0
> 
> contain(琳) = True  female : male   =      7.2 : 1.0
> 
> contain(刚) = True    male : female =      7.2 : 1.0
> 
> contain(雅) = True  female : male   =      7.2 : 1.0
> 
> contain(男) = True    male : female =      7.0 : 1.0
> 
> contain(酱) = True  female : male   =      6.8 : 1.0
> 
> contain(伟) = True    male : female =      6.7 : 1.0

我们可以看到包含'娜' '姐' '女' '娘'的昵称是女性的概率比较高，包含'浩' '杰' '刚'的昵称是男性的概率比较高，这也符合我们的认知。我们来看看我们把哪些昵称分错了：

    for (name, tag) in test_names:
        guess = classifier.classify(name_features(name))
        if guess != tag:
            print(tag, guess, name)

结果为：
> male female 痞子梦
> 
> male female Misaya若风lol
> 
> female male LISA中文首站
> 
> male female Miyavirolling
> 
> male female 没关系是搞笑啊
> 
> male female Silk\_River\_Research
> 
> female male 阿狮狮
> 
> female male BTV跨界喜剧王
> 
> female male 神迷电影
> 
> male female 顶层分享
> 
> ...

微博很多用户代表的是商家或单位，它们的昵称给我们带来困扰，如BTV跨界喜剧王、顶层分享等，可能我们需要做数据清洗。

##总结##

- **NaiveBayesClassifier::train(labeled_featuresets)**：训练模型，接受被标签的特征集，返回一个朴素贝叶斯分类器对象
- **NaiveBayesClassifier::classify(feature)**：分类，接受特征，返回标签
- **NaiveBayesClassifier::show\_most\_informative\_features(N)**：打印最有用的前N个特征
- **classify.accuracy(classifier, gold)**：计算分类器的正确率