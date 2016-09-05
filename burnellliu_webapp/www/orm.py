#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import asyncio, logging

import aiomysql

__author__ = 'Burnell Liu'


def log(sql, args=()):
    logging.info('SQL: [%s] args: %s' % (sql, args or []))


async def create_pool(loop, **kw):
    """
    创建连接池
    :param loop: 事件循环对象
    """
    logging.info('create database connection pool...')
    # 创建一个全局的连接池，每个HTTP请求都可以从连接池中直接获取数据库连接。
    # 使用连接池的好处是不必频繁地打开和关闭数据库连接，而是能复用就尽量复用。
    # 连接池由全局变量__pool存储，缺省情况下将编码设置为utf8，自动提交事务
    global __pool
    __pool = await aiomysql.create_pool(
        host=kw.get('host', 'localhost'),
        port=kw.get('port', 3306),
        user=kw['user'],
        password=kw['password'],
        db=kw['db'],
        charset=kw.get('charset', 'utf8'),
        autocommit=kw.get('autocommit', True),
        maxsize=kw.get('maxsize', 10),
        minsize=kw.get('minsize', 1),
        loop=loop
    )


async def select(sql, args, size=None):
    log(sql, args)
    global __pool
    async with __pool.get() as conn:
        async with conn.cursor(aiomysql.DictCursor) as cur:
            await cur.execute(sql.replace('?', '%s'), args or ())
            if size:
                rs = await cur.fetchmany(size)
            else:
                rs = await cur.fetcall()
        logging.info('rows returned: %s' % len(rs))
        return rs


test = select(None, None)
test.send(None)

