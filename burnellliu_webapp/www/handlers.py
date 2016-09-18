#!/usr/bin/env python3
# -*- coding: utf-8 -*-

__author__ = 'Burnell Liu'


from coreweb import get, post
from models import User, Comment, Blog, next_id


@get('/')
async def index(request):
    users = await User.find_all()
    return {
        '__template__': 'test.html',
        'users': users
    }