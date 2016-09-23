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
from apis import Page, APIError, APIValueError, APIResourceNotFoundError, APIPermissionError

__author__ = 'Burnell Liu'


def get_page_index(page_str):
    p = 1
    try:
        p = int(page_str)
    except ValueError as e:
        pass
    if p < 1:
        p = 1
    return p


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


@get('/signin')
def signin():
    """
    用户登录路由函数
    """
    return {
        '__template__': 'signin.html'
    }


@get('/signout')
def signout(request):
    """
    用户登出路由函数
    :param request: 请求对象
    :return:
    """
    referer = request.headers.get('Referer')
    r = web.HTTPFound(referer or '/')
    cookie_name = configs.session.cookie_name
    r.set_cookie(cookie_name, '-deleted-', max_age=0, httponly=True)
    logging.info('user signed out.')
    return r


@get('/manage/blogs')
def manage_blogs(*, page='1'):
    """
    管理博客路由函数
    :param page:
    :return:
    """
    return {
        '__template__': 'manage_blogs.html',
        'page_index': get_page_index(page)
    }


@get('/manage/blogs/create')
def manage_create_blog():
    """
    创建博客路由函数
    :return:
    """
    return {
        '__template__': 'manage_blog_edit.html',
        'id': '',
        'action': '/api/blogs'
    }


def check_admin(request):
    """
    检查是否是管理员用户
    :param request: 请求对象
    :return: 如果当前用户不是管理员则抛出异常
    """
    if request.__user__ is None or not request.__user__['admin']:
        raise APIPermissionError()


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
    mix_str = '%s-%s-%s-%s' % (user['id'], user['password'], expires, cookie_key)
    items = [user['id'], expires, hashlib.sha1(mix_str.encode('utf-8')).hexdigest()]
    return '-'.join(items)


def generate_sha1_password(user_id, original_password):
    """
    根据用户id和原始密码, 生成一个进过sha1加密的密码
    :param user_id: 用户id
    :param original_password: 原始密码
    :return: 加密密码
    """
    uid_password = '%s:%s' % (user_id, original_password)
    sha1_password = hashlib.sha1(uid_password.encode('utf-8')).hexdigest()
    return sha1_password


@post('/api/authenticate')
async def api_user_authenticate(*, email, password):
    """
    用户登录验证API函数
    :param email: 用户邮箱
    :param password: 用户密码
    :return: 回响消息, 并且设置COOKIE
    """
    if not email:
        raise APIValueError(email, u'非法邮箱账号')
    if not password:
        raise APIValueError(password, u'非法密码')

    users = await User.find_all(where='email=?', args=[email])
    if len(users) == 0:
        raise APIError('authenticate:fail', email, u'邮箱账号不存在')

    user = users[0]
    sha1_password = generate_sha1_password(user['id'], password)
    if user['password'] != sha1_password:
        raise APIError('authenticate:fail', 'invalid password', u'密码错误')

    cookie_name = configs.session.cookie_name
    r = web.Response()
    r.set_cookie(cookie_name, generate_user_cookie(user, 86400), max_age=86400, httponly=True)
    user['password'] = '******'
    r.content_type = 'application/json'
    r.body = json.dumps(user, ensure_ascii=False).encode('utf-8')
    return r


@get('/api/users')
async def api_get_users():
    """
    WEB API: 获取用户数据API函数
    :return: 用户数据字典
    """
    users = await User.find_all(order_by='created_at desc')
    for u in users:
        u['password'] = '******'
    return dict(users=users)


@post('/api/users')
async def api_register_user(*, email, name, password):
    """
    用户注册API函数
    :param email: 用户邮箱
    :param name: 用户名
    :param password: 密码, 传送过来的密码值为: 用户邮箱混合原始密码进行SHA1加密
    :return:
    """
    _RE_EMAIL = re.compile(r'^[a-z0-9\.\-\_]+\@[a-z0-9\-\_]+(\.[a-z0-9\-\_]+){1,4}$')
    _RE_SHA1 = re.compile(r'^[0-9a-f]{40}$')

    # 检查用户数据是否合法
    if not name or not name.strip():
        raise APIValueError(name, u'用户名非法')
    if not email or not _RE_EMAIL.match(email):
        raise APIValueError(email, u'邮箱账号非法')
    if not password or not _RE_SHA1.match(password):
        raise APIValueError(password, '密码非法')

    # 检查用户邮箱是否已经被注册
    users = await User.find_all(where='email=?', args=[email])
    if len(users) > 0:
        raise APIError('register:fail', email, u'邮箱已经被使用')

    # 生成用户ID, 并且混合用户ID和密码进行SHA1加密
    uid = generate_id()
    sha1_password = generate_sha1_password(uid, password)

    # 生成头像图片URL
    gravatar_url = 'http://www.gravatar.com/avatar/%s?d=mm&s=120' % hashlib.md5(email.encode('utf-8')).hexdigest()

    # 将新用户数据保存到数据库中
    user = User(id=uid, name=name.strip(), email=email, password=sha1_password, image=gravatar_url)
    await user.save()

    # 生成COOKIE
    cookie = generate_user_cookie(user, 86400)
    cookie_name = configs.session.cookie_name

    # 生成响应消息
    r = web.Response()
    r.set_cookie(cookie_name, cookie, max_age=86400, httponly=True)
    user['password'] = '******'
    r.content_type = 'application/json'
    r.body = json.dumps(user, ensure_ascii=False).encode('utf-8')
    return r


@get('/api/blogs')
async def api_blogs(*, page='1'):
    page_index = get_page_index(page)
    num = await Blog.find_number('count(id)')
    p = Page(num, page_index)
    if num == 0:
        return dict(page=p, blogs=())
    blogs = await Blog.find_all(orderBy='created_at desc', limit=(p.offset, p.limit))
    return dict(page=p, blogs=blogs)


@post('/api/blogs')
async def api_create_blog(request, *, name, summary, content):
    check_admin(request)
    if not name or not name.strip():
        raise APIValueError('name', u'博客名不能为空')

    if not summary or not summary.strip():
        raise APIValueError('summary', u'摘要不能为空')

    if not content or not content.strip():
        raise APIValueError('content', u'内容不能为空')

    blog = Blog(user_id=request.__user__['id'],
                user_name=request.__user__['name'],
                user_image=request.__user__['image'],
                name=name.strip(),
                summary=summary.strip(),
                content=content.strip())
    await blog.save()
    return blog
