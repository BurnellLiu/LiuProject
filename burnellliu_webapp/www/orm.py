#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import asyncio
import logging
import aiomysql

__author__ = 'Burnell Liu'

logging.basicConfig(level=logging.INFO)


def log_sql(sql, args=()):
    logging.info('SQL: [%s] args: %s' % (sql, args or []))


async def create_pool(loop, **kw):
    """
    创建连接池
    :param loop: 事件循环对象
    """
    logging.info('Create database connection pool...')
    # 创建一个全局的连接池，每个HTTP请求都可以从连接池中直接获取数据库连接。
    # 使用连接池的好处是不必频繁地打开和关闭数据库连接，而是能复用就尽量复用。
    # 连接池由全局变量__pool存储，缺省情况下将编码设置为utf8，自动提交事务
    global __pool
    __pool = await aiomysql.create_pool(
        host=kw.get('host', 'localhost'),
        port=kw.get('port', 3306),
        user=kw['user'],
        password=kw['password'],
        db=kw['db'],
        charset=kw.get('charset', 'utf8'),
        autocommit=kw.get('autocommit', True),
        maxsize=kw.get('maxsize', 10),
        minsize=kw.get('minsize', 1),
        loop=loop
    )


async def select(sql, args, size=None):
    """
    执行SELECT语句
    :param sql: SQL语句
    :param args: SQL参数
    :param size: 获取指定数量的记录
    :return:
    """
    log_sql(sql, args)
    global __pool
    async with __pool.get() as conn:
        async with conn.cursor(aiomysql.DictCursor) as cur:
            # SQL语句的占位符是?，而MySQL的占位符是%s，所以需要替换
            await cur.execute(sql.replace('?', '%s'), args or ())
            if size:
                rs = await cur.fetchmany(size)
            else:
                rs = await cur.fetcall()
        logging.info('rows returned: %s' % len(rs))
        return rs


async def execute(sql, args, autocommit=True):
    """
    通用执行语句
    :param sql: SQL语句
    :param args: SQL参数
    :param autocommit: 是否自动提交
    :return: 受影响的行数
    """
    log_sql(sql, args)
    async with __pool.get() as conn:
        if not autocommit:
            await conn.begin()
        try:
            async with conn.cursor(aiomysql.DictCursor) as cur:
                await cur.execute(sql.replace('?', '%s'), args)
                affected = cur.rowcount
            if not autocommit:
                await conn.commit()
        except BaseException as e:
            if not autocommit:
                await conn.rollback()
            raise
        return affected


def create_args_string(num):
    array = []
    for n in range(num):
        array.append('?')
    return ', '.join(array)


class Field(object):
    """
    数据库表字段基类
    """
    def __init__(self, name, column_type, primary_key, default):
        """
        表字段构造函数
        :param name: 字段名称
        :param column_type: 字段类型
        :param primary_key: 标记是否为主键
        :param default: 默认值
        """
        self.name = name
        self.column_type = column_type
        self.primary_key = primary_key
        self.default = default

    def __str__(self):
        return '<%s, %s:%s>' % (self.__class__.__name__, self.column_type, self.name)


class StringField(Field):
    """
    字符串字段类
    """
    def __init__(self, name=None, primary_key=False, default=None, ddl='varchar(100)'):
        super().__init__(name, ddl, primary_key, default)


class BooleanField(Field):
    """
    布尔字段类
    """
    def __init__(self, name=None, default=False):
        super().__init__(name, 'boolean', False, default)


class IntegerField(Field):
    """
    整数字段类
    """
    def __init__(self, name=None, primary_key=False, default=0):
        super().__init__(name, 'bigint', primary_key, default)


class FloatField(Field):
    """
    浮点数字段类
    """
    def __init__(self, name=None, primary_key=False, default=0.0):
        super().__init__(name, 'real', primary_key, default)


class TextField(Field):
    """
    文本字段类
    """
    def __init__(self, name=None, default=None):
        super().__init__(name, 'text', False, default)


class ModelMetaclass(type):
    """
    数据表模型元类
    """
    def __new__(mcs, name, bases, attrs):
        """
        创建类
        :param mcs: 准备创建的类对象
        :param name: 类的名字
        :param bases: 类继承的父类集合
        :param attrs: 类的属性集合(包括方法)
        """
        if name == 'Model':
            return type.__new__(mcs, name, bases, attrs)

        table_name = attrs.get('__table__', None) or name
        logging.info('Found model: %s (table: %s)' % (name, table_name))

        field_dict = dict()
        field_key_list = []
        field_primary_key = None
        for k, v in attrs.items():
            # 找出类中的所有字段属性
            if isinstance(v, Field):
                logging.info('Found mapping: %s ==> %s' % (k, v))
                field_dict[k] = v
                if v.primary_key:
                    # 找到主键:
                    if field_primary_key:
                        raise BaseException('Duplicate primary key for field: %s' % k)
                    field_primary_key = k
                else:
                    field_key_list.append(k)

        if not field_primary_key:
            raise BaseException('Primary key not found.')

        for k in field_dict.keys():
            attrs.pop(k)

        escaped_fields = list(map(lambda f: '`%s`' % f, field_key_list))
        attrs['__mappings__'] = field_dict # 保存属性和列的映射关系
        attrs['__table__'] = table_name
        attrs['__primary_key__'] = field_primary_key # 主键属性名
        attrs['__fields__'] = field_key_list # 除主键外的属性名
        attrs['__select__'] = 'select `%s`, %s from `%s`' % (field_primary_key, ', '.join(escaped_fields), table_name)
        attrs['__insert__'] = 'insert into `%s` (%s, `%s`) values (%s)' % (table_name, ', '.join(escaped_fields), field_primary_key, create_args_string(len(escaped_fields) + 1))
        attrs['__update__'] = 'update `%s` set %s where `%s`=?' % (table_name, ', '.join(map(lambda f: '`%s`=?' % (field_dict.get(f).name or f), field_key_list)), field_primary_key)
        attrs['__delete__'] = 'delete from `%s` where `%s`=?' % (table_name, field_primary_key)
        return type.__new__(mcs, name, bases, attrs)


class Model(dict, metaclass=ModelMetaclass):

    def __init__(self, **kw):
        super(Model, self).__init__(**kw)

    def __getattr__(self, key):
        try:
            return self[key]
        except KeyError:
            raise AttributeError(r"'Model' object has no attribute '%s'" % key)

    def __setattr__(self, key, value):
        self[key] = value

    def getValue(self, key):
        return getattr(self, key, None)

    def getValueOrDefault(self, key):
        value = getattr(self, key, None)
        if value is None:
            field = self.__mappings__[key]
            if field.default is not None:
                value = field.default() if callable(field.default) else field.default
                logging.debug('using default value for %s: %s' % (key, str(value)))
                setattr(self, key, value)
        return value

    @classmethod
    async def findAll(cls, where=None, args=None, **kw):
        ' find objects by where clause. '
        sql = [cls.__select__]
        if where:
            sql.append('where')
            sql.append(where)
        if args is None:
            args = []
        orderBy = kw.get('orderBy', None)
        if orderBy:
            sql.append('order by')
            sql.append(orderBy)
        limit = kw.get('limit', None)
        if limit is not None:
            sql.append('limit')
            if isinstance(limit, int):
                sql.append('?')
                args.append(limit)
            elif isinstance(limit, tuple) and len(limit) == 2:
                sql.append('?, ?')
                args.extend(limit)
            else:
                raise ValueError('Invalid limit value: %s' % str(limit))
        rs = await select(' '.join(sql), args)
        return [cls(**r) for r in rs]

    @classmethod
    async def findNumber(cls, selectField, where=None, args=None):
        ' find number by select and where. '
        sql = ['select %s _num_ from `%s`' % (selectField, cls.__table__)]
        if where:
            sql.append('where')
            sql.append(where)
        rs = await select(' '.join(sql), args, 1)
        if len(rs) == 0:
            return None
        return rs[0]['_num_']

    @classmethod
    async def find(cls, pk):
        ' find object by primary key. '
        rs = await select('%s where `%s`=?' % (cls.__select__, cls.__primary_key__), [pk], 1)
        if len(rs) == 0:
            return None
        return cls(**rs[0])

    async def save(self):
        args = list(map(self.getValueOrDefault, self.__fields__))
        args.append(self.getValueOrDefault(self.__primary_key__))
        rows = await execute(self.__insert__, args)
        if rows != 1:
            logging.warn('failed to insert record: affected rows: %s' % rows)

    async def update(self):
        args = list(map(self.getValue, self.__fields__))
        args.append(self.getValue(self.__primary_key__))
        rows = await execute(self.__update__, args)
        if rows != 1:
            logging.warn('failed to update by primary key: affected rows: %s' % rows)

    async def remove(self):
        args = [self.getValue(self.__primary_key__)]
        rows = await execute(self.__delete__, args)
        if rows != 1:
            logging.warn('failed to remove by primary key: affected rows: %s' % rows)


class User(Model):
    __table__ = 'users'

    id = IntegerField(primary_key=True)
    name = StringField()


user = User(id=123, name='Michael')


"""
event_loop.run_until_complete(create_pool(event_loop, host='bdm240853593.my3w.com',
                                          user='bdm240853593', password='ttlovelj911',
                                          db='bdm240853593_db'))
                                          """


