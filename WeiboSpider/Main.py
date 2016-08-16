# -*- coding: utf-8 -*-

import ConfigParser
from WeiboPage import *
from UrlRecord import UrlRecord


def get_cookie():
    config = ConfigParser.ConfigParser()
    config.read('./Config.txt')
    cookie = config.get('Cookie', 'Default')
    return cookie


def get_current_url_id():
    config = ConfigParser.ConfigParser()
    config.read('./Config.txt')
    current_id = config.getint('UrlRecord', 'Current')
    return current_id


def set_current_url_id(url_id):
    config = ConfigParser.ConfigParser()
    config.read('./Config.txt')
    config.set('UrlRecord', 'Current', url_id)
    config.write(open("./Config.txt", "w"))


def build_file_name(url_id, url, name):
    file_name = './WeiboData/'
    file_name += str(url_id)
    file_name += '-'
    file_name += name
    temp = url.replace('/', '#')
    temp = temp.replace(':', '#')
    temp = temp.replace('*', '#')
    temp = temp.replace('?', '#')
    temp = temp.replace('"', '#')
    temp = temp.replace('"\"', '#')
    temp = temp.replace('<', '#')
    temp = temp.replace('>', '#')
    file_name += temp
    file_name += '.txt'

    return file_name


def main():
    cookie = get_cookie()
    url_record = UrlRecord()

    current_url_id = get_current_url_id()
    while current_url_id < 1000:
        person_url = url_record.get(current_url_id)
        if person_url is None:
            break

        url = person_url[0]
        name = person_url[1]

        file_name = build_file_name(current_url_id, url, name)
        fp = open(file_name, 'w')

        # 将用户的微博写入文件中
        people = PeoplePage(url, cookie)
        total_num = people.get_blog_page_total_num()
        for i in range(1, total_num + 1):
            blog_list = people.get_blog_list(i)
            for blog in blog_list:
                fp.write(blog[0])
                fp.write('\t')
                fp.write(blog[1])
                fp.write('\t')
                fp.write(blog[2])
                fp.write('\t')
                fp.write('\n')
            print("%s Complete %d" % (name, i * 100 / total_num))
        fp.close()

        # 将当前用户的关注用户写入数据库
        care_page_url = people.get_care_people_page_url()
        print care_page_url
        if care_page_url == '':
            break

        care_people_page = CarePeoplePage(care_page_url, cookie)
        total_num = care_people_page.get_page_total_num()

        for i in range(1, total_num + 1):
            people_list = care_people_page.get_people_list(i)
            for people in people_list:
                url_record.add(people[1].decode('utf-8'), people[0].decode('utf-8'))

        current_url_id += 1
        set_current_url_id(current_url_id)


main()
