
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
       
       
def WeiboLogin():
    COOKIE ='_T_WM=69d0494866622d11c5b145dd49e1ea85; SUB=_2A256LCwgDeRxGedJ4lYQ8yzFzDyIHXVZ7rRorDV6PUJbstBeLWX7kW1LHet0FjTkE5vx6G808RwtMen4VsFvYQ..; SUBP=0033WrSXqPxfM725Ws9jqgMF55529P9D9W5HzJx-7GkUYjF5QwMzUTFb5JpX5o2p; SUHB=0gHjjKrvCAErmd; SSOLoginState=1462262897; gsid_CTandWM=4uiWCpOz5cQZsScrTmXLy7wJCfE; M_WEIBOCN_PARAMS=featurecode%3D20000181%26fid%3D1005051794132970_-_FOLLOWERS%26uicode%3D10000012; H5_INDEX=0_all; H5_INDEX_TITLE=Burnell_Liu'
    header = { 'Connection' : 'keep-alive', 'cookie' : COOKIE, 'User-Agent' : 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.153 Safari/537.36 SE 2.X MetaSr 1.0' }
    url = 'http://weibo.cn/xiena'
    request = urllib2.Request(url, headers=header) 
    
    try:
        content =urllib2.urlopen(request)
    except:
        print('Open Fail')
        return
    
    soup = BeautifulSoup(content.read())
    #xiena = soup('sc0')
    #print soup.prettify()
    #divList = soup.find('div', id='M_Dtob1EtBb')
    divList = soup('div')
    print divList[13].prettify()

WeiboLogin()
                    