
# -*- coding: utf-8 -*-

import sqlite3


class CommentIdRecord:
    """
    评论Id记录类
    """
    def __init__(self, db_name):
        self.__sqlConn = sqlite3.connect(db_name)
        try:
            self.__sqlConn.execute('''
            CREATE TABLE comment_id
            ( 
            id_name  CHAR (256) PRIMARY KEY NOT NULL UNIQUE
            )''')
            self.__sqlConn.commit()

        except Exception, e:
            print e
            
    def add(self, comment_id):
        """
        增加一个评论Id, 如果失败则表明已经存在该Id
        :param comment_id: id
        :return: 成功True, 失败False
        """
        try:
            self.__sqlConn.execute("INSERT INTO comment_id(id_name) VALUES (?)", (comment_id,))
            self.__sqlConn.commit()
        except Exception, e:
            print e
            return False

        return True

    def __del__(self):
        self.__sqlConn.close()
