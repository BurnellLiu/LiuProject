# -*- coding: utf-8 -*-


import time
import hashlib
from comment_page import CommentPage
from comment_id_record import CommentIdRecord
from config_file import *


def get_real_time_comment():
    cookie = get_cookie(1)
    url = get_url()

    comment_page = CommentPage(url, cookie)

    id_record = CommentIdRecord('./data/comment_id_record.db')

    for i in range(1, 21):
        comment_list = comment_page.get_comments(i)
        file_object = open('./data/comments.txt', 'a')

        if len(comment_list) == 0:
            print 'Page Error'

        valid_comment = 0
        for comment in comment_list:
            md5 = hashlib.md5(comment.encode('utf-8'))
            if id_record.add(md5.hexdigest()):
                file_object.write(comment.encode('utf-8'))
                file_object.write('\r\n')
                valid_comment += 1

        file_object.close()

        print 'Page total: ', len(comment_list), 'valid: ', valid_comment
        print 'Complete: ', i * 100.0/20

        time.sleep(5)


if __name__ == '__main__':
    get_real_time_comment()
