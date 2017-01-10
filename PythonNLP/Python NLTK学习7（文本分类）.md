本系列博客为学习《用Python进行自然语言处理》一书的学习笔记。

##引言##
分类就是对事物进行归类，举个栗子：男人站左边，女人站右边，人妖站中间。文本分类就是对文本进行归类，举个栗子：垃圾邮件过滤。

那么程序是如何分辨出一个邮件是垃圾邮件还是普通邮件的呢？
这个问题可以类比于别人问你如何分辨一个人是男人还是女人，你可能会说：男人有胡子，女人没胡子，男人皮肤粗糙，女人皮肤细腻等等。有胡子，皮肤粗糙这是一个人的一些特征，我们可以根据一个人的一些特征来分辨他（她）的性别，同理我们也可以根据一封邮件的特征来分辨它是否是垃圾邮件。人的特征好理解，那么一封邮件的特征是什么呢？邮件就是一段文本，一段文本的特征就是里面包含的单词。但是不是所有的单词（特征）都对我们的分类有帮助呢？答案是否定的，就好像一个人的其中一个特征是说东北话，这个特征对我们性别分辨就没有帮助，所以要挑选有用的特征。

解决了特征的问题你可能会问：男人有胡子，女人没胡子这种判断是你妈妈告诉你的，那么程序的判断依据是什么呢？这其实是模型的问题，男人有胡子这种模型可能是你妈妈告诉你的，或者你自己观察总结出来的。针对邮件分类，我们很难直接给出一个模型。但是我们可以对大量的已经分类好的邮件数据集使用机器学习算法来生成这种模型。

以上其实就是一个有监督分类，有监督分类的框架如下图：

![](http://www.burnelltek.com/static/img/1484036666651classify.png)

##性别鉴定##
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

我们可以看到'Neo'被正确的分类为男性，关于朴素贝叶斯分类器的具体实现，我们以后再介绍。我们还可以检查分类器，看看哪些特征值对区分名字的性别是最有效的。

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