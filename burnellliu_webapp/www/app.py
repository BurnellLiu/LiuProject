#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import logging
import asyncio
from aiohttp import web

__author__ = 'Burnell Liu'

logging.basicConfig(level=logging.INFO)


def index(request):
    return web.Response(body=b'<h1>hello</h1>')


async def init(loop):
    app = web.Application(loop=loop)
    app.router.add_route('GET', '/', index)
    server = await loop.create_server(app.make_handler(), '127.0.0.1', 9000)
    logging.info('server started at http://127.0.0.1:9000...')
    return server


event_loop = asyncio.get_event_loop()
event_loop.run_until_complete(init(event_loop))
event_loop.run_forever()
