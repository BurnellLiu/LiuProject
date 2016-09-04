# -*- coding: utf-8 -*-


import asyncio

async def hello():
    print("Hello world!")
    r = await asyncio.sleep(1)
    print("Hello again!")


loop = asyncio.get_event_loop()

loop.run_until_complete(hello())
loop.close()

