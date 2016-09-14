#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import functools


__author__ = 'Burnell Liu'


def get(path):
    """
    定义装饰器 @get('/path')
    :param path: URL路径
    """
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kw):
            return func(*args, **kw)
        wrapper.__method__ = 'GET'
        wrapper.__route__ = path
        return wrapper
    return decorator


def post(path):
    """
    定义装饰器 @post('/path')
    :param path: URL路径
    """
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kw):
            return func(*args, **kw)
        wrapper.__method__ = 'POST'
        wrapper.__route__ = path
        return wrapper
    return decorator

