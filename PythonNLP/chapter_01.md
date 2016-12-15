# 用Python进行自然语言处理学习1.1 #

本系列博客为学习《用Python进行自然语言处理》一书的学习笔记
## 搭建环境 ##
- Python版本为3.5
- IDE：PyCharm
- PyCharm中安装NLTK和matplotlib

Python版本可以随意选择，只要NLTK支持就可以。IDE的话笔者习惯使用Pycharm在Windows下进行Python开发，而且PyCharm中安装Python库也很方便，随自己喜好。

![](http://www.burnelltek.com/static/img/1481709413821NLTK.PNG)

打开Python3.5解释器，输入如下代码：

	import nltk
    nltk.download()

如果出现错误，则说明你的NLTK没有安装成功，没问题的话我们就可以看到如下对话框：

![](http://www.burnelltek.com/static/img/1481794126357NLTK.PNG)

设置好下载目录，选择book选项，点击下载。《用Python进行自然语言处理》这本书中所用到的所有书籍数据都在book选项下，下载时间可能有点长，需要耐心等待。下载完成后我们的基本环境就搭建完成啦。

输入下面代码来看看书籍数据：
   
	import nltk
    form nltk.book import *

![](http://www.burnelltek.com/static/img/1481795185592NLTK.PNG)

我们可以看到nltk预先帮我们加载了一些书籍数据text1~text9，text1~text9为Text类的实例对象名称。

