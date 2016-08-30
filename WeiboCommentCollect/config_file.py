# -*- coding: utf-8 -*-


import ConfigParser


def get_cookie(index):
    config = ConfigParser.ConfigParser()
    config.read('./config.txt')
    cookie = config.get('default', 'cookie' + str(index))
    return cookie


def get_url():
    config = ConfigParser.ConfigParser()
    config.read('./config.txt')
    cookie = config.get('default', 'url')
    return cookie


def get_total_page():
    config = ConfigParser.ConfigParser()
    config.read('./config.txt')
    total_index = config.getint('default', 'total_page')
    return total_index


def get_current_page_index():
    config = ConfigParser.ConfigParser()
    config.read('./config.txt')
    current_index = config.getint('default', 'current_page')
    return current_index


def set_current_page_index(page_index):
    config = ConfigParser.ConfigParser()
    config.read('./config.txt')
    config.set('default', 'current_page', page_index)
    config.write(open("./Config.txt", "w"))