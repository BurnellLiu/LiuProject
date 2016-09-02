
# -*- coding: utf-8 -*-


def application(environ, start_response):
    print environ['REQUEST_METHOD']
    print environ['PATH_INFO']
    # 发送HTTP响应头
    start_response('200 OK', [('Content-Type', 'text/html')])
    # 返回HTTP Body
    return '<h1>Hello, Web!</h1>'


