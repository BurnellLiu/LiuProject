#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import functools
import inspect


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


def get_required_kw_args(fn):
    """
    获取目标函数的没有默认值的命名关键字参数
    :param fn: 目标函数
    :return: 函数参数元组
    """
    args = []
    params = inspect.signature(fn).parameters
    for name, param in params.items():
        print(name, param.kind, param.default)
        if param.kind == inspect.Parameter.KEYWORD_ONLY and param.default == inspect.Parameter.empty:
            args.append(name)
    return tuple(args)


def get_named_kw_args(fn):
    """
    获取指定函数的命名关键字参数
    :param fn: 目标函数
    :return: 函数参数元组
    """
    args = []
    params = inspect.signature(fn).parameters
    for name, param in params.items():
        if param.kind == inspect.Parameter.KEYWORD_ONLY:
            args.append(name)
    return tuple(args)


def has_named_kw_args(fn):
    """
    检查目标函数是否存在命名关键字参数
    :param fn: 目标函数
    :return: True, False
    """
    params = inspect.signature(fn).parameters
    for name, param in params.items():
        if param.kind == inspect.Parameter.KEYWORD_ONLY:
            return True
    return False


def has_var_kw_arg(fn):
    """
    检查目标函数是否存在关键字参数
    :param fn: 目标函数
    :return: True, False
    """
    params = inspect.signature(fn).parameters
    for name, param in params.items():
        if param.kind == inspect.Parameter.VAR_KEYWORD:
            return True
    return False


if __name__ == '__main__':
    pass

