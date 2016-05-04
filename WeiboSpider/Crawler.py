
# -*- coding: utf-8 -*-

import urllib2
from bs4 import BeautifulSoup
from urlparse import urljoin
import sys 

def Crawl(pagesList, depth = 2):
    for i in range(depth):
        newPages = set()
        for page in pagesList:
            try:
                c = urllib2.urlopen(page)
            except:
                print 'Could not open %s' % page
                continue
            
            try:
                soup = BeautifulSoup(c.read())
            except:
                print "Can't Paser"
                continue
                
            links = soup('a')
            for link in links:
                if ('href' in dict(link.attrs)):
                    url = urljoin(page, link['href'])
                    #print link
                    print url
                    if url[0:4] == 'http':
                        newPages.add(url)

        pagesList = newPages
        

def GetBlogFromPage(pageContent):

    soup = BeautifulSoup(pageContent)
    
    fp = open('page.txt', 'w')
    fp.write(soup.prettify().encode('utf-8'))
    fp.close()
    
    blogDict = []
    
    fp = open('xx.txt', 'w')    
    divList = soup('div')
    
    for div in divList:
        divAttrs = div.attrs
        
        if (not divAttrs.has_key(u'class') or not divAttrs.has_key('id')):
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
                blogTime = span.string
        
        fp.write(blogId.encode('utf-8'))
        fp.write('  ')
        fp.write(blogText.encode('utf-8'))
        fp.write('  ')
        fp.write(blogTime.encode('utf-8'))
        fp.write('\n')
        fp.write('\n')    
            
    
       
       
def WeiboLogin():
    COOKIE ='_T_WM=34fdb0229023a2bbbd2158fce59ed838; SUB=_2A256Ld3ODeRxGedJ4lYQ8yzFzDyIHXVZ0eOGrDV6PUJbstBeLRH6kW1LHetKQuM-Or4E3fjLmMbCn2CplXbpkA..; SUBP=0033WrSXqPxfM725Ws9jqgMF55529P9D9W5HzJx-7GkUYjF5QwMzUTFb5JpX5o2p; SUHB=04WvQCvoE3WI_D; SSOLoginState=1462349214; gsid_CTandWM=4uEICpOz5mEXV3zeThpDS7wJCfE'
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
    
    GetBlogFromPage(content.read())

WeiboLogin()
                    