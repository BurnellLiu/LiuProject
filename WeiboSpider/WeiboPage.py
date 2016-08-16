# -*- coding: utf-8 -*-

import urllib2
from bs4 import BeautifulSoup
from urlparse import urljoin


class CarePeoplePage:
    def __init__(self, url, cookie):
        header = {
            'Connection': 'keep-alive',
            'cookie': cookie,
            'User-Agent': ('Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) '
                           'Chrome/35.0.1916.153 Safari/537.36 SE 2.X MetaSr 1.0')
        }

        self.__first_page_url = url
        self.__headers = header
        self.__page_total_num = 0

        request = urllib2.Request(url, headers=self.__headers)
        try:
            content = urllib2.urlopen(request)
        except Exception, e:
            print e
            return

        page_content = content.read()

        # soup = BeautifulSoup(pageContent)
        # pageName = 'CarePeoplePage.txt'
        # fp = open(pageName, 'w')
        # fp.write(soup.prettify().encode('utf-8'))
        # fp.close()

        self.__page_total_num = self.__get_next_page_total_num(page_content)
        if self.__page_total_num == 0:
            self.__page_total_num = 1

    def get_page_total_num(self):
        return self.__page_total_num

    def get_people_list(self, page_index):
        people_list = []
        if page_index < 1 or page_index > self.__page_total_num:
            return people_list

        url = ''
        if page_index == 1:
            url = self.__first_page_url
        else:
            url = self.__first_page_url
            url += '?page='
            url += str(page_index)

        request = urllib2.Request(url, headers=self.__headers)
        try:
            content = urllib2.urlopen(request)
        except Exception, e:
            print e
            print 'Network Error'
            return people_list

        page_content = content.read()
        people_list = self.__get_peoples_from_page(page_content)

        return people_list

    @staticmethod
    def __get_peoples_from_page(page_content):
        people_list = []

        soup = BeautifulSoup(page_content)
        table_list = soup('table')
        for table in table_list:
            td_list = table('td')
            if len(td_list) != 2:
                continue
            td = td_list[1]
            link = td.a

            if 'href' in dict(link.attrs):
                url = link['href']
                people_list.append([link.string.encode('utf-8'), url.encode('utf-8')])

        return people_list

    @staticmethod
    def __get_next_page_total_num(page_content):
        soup = BeautifulSoup(page_content)
        div_list = soup('div')
        for div in div_list:
            div_attrs = div.attrs

            if not div_attrs.has_key('class') or not div_attrs.has_key('id'):
                continue
            if div_attrs['class'] != ['pa'] or div_attrs['id'] != 'pagelist':
                continue

            input_list = div('input')
            for input in input_list:
                input_attrs = input.attrs
                if (not input_attrs.has_key('name') or
                        not input_attrs.has_key('type') or
                        not input_attrs.has_key('value')):
                    continue

                num = input_attrs['value']
                return int(num)

        return 0


class PeoplePage:
    def __init__(self, url, cookie):
        header = {
            'Connection': 'keep-alive',
            'cookie': cookie,
            'User-Agent': ('Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) '
                           'Chrome/35.0.1916.153 Safari/537.36 SE 2.X MetaSr 1.0')
        }

        self.__first_page_url = url
        self.__headers = header
        self.__blog_page_total_num = 0
        self.__care_people_page_url = ''

        request = urllib2.Request(url, headers=self.__headers)
        try:
            content = urllib2.urlopen(request)
        except Exception, e:
            print e
            return

        page_content = content.read()

        # soup = BeautifulSoup(pageContent)
        # fp = open('xienapage.txt', 'w')
        # fp.write(soup.prettify().encode('utf-8'))
        # fp.close()

        self.__blog_page_total_num = self.__get_blog_page_total_num(page_content)
        if self.__blog_page_total_num == 0:
            self.__blog_page_total_num = 1

        self.__care_people_page_url = self.__get_care_people_page_url(page_content)

    def get_people_data(self):
        pass

    def get_care_people_page_url(self):
        return self.__care_people_page_url

    def get_blog_page_total_num(self):
        return self.__blog_page_total_num

    def get_blog_list(self, page_index):

        blog_list = []
        if page_index < 1 or page_index > self.__blog_page_total_num:
            return blog_list

        url = ''
        if page_index == 1:
            url = self.__first_page_url
        else:
            url = self.__first_page_url
            url += '?page='
            url += str(page_index)

        request = urllib2.Request(url, headers=self.__headers)

        try:
            content = urllib2.urlopen(request)
        except Exception, e:
            print e
            print('Network Error')
            return blog_list

        page_content = content.read()
        blog_list = self.__get_blogs_from_page(page_content)

        return blog_list

    @staticmethod
    def __get_blog_page_total_num(page_content):
        soup = BeautifulSoup(page_content)
        div_list = soup('div')
        for div in div_list:
            div_attrs = div.attrs

            if not div_attrs.has_key('class') or not div_attrs.has_key('id'):
                continue
            if div_attrs['class'] != ['pa'] or div_attrs['id'] != 'pagelist':
                continue

            input_list = div('input')
            for input in input_list:
                input_attrs = input.attrs
                if (not input_attrs.has_key('name') or
                        not input_attrs.has_key('type') or
                        not input_attrs.has_key('value')):
                    continue

                num = input_attrs['value']
                return int(num)

        return 0

    @staticmethod
    def __get_blogs_from_page(page_content):
        soup = BeautifulSoup(page_content)

        blog_list = []

        div_list = soup('div')
        for div in div_list:
            div_attrs = div.attrs

            if not div_attrs.has_key('class') or not div_attrs.has_key('id'):
                continue
            if div_attrs['class'] != ['c']:
                continue

            blog_id = div_attrs['id']
            blog_text = ''
            blog_time = ''

            span_list = div('span')
            for span in span_list:
                span_attrs = span.attrs

                if not span_attrs.has_key('class'):
                    continue
                if span_attrs['class'] != ['ctt'] and span_attrs['class'] != ['cmt'] and span_attrs['class'] != ['ct']:
                    continue

                if blog_text == '' and (span_attrs['class'] == ['ctt'] or span_attrs['class'] == ['cmt']):
                    for string in span.strings:
                        blog_text += string

                if span_attrs['class'] == ['ct']:
                    for string in span.strings:
                        blog_time += string

            try:
                blog_list.append([blog_id.encode('utf-8'), blog_text.encode('utf-8'), blog_time.encode('utf-8')])
            except Exception, e:
                print  e
                continue

        return blog_list

    def __get_care_people_page_url(self, page_content):
        url = ''

        soup = BeautifulSoup(page_content)
        div_list = soup('div')
        for div in div_list:
            div_attrs = div.attrs

            if not div_attrs.has_key('class'):
                continue
            if div_attrs['class'] != ['u']:
                continue

            sub_div_list = div('div')

            for subDiv in sub_div_list:
                sub_div_attrs = subDiv.attrs
                if not sub_div_attrs.has_key('class'):
                    continue
                if sub_div_attrs['class'] != ['tip2']:
                    continue

                link = subDiv.a
                if 'href' in dict(link.attrs):
                    url = urljoin(self.__first_page_url, link['href'])
                    return url.encode('utf-8')

        return url
