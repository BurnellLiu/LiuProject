# -*- coding: utf-8 -*-

import ConfigParser

from WeiboPage import *
from UrlRecord import UrlRecord

def GetCurrentUrlId():
    config = ConfigParser.ConfigParser()
    config.read('./Config.txt')
    currentId = config.getint('UrlRecord', 'Current')
    return currentId

def SetCurrentUrlId(id):
    config = ConfigParser.ConfigParser()
    config.read('./Config.txt')
    config.set('UrlRecord', 'Current', id)
    config.write(open("./Config.txt", "w"))
    
def BuidFileName(id, url, name):
    fileName = './WeiboData/'
    fileName += str(id)
    fileName += '-'
    fileName += name
    temp = url.replace('/', '#')
    temp = temp.replace(':', '#')
    temp = temp.replace('*', '#')
    temp = temp.replace('?', '#')
    temp = temp.replace('"', '#')
    temp = temp.replace('"\"', '#')
    temp = temp.replace('<', '#')
    temp = temp.replace('>', '#')
    fileName += temp
    fileName += '.txt'
    
    return fileName
    
def Main():
    cookie ='_T_WM=4cef87de351204218cb2850c35301ad0; SUB=_2A256NPlyDeRxGedJ4lYQ8yzFzDyIHXVZ1oc6rDV6PUJbrdAKLRLakW1LHeta-GtGvixZBpSAmRLjNEBNI-Gzcw..; gsid_CTandWM=4uww9b2913nBkMym5QzfU7wJCfE'
    urlRecord = UrlRecord()
    
    currentUrlId = GetCurrentUrlId()
    while (currentUrlId < 1000):
        personUrl = urlRecord.Get(currentUrlId)
        if (personUrl == None):
            break
            
        url = personUrl[0]
        name = personUrl[1]
    
        fileName = BuidFileName(currentUrlId, url, name)
        fp = open(fileName, 'w')
        
        #将用户的微博写入文件中
        people = PeoplePage(url, cookie)
        totalNum = people.GetBlogPageTotalNum()
        for i in range(1, totalNum + 1):
            blogList = people.GetBlogList(i)
            for blog in blogList:
                fp.write(blog[0])
                fp.write('\t')
                fp.write(blog[1])
                fp.write('\t')
                fp.write(blog[2])
                fp.write('\t')
                fp.write('\n')
            print("%s Complete %d" % (name, i * 100/totalNum)) 
        fp.close()

        
        #将当前用户的关注用户写入数据库
        carePageUrl = people.GetCarePeoplePageUrl()
        if (carePageUrl == ''):
            break
            
        carePeoplePage = CarePeoplePage(carePageUrl, cookie)
        totalNum = carePeoplePage.GetPageTotalNum()

        for i in range(1, totalNum + 1):
            peopleList = carePeoplePage.GetPeopleList(i)
            for people in peopleList:
                urlRecord.Add(people[1].decode('utf-8'), people[0].decode('utf-8'))
        
        currentUrlId += 1
        SetCurrentUrlId(currentUrlId)
Main()
