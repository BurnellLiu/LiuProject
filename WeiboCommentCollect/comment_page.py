# -*- coding: utf-8 -*-

import urllib2
from bs4 import BeautifulSoup


class CommentPage:
    """
    微博评论页解析类
    """
    def __init__(self, page_url, user_cookie):
        """
        构造函数
        :param page_url: 评论首页URL字符串
        :param user_cookie: COOKIE字符串
        """
        header = {
            'Connection': 'keep-alive',
            'Cookie': user_cookie,
            'User-Agent': ('Mozilla/5.0 (Windows NT 10.0; WOW64) '
                           'AppleWebKit/537.36 (KHTML, like Gecko) '
                           'Chrome/48.0.2564.116 Safari/537.36')
        }

        self.__first_page_url = page_url
        self.__http_headers = header
        self.__pages_total_num = 0

        request = urllib2.Request(page_url, headers=self.__http_headers)
        try:
            html_page = urllib2.urlopen(request).read()
        except Exception, e:
            print e
            return

        self.__pages_total_num = self.__get_total_pages(html_page)

    def change_cookie(self, user_cookie):
        """
        改变COOKIE值
        :param user_cookie: COOKIE字符串
        :return: None
        """
        self.__http_headers['Cookie'] = user_cookie

    def get_pages_total_num(self):
        """
        获取评论页面总数
        :return: 页面总数
        """
        return self.__pages_total_num

    def get_comments(self, page_index):
        """
        获取某页的评论
        :param page_index: 页索引, 从1开始
        :return: 评论列表
        """
        if page_index < 1 or page_index > self.__pages_total_num:
            return []

        page_url = ''
        referer_url = ''
        if page_index == 1:
            page_url = self.__first_page_url
        else:
            loc = self.__first_page_url.find('#cmtfrm')
            page_url = self.__first_page_url[:loc]
            page_url += '?page='
            referer_url = page_url
            page_url += str(page_index)
            referer_url += str(page_index-1)

        self.__http_headers['Referer'] = referer_url
        print page_url
        request = urllib2.Request(page_url, headers=self.__http_headers)
        try:
            html_page = urllib2.urlopen(request).read()
        except Exception, e:
            print e
            return []

        return self.__get_comments(html_page)

    @staticmethod
    def __get_comments(html_page):
        """
        获取网页中的评论
        :param html_page: HTML格式的文本字符串
        :return: 评论字列表(id, comment)
        """
        comment_list = []

        soup = BeautifulSoup(html_page, "html.parser")

        # fp = open('comment.html', 'w')
        # fp.write(soup.prettify().encode('utf-8'))
        # fp.close()

        div_list = soup('div')
        for div in div_list:
            div_attrs = div.attrs
            if not div_attrs.has_key('class'):
                continue
            if div_attrs['class'] != ['c']:
                continue
            if not div_attrs.has_key('id'):
                continue
            if div_attrs['id'] == 'M_':
                continue

            span_list = div('span')
            for span in span_list:
                span_attrs = span.attrs
                if not span_attrs.has_key('class'):
                    continue
                if span_attrs['class'] != ['ctt']:
                    continue

                comment = ''.join(span.strings)
                comment_list.append(comment)
                break
        return comment_list

    @staticmethod
    def __get_total_pages(html_page):
        """
        获取总页面数量
        :param html_page: HTML格式的文本字符串
        :return: 页面总数量
        """
        soup = BeautifulSoup(html_page, "html.parser")
        div_list = soup('div')
        for div in div_list:
            div_attrs = div.attrs
            if not div_attrs.has_key('class'):
                continue
            if div_attrs['class'] != ['pa']:
                continue
            if not div_attrs.has_key('id'):
                continue

            if div_attrs['id'] == 'pagelist':
                input_list = div('input')
                for ele_input in input_list:
                    input_attrs = ele_input.attrs
                    if not input_attrs.has_key('name'):
                        continue
                    if not input_attrs.has_key('type'):
                        continue
                    if not input_attrs.has_key('value'):
                        continue
                    if input_attrs['name'] != 'mp':
                        continue

                    return int(input_attrs['value'])

        return 0


