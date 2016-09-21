#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import time
import re
import hashlib
import json
import logging
import base64
import asyncio

from aiohttp import web

from config import configs
from coreweb import get, post
from models import User, Comment, Blog, generate_id
from apis import APIError, APIValueError, APIResourceNotFoundError

__author__ = 'Burnell Liu'


def generate_user_cookie(user, max_age):
    """
    根据用户信息生成COOKIE
    :param user: 用户
    :param max_age: COOKIE有效时间
    :return: COOKIE字符串
    """
    # 过期时间
    expires = str(int(time.time() + max_age))
    cookie_key = configs.session.secret
    mix_str = '%s-%s-%s-%s' % (user.id, user.password, expires, cookie_key)
    items = [user.id, expires, hashlib.sha1(mix_str.encode('utf-8')).hexdigest()]
    return '-'.join(items)


@get('/')
async def index(request):
    """
    WEB APP首页路由函数
    :param request:
    :return:
    """
    summary = 'Lorem ipsum dolor sit amet, consectetur adipisicing elit, ' \
              'sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.'
    blogs = [
        Blog(id='1', name='Test Blog', summary=summary, created_at=time.time()-120),
        Blog(id='2', name='Something New', summary=summary, created_at=time.time()-3600),
        Blog(id='3', name='Learn Swift', summary=summary, created_at=time.time()-7200)
    ]
    return {
        '__template__': 'blogs.html',
        'blogs': blogs
    }


@get('/register')
def register():
    """
    用户注册路由函数
    :return:
    """
    return {
        '__template__': 'register.html'
    }


@post('/api/users')
async def api_register_user(*, email, name, password):
    """
    用户注册post数据路由函数
    :param email: 用户邮箱
    :param name: 用户名
    :param password: 密码, 传送过来的密码值为: 用户邮箱混合原始密码进行SHA1加密
    :return:
    """
    _RE_EMAIL = re.compile(r'^[a-z0-9\.\-\_]+\@[a-z0-9\-\_]+(\.[a-z0-9\-\_]+){1,4}$')
    _RE_SHA1 = re.compile(r'^[0-9a-f]{40}$')

    # 检查用户数据是否合法
    if not name or not name.strip():
        raise APIValueError('name')
    if not email or not _RE_EMAIL.match(email):
        raise APIValueError('email')
    if not password or not _RE_SHA1.match(password):
        raise APIValueError('password')

    # 检查用户邮箱是否已经被注册
    users = await User.find_all(where='email=?', args=[email])
    if len(users) > 0:
        raise APIError('register:failed', 'email', u'邮箱已经被使用')

    # 生成用户ID, 并且混合用户ID和密码进行SHA1加密
    uid = generate_id()
    uid_password = '%s:%s' % (uid, password)
    sha1_password = hashlib.sha1(uid_password.encode('utf-8')).hexdigest()

    # 生成头像图片URL
    gravatar_url = 'http://www.gravatar.com/avatar/%s?d=mm&s=120' % hashlib.md5(email.encode('utf-8')).hexdigest()

    # 将新用户数据保存到数据库中
    user = User(id=uid, name=name.strip(), email=email, password=sha1_password, image=gravatar_url)
    await user.save()

    # 生成COOKIE
    cookie = generate_user_cookie(user, 86400)
    cookie_name = 'BL_TEK_SESSION'

    # 生成响应消息
    r = web.Response()
    r.set_cookie(cookie_name, cookie, max_age=86400, httponly=True)
    user.password = '******'
    r.content_type = 'application/json'
    r.body = json.dumps(user, ensure_ascii=False).encode('utf-8')
    return r


@get('/api/users')
async def api_get_user():
    """
    WEB API: 获取用户数据路由函数
    :return: 用户数据字典
    """
    users = await User.find_all(order_by='created_at desc')
    for u in users:
        u['password'] = '******'
    return dict(users=users)

