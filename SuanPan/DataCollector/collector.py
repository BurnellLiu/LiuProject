#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import tushare as ts


def stock_industry_download():
    """
    下载股票行业分类数据
    """
    df = ts.get_industry_classified()
    df.to_csv('../Data/stock_industry.csv')


def stock_industry_load(file_name):
    """
    加载股票行业分类信息
    :param file_name: 文件名
    :return: {'行业名':[('股票代码', '股票名'), ...]}
    """
    stock_industry = {}
    file = open(file_name)

    is_first_line = True
    for line in file:
        if is_first_line:
            is_first_line = False
            continue
        prop_list = line.split(',')
        industry_name = prop_list[3].strip()
        if industry_name not in stock_industry:
            stock_industry[industry_name] = []
        stock_industry[industry_name].append((prop_list[1], prop_list[2]))

    file.close()
    return stock_industry


def industry_stock_download(name):
    """
    下载某一行业的所有股票数据
    :param name: 行业名称
    """
    industry_data = stock_industry_load('../Data/stock_industry.csv')
    for stock in industry_data[name]:
        stock_code = stock[0]
        stock_name = stock[1]
        print(stock_code)
        print(stock_name)

        df = ts.get_hist_data(stock_code)
        df.to_csv('../Data/%s-%s-%s.csv' % (name, stock_code, stock_name))

if __name__ == '__main__':
    industry_stock_download('电子信息')