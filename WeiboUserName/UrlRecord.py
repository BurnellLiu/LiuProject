
# -*- coding: utf-8 -*-

import sqlite3


class UrlRecord:
    def __init__(self):
        self.__sqlConn = sqlite3.connect('./UrlRecord.db')
        try:
            self.__sqlConn.execute('''
            create table url_list
            ( 
            url  char (256) not null unique,
            name char (256)
            )''')
            self.__sqlConn.commit()
            
            first_url =['http://weibo.cn/xiena', u'谢娜']
            self.__sqlConn.execute("insert into url_list(url, name) values (?, ?)", (first_url[0], first_url[1]))
            self.__sqlConn.commit()
        except Exception as e:
            print(e)
            
    def add(self, url, name):
        try:
            self.__sqlConn.execute("insert into url_list(url, name) VALUES (?, ?)", (url, name))
            self.__sqlConn.commit()
        except Exception as e:
            print(e)
        
    def get(self, rowid):
        str_id = str(rowid)
        sql = "select * from url_list where rowid = "
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


def test_url_record():
    url_record = UrlRecord();

if __name__ == '__main__':
    test_url_record()

