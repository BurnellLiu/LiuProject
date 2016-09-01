# -*- coding: utf-8 -*-


import time
import hashlib
from comment_page import CommentPage
from comment_id_record import CommentIdRecord
from config_file import *


def history_collect(cookie_index):
    cookie = get_cookie(cookie_index)
    total_page = get_total_page()
    current_page = get_current_page_index()
    url = get_url()

    comment_page = CommentPage(url, cookie)
    new_total_page = comment_page.get_pages_total_num()
    if new_total_page == 0 or new_total_page < total_page:
        return

    id_record = CommentIdRecord('./data/comment_id_record.db')

    dif = new_total_page-total_page
    for i in range(1000000):
        comment_list = comment_page.get_comments(current_page + dif)
        if len(comment_list) == 0:
            time.sleep(5)
            print 'Page Error'
            continue

        file_object = open('./data/comments.txt', 'a')

        valid_comment = 0
        for comment in comment_list:
            md5 = hashlib.md5(comment.encode('utf-8'))
            if id_record.add(md5.hexdigest()):
                file_object.write(comment.encode('utf-8'))
                file_object.write('\r\n')
                valid_comment += 1

        file_object.close()

        print 'Page total: ', len(comment_list), 'valid: ', valid_comment

        current_page += 10
        set_current_page_index(current_page)

        time.sleep(5)


if __name__ == '__main__':
    history_collect(1)
