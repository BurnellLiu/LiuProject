# -*- coding: utf-8 -*-

import configparser
import time
from WeiboPage import *
from UrlRecord import UrlRecord


def get_cookie():
    config = configparser.ConfigParser()
    config.read('./Config.txt', encoding='utf-8')
    cookie = config.get('cookie', 'default', raw=True)
    return cookie


def get_current_url_id():
    config = configparser.ConfigParser()
    config.read('./Config.txt')
    current_id = config.getint('urlrecord', 'current')
    return current_id


def set_current_url_id(url_id):
    config = configparser.ConfigParser()
    config.read('./Config.txt')
    config.set('urlrecord', 'current', str(url_id))
    config.write(open("./Config.txt", "w"))


def main():
    fp_male = open('male.txt', 'a', encoding='utf-8')
    fp_female = open('female.txt', 'a', encoding='utf-8')

    cookie = get_cookie()
    url_record = UrlRecord()

    current_url_id = get_current_url_id()
    while True:
        print('current url id is', current_url_id)

        people_url = url_record.get(current_url_id)
        if people_url is None:
            print("people url is None")
            break

        url = people_url[0]

        people = PeoplePage(url, cookie)

        info_url = people.get_people_info_page_url()
        if info_url is None:
            print('info url is None')
            break

        info_page = PeopleInfoPage(info_url, cookie)
        info = info_page.get_people_info()
        print(info)
        if 'name' in info and 'gender' in info:
            if info['gender'] == u'男':
                fp_male.write(info['name'])
                fp_male.write('\n')
                fp_male.flush()
            elif info['gender'] == u'女':
                fp_female.write(info['name'])
                fp_female.write('\n')
                fp_female.flush()

        # 将当前用户的关注用户写入数据库
        care_page_url = people.get_care_people_page_url()
        if care_page_url is None:
            print('care page url is None')
            break

        care_people_page = CarePeoplePage(care_page_url, cookie)
        people_list = care_people_page.get_people_list(1)
        for people in people_list:
            url_record.add(people[1], people[0])

        current_url_id += 1
        set_current_url_id(current_url_id)

        time.sleep(1)

    fp_male.close()
    fp_female.close()

        
if __name__ == '__main__':
    main()
