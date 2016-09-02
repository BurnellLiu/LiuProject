
# -*- coding: utf-8 -*-

from wsgiref.simple_server import make_server

from hello_web import application

if __name__ == '__main__':
    http_server = make_server('', 8000, application)
    print 'HTTP Server On Port 8000...'
    http_server.serve_forever()


