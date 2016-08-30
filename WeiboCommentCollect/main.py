# -*- coding: utf-8 -*-


import time
import hashlib
from comment_page import CommentPage
from comment_id_record import CommentIdRecord
from config_file import *


def collect_comment(cookie_index):
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
    for i in range(1000):
        comment_list = comment_page.get_comments(current_page + dif)
        if len(comment_list) == 0:
            time.sleep(1)
            continue

        file_object = open('./data/comments.txt', 'a')

        for comment in comment_list:
            md5 = hashlib.md5(comment.encode('utf-8'))
            if id_record.add(md5.hexdigest()):
                file_object.write(comment.encode('utf-8'))
                file_object.write('\r\n')

        file_object.close()

        print 'Complete: ', cookie_index, '  ', i * 100.0/1000

        current_page += 1
        set_current_page_index(current_page)

        time.sleep(0.4)


def main():
    for i in range(1, 5):
        collect_comment(i)


if __name__ == '__main__':
    main()
