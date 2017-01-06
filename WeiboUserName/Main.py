# -*- coding: utf-8 -*-

import configparser
import time
from WeiboPage import *
from UrlRecord import UrlRecord


def get_cookie(cookie_index):
    config = configparser.ConfigParser()
    config.read('./Config.txt', encoding='utf-8')

    cookie_name = str(cookie_index)
    cookie = config.get('cookie', cookie_name, raw=True)
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
    fp_male = open('./male.txt', 'a', encoding='utf-8')
    fp_female = open('./female.txt', 'a', encoding='utf-8')

    cookie_index = 1
    url_record = UrlRecord()
    current_url_id = get_current_url_id()

    sleep_time = 4

    while True:
        print('current cookie index is', cookie_index)
        cookie = get_cookie(cookie_index)
        print('current url id is', current_url_id)

        people_url = url_record.get(current_url_id)
        if people_url is None:
            print("people url is None")
            break

        url = people_url[0]

        people = PeoplePage(url, cookie)
        time.sleep(sleep_time)
        state = people.get_state()
        if state:
            if state.find('HTTP Error 403: Forbidden') != -1:
                break

        info_url = people.get_people_info_page_url()
        if info_url is None:
            print('info url is None')
            current_url_id += 1
            set_current_url_id(current_url_id)
            continue

        print(info_url)
        info_page = PeopleInfoPage(info_url, cookie)
        time.sleep(sleep_time)
        info = info_page.get_people_info()

        # print(info)
        if 'name' in info and 'gender' in info:
            name = info['name']
            # print(name)
            if info['gender'] == u'男':
                fp_male.write(name)
                fp_male.write('\n')
                fp_male.flush()
            elif info['gender'] == u'女':
                fp_female.write(name)
                fp_female.write('\n')
                fp_female.flush()

        # 将当前用户的关注用户写入数据库
        care_page_url = people.get_care_people_page_url()
        if care_page_url is None:
            print('care page url is None')
            current_url_id += 1
            set_current_url_id(current_url_id)
            continue

        care_people_page = CarePeoplePage(care_page_url, cookie)
        time.sleep(sleep_time)
        people_list = care_people_page.get_people_list(1)
        time.sleep(sleep_time)
        for people in people_list:
            url_record.add(people[1], people[0])

        current_url_id += 1
        set_current_url_id(current_url_id)

    fp_male.close()
    fp_female.close()

        
if __name__ == '__main__':
    main()
