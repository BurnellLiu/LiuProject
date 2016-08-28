# -*- coding: utf-8 -*-

import urllib2
from bs4 import BeautifulSoup
from urlparse import urljoin


class CommentPage:
    """
    微博评论页解析类
    """
    def __init__(self, url, cookie):
        """
        构造函数
        :param url: 评论首页URL字符串
        :param cookie: COOKIE字符串
        """
        header = {
            'Connection': 'keep-alive',
            'cookie': cookie,
            'User-Agent': ('Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) '
                           'Chrome/35.0.1916.153 Safari/537.36 SE 2.X MetaSr 1.0')
        }

        self.__first_page_url = url
        self.__http_headers = header
        self.__pages_total_num = 0

        request = urllib2.Request(url, headers=self.__http_headers)
        try:
            html_page = urllib2.urlopen(request).read()
        except Exception, e:
            print e
            return

        self.__pages_total_num = self.__get_total_pages(html_page)

    def get_pages_total_num(self):
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
        if page_index == 1:
            page_url = self.__first_page_url
        else:
            loc = self.__first_page_url.find('#cmtfrm')
            page_url = self.__first_page_url[:loc]
            page_url += '?page='
            page_url += str(page_index)

        print page_url
        request = urllib2.Request(page_url, headers=self.__http_headers)
        try:
            html_page = urllib2.urlopen(request).read()
        except Exception, e:
            print e
            print('Network Error')
            return []

        return self.__get_comments(html_page)

    @staticmethod
    def __get_comments(html_page):
        """
        获取网页中的评论
        :param html_page: HTML格式的文本字符串
        :return: 评论字符串列表
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


if __name__ == '__main__':
    url = 'http://weibo.cn/comment/E3NfT4Eqe?uid=1828817187&rl=0#cmtfrm'
    cookie = ('_T_WM=b9f4647c7ae68070b126a5911ef8dfde; ALF=1474959398; '
              'SCF=Av8kfMFJJbJ0r-aWlj3udPHEJz7K4jPab_SbxuieuQI6y3T1-TplmJjkNjN9sMn4FyJETV8yhm_eGENxCuVU0ls.; '
              'SUB=_2A256xvz0DeTxGedJ4lYQ8yzFzDyIHXVWSIS8rDV6PUJbktANLVb9kW0qeBrykEMnyDNCfwQ4ra-KagG-Fw..; '
              'SUBP=0033WrSXqPxfM725Ws9jqgMF55529P9D9W5HzJx-7GkUYjF5QwMzUTFb5JpX5o2p5NHD95QpS0.'
              'XeKeE1KM7Ws4DqcjzBcv.9-pLdcRt; '
              'SUHB=0tKrHurYpk1bGd; SSOLoginState=1472367780')

    comment_page = CommentPage(url, cookie)

    print comment_page.get_pages_total_num()

    print comment_page.get_comments(1)
    print comment_page.get_comments(2)