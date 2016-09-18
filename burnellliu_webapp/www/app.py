#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import logging
import asyncio
import os
import json
import time

from aiohttp import web
from datetime import datetime
from jinja2 import Environment, FileSystemLoader

import orm
import coreweb

__author__ = 'Burnell Liu'


async def logger_factory(app, handler):
    """
    记录URL日志的中间件, 请求被处理前需要被该中间件进行处理
    :param app: WEB应用对象
    :param handler: 处理请求对象
    :return: 中间件处理函数
    """
    async def logger(request):
        logging.info('request: %s %s' % (request.method, request.path))
        return await handler(request)
    return logger


async def response_factory(app, handler):
    """
    处理响应的中间件, 请求被处理后需要被该中间件进行处理
    :param app:
    :param handler:
    :return:
    """
    async def response(request):
        logging.info('response handler...')
        r = await handler(request)
        if isinstance(r, web.StreamResponse):
            return r
        if isinstance(r, dict):
            template = r.get('__template__')
            if template is None:
                resp = web.Response(body=json.dumps(r, ensure_ascii=False, default=lambda o: o.__dict__).encode('utf-8'))
                resp.content_type = 'application/json;charset=utf-8'
                return resp
            else:
                resp = web.Response(body=app['__templating__'].get_template(template).render(**r).encode('utf-8'))
                resp.content_type = 'text/html;charset=utf-8'
                return resp
    return response


def init_jinja2(app, **kw):
    logging.info('init jinja2...')
    options = dict(
        autoescape=kw.get('autoescape', True),
        block_start_string=kw.get('block_start_string', '{%'),
        block_end_string=kw.get('block_end_string', '%}'),
        variable_start_string=kw.get('variable_start_string', '{{'),
        variable_end_string=kw.get('variable_end_string', '}}'),
        auto_reload=kw.get('auto_reload', True)
    )
    path = kw.get('path', None)
    if path is None:
        path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates')
    logging.info('set jinja2 template path: %s' % path)
    env = Environment(loader=FileSystemLoader(path), **options)
    filters = kw.get('filters', None)
    if filters is not None:
        for name, f in filters.items():
            env.filters[name] = f
    app['__templating__'] = env


def datetime_filter(t):
    delta = int(time.time() - t)
    if delta < 60:
        return u'1分钟前'
    if delta < 3600:
        return u'%s分钟前' % (delta // 60)
    if delta < 86400:
        return u'%s小时前' % (delta // 3600)
    if delta < 604800:
        return u'%s天前' % (delta // 86400)
    dt = datetime.fromtimestamp(t)
    return u'%s年%s月%s日' % (dt.year, dt.month, dt.day)


async def app_init(event_loop):
    """
    网站初始化函数
    :param event_loop: 事件循环对象
    :return: 服务器对象
    """
    # 创建数据库连接池
    await orm.create_pool(
        loop=event_loop,
        host='bdm240853593.my3w.com',
        user='bdm240853593',
        password='ttlovelj911',
        db='bdm240853593_db')

    # 创建网站应用对象
    # middlewares 接收一个列表，列表的元素就是拦截器函数
    # aiohttp内部循环里以倒序分别将url处理函数用拦截器装饰一遍
    # 最后再返回经过全部拦截器装饰过的函数, 这样最终调用 url 处理函数之前或之后就可以进行一些额外的处理啦。
    web_app = web.Application(loop=event_loop, middlewares=[logger_factory, response_factory])

    # 初始化前端模板
    init_jinja2(web_app, filters=dict(datetime=datetime_filter))

    # 添加路由函数
    coreweb.add_routes(web_app, 'handlers.py')

    # 添加静态文件
    coreweb.add_static(web_app)

    # 创建服务器
    server = await event_loop.create_server(web_app.make_handler(), '127.0.0.1', 9000)
    logging.info('server started at http://127.0.0.1:9000...')
    return server


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)

    loop = asyncio.get_event_loop()
    loop.run_until_complete(app_init(loop))
    loop.run_forever()
