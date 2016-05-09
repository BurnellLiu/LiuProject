
# -*- coding: utf-8 -*-

import sqlite3


class UrlRecord:
    def __init__(self):
        self.__sqlConn = sqlite3.connect('./DataBase/UrlRecord.db')
        try:
            self.__sqlConn.execute('''
            CREATE TABLE UrlList
            ( 
            Url  CHAR (256) NOT NULL UNIQUE,
            Name CHAR (256) 
            )''')
            self.__sqlConn.commit()
            
            fristUrl =['http://weibo.cn/xiena', u'谢娜']
            self.__sqlConn.execute("INSERT INTO UrlList(Url, Name) VALUES (?, ?)", (fristUrl[0], fristUrl[1]))
            self.__sqlConn.commit()
        except Exception, e:
            print e
            
    def Add(self, url, name):
        try:
            self.__sqlConn.execute("INSERT INTO UrlList(Url, Name) VALUES (?, ?)", (url, name))
            self.__sqlConn.commit()
        except Exception, e:
            print e
        
    def Get(self, rowid):
        cu = self.__sqlConn.execute("SELECT * FROM UrlList Where rowid = ?", (str(rowid)))
        urlList = cu.fetchall()
        cu.close()
        if (len(urlList) != 0):
            return urlList[0]
        else:
            return None
        
        
    def __del__(self):
        self.__sqlConn.close()
