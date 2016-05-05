
# -*- coding: utf-8 -*-

import urllib2
from bs4 import BeautifulSoup

class CarePeoplePage:
    def __init__(self, url, cookie):
        self.url = url
        
        header = { 
                   'Connection' : 'keep-alive', 
                   'cookie' : cookie, 
                   'User-Agent' : 'JUC (Linux; U; 2.3.7; zh-cn; MB200; 320*480) UCWEB7.9.3.103/139/999' 
                 }
        self.headers = header
        
        request = urllib2.Request(url, headers=self.headers) 
    
        try:
            content =urllib2.urlopen(request)
        except:
            print('Open Fail')
            return
    
        pageContent = content.read()
        soup = BeautifulSoup(pageContent)
        pageName = 'CarePeoplePage.txt'
        fp = open(pageName, 'w')
        fp.write(soup.prettify().encode('utf-8'))
        fp.close()
        
    def GetPeopleList(self):
        pass
    

def GetBlogPageTotalNum(pageContent):
    soup = BeautifulSoup(pageContent)
    divList = soup('div') 
    for div in divList:
        divAttrs = div.attrs
        
        if (not divAttrs.has_key('class') or not divAttrs.has_key('id')):
            continue
        if (divAttrs['class'] != ['pa'] or divAttrs['id'] != 'pagelist'):
            continue
        
        inputList = div('input')
        for input in inputList:
            inputAttrs = input.attrs
            if (not inputAttrs.has_key('name') or 
                not inputAttrs.has_key('type') or
                not inputAttrs.has_key('value')):
                continue
            
            num = inputAttrs['value']
            return int(num)
            
    return 0   
            

def GetBlogsFromPage(pageContent):

    soup = BeautifulSoup(pageContent)
    
    blogList = []
    
    divList = soup('div') 
    for div in divList:
        divAttrs = div.attrs
        
        if (not divAttrs.has_key('class') or not divAttrs.has_key('id')):
            continue
        if (divAttrs['class'] != ['c']):
            continue

        blogId = divAttrs['id']
        blogText = ''
        blogTime = ''
        
        spanList = div('span')
        for span in spanList:
            spanAttrs = span.attrs
            
            if (not spanAttrs.has_key('class')):
                continue
            if (spanAttrs['class'] != ['ctt'] and spanAttrs['class'] != ['cmt'] and spanAttrs['class'] != ['ct']):
                continue
            
            if (blogText == '' and (spanAttrs['class'] == ['ctt'] or spanAttrs['class'] == ['cmt'])):
                for string in span.strings:
                    blogText += string
            
            if (spanAttrs['class'] == ['ct']):
                for string in span.strings:
                    blogTime += string
        
        try:
            blogList.append([blogId.encode('utf-8'), blogText.encode('utf-8'), blogTime.encode('utf-8')])
        except:
            continue
        
        
            
    return blogList
       
       
def WeiboLogin():

    #title = soup.title.string
    #pageName = title
    #pageName += '_page.txt'
    #fp = open(pageName, 'w')
    #fp.write(soup.prettify().encode('utf-8'))
    #fp.close()

    COOKIE ='_T_WM=34fdb0229023a2bbbd2158fce59ed838; gsid_CTandWM=4ue8CpOz5611twdEuRLtb7wJCfE; SUB=_2A256LteIDeRxGedJ4lYQ8yzFzDyIHXVZ0PnArDV6PUJbstBeLRTdkW1LHeubGRqHxPYch01CanIatWUY7tNOmQ..; SUBP=0033WrSXqPxfM725Ws9jqgMF55529P9D9W5HzJx-7GkUYjF5QwMzUTFb5JpX5o2p; SUHB=0h1Il6Dnq_KWr0; SSOLoginState=1462413272'
    header = { 
    'Connection' : 'keep-alive', 
    'cookie' : COOKIE, 
    'User-Agent' : 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.153 Safari/537.36 SE 2.X MetaSr 1.0' }
    
    url = 'http://weibo.cn/xiena'
    request = urllib2.Request(url, headers=header) 
    
    try:
        content =urllib2.urlopen(request)
    except:
        print('Open Fail')
        return
    
    pageContent = content.read()
    
    weiboName = 'xiena.txt'
    fp = open(weiboName, 'w')
    blogList = GetBlogsFromPage(pageContent)
    for blog in blogList:
        fp.write(blog[0])
        fp.write('  ')
        fp.write(blog[1])
        fp.write('  ')
        fp.write(blog[2])
        fp.write('\n')
        fp.write('\n')  
    
    pageNum = GetBlogPageTotalNum(pageContent)
    for i in range(2, pageNum + 1):
        newUrl = url
        newUrl += '?page='
        newUrl += str(i)
        print newUrl
        request = urllib2.Request(newUrl, headers=header) 
        try:
            content =urllib2.urlopen(request)
        except:
            continue
            
        pageContent = content.read()
        blogList = GetBlogsFromPage(pageContent)
        for blog in blogList:
            fp.write(blog[0])
            fp.write('  ')
            fp.write(blog[1])
            fp.write('  ')
            fp.write(blog[2])
            fp.write('\n')
            fp.write('\n')
        
        print i*100/pageNum
    
    fp.close()

#WeiboLogin()
COOKIE ='_T_WM=34fdb0229023a2bbbd2158fce59ed838; gsid_CTandWM=4ue8CpOz5611twdEuRLtb7wJCfE; SUB=_2A256LteIDeRxGedJ4lYQ8yzFzDyIHXVZ0PnArDV6PUJbstBeLRTdkW1LHeubGRqHxPYch01CanIatWUY7tNOmQ..; SUBP=0033WrSXqPxfM725Ws9jqgMF55529P9D9W5HzJx-7GkUYjF5QwMzUTFb5JpX5o2p; SUHB=0h1Il6Dnq_KWr0; SSOLoginState=1462413272'   
url = 'http://weibo.cn/1192329374/follow'
carePeoplePage = CarePeoplePage(url, COOKIE)
             