
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
            
            frist_url =['http://weibo.cn/xiena', u'谢娜']
            self.__sqlConn.execute("INSERT INTO UrlList(Url, Name) VALUES (?, ?)", (frist_url[0], frist_url[1]))
            self.__sqlConn.commit()
        except Exception, e:
            print e
            
    def add(self, url, name):
        try:
            self.__sqlConn.execute("INSERT INTO UrlList(Url, Name) VALUES (?, ?)", (url, name))
            self.__sqlConn.commit()
        except Exception, e:
            print e
        
    def get(self, rowid):
        str_id = str(rowid)
        sql = "SELECT * FROM UrlList Where rowid = "
        sql += "'"
        sql += str_id
        sql += "'"
        cu = self.__sqlConn.execute(sql)
        url_list = cu.fetchall()
        cu.close()
        if len(url_list) != 0:
            return url_list[0]
        else:
            return None

    def __del__(self):
        self.__sqlConn.close()
