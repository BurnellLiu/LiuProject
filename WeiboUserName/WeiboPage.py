# -*- coding: utf-8 -*-

import urllib.request
from bs4 import BeautifulSoup
from urllib.parse import urljoin


class CarePeoplePage:
    def __init__(self, url, cookie):
        """
        关注人页面类构造函数
        :param url: 关注人页面链接
        :param cookie: 登陆的COOKIE
        """
        header = {
            'Connection': 'keep-alive',
            'cookie': cookie,
            'User-Agent': ('Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) '
                           'Chrome/35.0.1916.153 Safari/537.36 SE 2.X MetaSr 1.0')
        }

        self.__first_page_url = url
        self.__headers = header
        self.__page_total_num = 0

        request = urllib.request.Request(url, headers=self.__headers)
        try:
            content = urllib.request.urlopen(request)
        except Exception as e:
            print(e)
            return

        page_content = content.read()

        # soup = BeautifulSoup(pageContent)
        # pageName = 'CarePeoplePage.txt'
        # fp = open(pageName, 'w')
        # fp.write(soup.prettify().encode('utf-8'))
        # fp.close()

        self.__page_total_num = self.__parse_next_page_total_num(page_content)
        if self.__page_total_num == 0:
            self.__page_total_num = 1

    def get_page_total_num(self):
        """
        获取页面总数
        :return: 页面总数
        """
        return self.__page_total_num

    def get_people_list(self, page_index):
        """
        获取关注人列表
        :param page_index: 页面索引，从1开始
        :return: 关注人列表[(name, url), ...]
        """
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

        request = urllib.request.Request(url, headers=self.__headers)
        try:
            content = urllib.request.urlopen(request)
        except Exception as e:
            print(e)
            print('Network Error')
            return people_list

        page_content = content.read()
        people_list = self.__parse_peoples_url(page_content)

        return people_list

    @staticmethod
    def __parse_peoples_url(page_content):
        """
        解析关注人链接
        :param page_content: 页面HTML内容
        :return: 关注人列表[(name, url), ...]
        """
        people_list = []

        soup = BeautifulSoup(page_content, 'html.parser')
        table_list = soup('table')
        for table in table_list:
            td_list = table('td')
            if len(td_list) != 2:
                continue
            td = td_list[1]
            link = td.a

            if 'href' in dict(link.attrs):
                url = link['href']
                people_list.append((link.string, url))

        return people_list

    @staticmethod
    def __parse_next_page_total_num(page_content):
        """
        解析页面总数
        :param page_content: 页面HTML内容
        :return: 总数
        """
        soup = BeautifulSoup(page_content, 'html.parser')
        div_list = soup('div')
        for div in div_list:
            div_attrs = div.attrs

            if 'class' not in div_attrs or 'id' not in div_attrs:
                continue
            if div_attrs['class'] != ['pa'] or div_attrs['id'] != 'pagelist':
                continue

            input_list = div('input')
            for input_e in input_list:
                input_attrs = input_e.attrs
                if 'name' not in input_attrs or 'type' not in input_attrs or 'value' not in input_attrs:
                    continue

                num = input_attrs['value']
                return int(num)

        return 0


class PeopleInfoPage:
    def __init__(self, url, cookie):
        """
        用户信息页面类构造函数
        :param url: 用户信息页面URL
        :param cookie: 登陆的COOKIE
        """
        header = {
            'Connection': 'keep-alive',
            'cookie': cookie,
            'User-Agent': ('Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) '
                           'Chrome/35.0.1916.153 Safari/537.36 SE 2.X MetaSr 1.0')
        }

        self.__people_info = {}

        request = urllib.request.Request(url, headers=header)
        try:
            content = urllib.request.urlopen(request)
        except Exception as e:
            print(e)
            return

        self.__parse_people_info(content)

    def get_people_info(self):
        """
        获取用户信息
        :return: 用户信息字典
        """
        return self.__people_info

    def __parse_people_info(self, page_content):
        """
        解析用户信息
        :param page_content: 用户信息页面HTML内容
        """

        soup = BeautifulSoup(page_content, 'html.parser')
        div_list = soup.find_all('div', attrs={'class': 'c'})
        for div in div_list:
            div_text = div.get_text()
            if div_text.find(u'昵称') != -1 and div_text.find(u'性别') != -1:
                for info in div.strings:
                    pair_info = info.split(':')
                    if len(pair_info) < 2:
                        continue
                    if info.find(u'昵称') != -1:
                        self.__people_info['name'] = pair_info[1].strip()
                    if info.find(u'性别') != -1:
                        self.__people_info['gender'] = pair_info[1].strip()
                return


class PeoplePage:
    def __init__(self, url, cookie):
        """
        用户页面类构造函数
        :param url: 用户页面URL
        :param cookie: 登陆的COOKIE
        """
        header = {
            'Connection': 'keep-alive',
            'cookie': cookie,
            'User-Agent': ('Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) '
                           'Chrome/35.0.1916.153 Safari/537.36 SE 2.X MetaSr 1.0')
        }

        self.__first_page_url = url
        self.__headers = header
        self.__blog_page_total_num = 0
        self.__care_people_page_url = None
        self.__people_info_page_url = None

        self.__current_state = None;

        request = urllib.request.Request(url, headers=self.__headers)
        try:
            content = urllib.request.urlopen(request)
        except Exception as e:
            print(e)
            self.__current_state = e.__str__()
            return

        page_content = content.read()

        # soup = BeautifulSoup(page_content, 'html.parser')
        # fp = open('xienapage.txt', 'w', encoding='utf-8')
        # fp.write(soup.prettify())
        # fp.close()

        self.__blog_page_total_num = self.__get_blog_page_total_num(page_content)
        if self.__blog_page_total_num == 0:
            self.__blog_page_total_num = 1

        self.__care_people_page_url = self.__parse_care_people_page_url(page_content)

        self.__people_info_page_url = self.__parse_people_info_page_url(page_content)

    def get_state(self):
        return self.__current_state

    def get_people_info_page_url(self):
        return self.__people_info_page_url

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

        request = urllib.request.Request(url, headers=self.__headers)

        try:
            content = urllib.request.urlopen(request)
        except Exception as e:
            print(e)
            print('Network Error')
            return blog_list

        page_content = content.read()
        blog_list = self.__get_blogs_from_page(page_content)

        return blog_list

    def __parse_care_people_page_url(self, page_content):
        """
        解析关注页面链接
        :param page_content:  页面HTML内容
        :return: 关注页面链接
        """

        soup = BeautifulSoup(page_content, 'html.parser')
        div_list = soup('div')
        for div in div_list:
            div_attrs = div.attrs

            if 'class' not in div_attrs:
                continue
            if div_attrs['class'] != ['u']:
                continue

            sub_div_list = div('div')

            for subDiv in sub_div_list:
                sub_div_attrs = subDiv.attrs
                if 'class' not in sub_div_attrs:
                    continue
                if sub_div_attrs['class'] != ['tip2']:
                    continue

                link = subDiv.a
                if 'href' in dict(link.attrs):
                    url = urljoin(self.__first_page_url, link['href'])
                    return url

        return None

    def __parse_people_info_page_url(self, page_content):
        """
        解析用户信息链接
        :param page_content: 用户页面HTML内容
        :return: 用户信息链接
        """
        soup = BeautifulSoup(page_content, 'html.parser')
        a_list = soup.find_all('a', text=u'资料')
        if len(a_list) != 1:
            return None

        url = urljoin(self.__first_page_url, a_list[0]['href'])
        return url

    @staticmethod
    def __get_blog_page_total_num(page_content):
        soup = BeautifulSoup(page_content, 'html.parser')
        div_list = soup('div')
        for div in div_list:
            div_attrs = div.attrs

            if 'class' not in div_attrs or 'id' not in div_attrs:
                continue
            if div_attrs['class'] != ['pa'] or div_attrs['id'] != 'pagelist':
                continue

            input_list = div('input')
            for input in input_list:
                input_attrs = input.attrs
                if ('name' not in input_attrs or
                        'type' not in input_attrs or
                        'value' not in input_attrs):
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
            except Exception as e:
                print(e)
                continue

        return blog_list


def weibo_page_test():
    cookie = ''
    people_page = PeoplePage('http://weibo.cn/xiena', cookie)
    info_url = people_page.get_people_info_page_url()
    if info_url:
        info_page = PeopleInfoPage(info_url, cookie)
        print(info_page.get_people_info())

    care_people_url = people_page.get_care_people_page_url()
    print(care_people_url)
    care_people_page = CarePeoplePage(care_people_url, cookie)
    print(care_people_page.get_page_total_num())
    print(care_people_page.get_people_list(2))

if __name__ == '__main__':
    weibo_page_test()