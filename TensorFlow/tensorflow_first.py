# -*- coding: utf-8 -*-

import tensorflow as tf


def tensorflow_graph():
    # 创建一个常量op, 产生一个1*2矩阵. 这个op被作为一个节点加到默认图中
    # 构造器返回值代表该常量op的返回值
    matrix1 = tf.constant([[3.0, 3.0]])

    # 创建另一个常量op, 产生一个2*1矩阵.
    matrix2 = tf.constant([[2.0], [2.0]])

    # 创建一个矩阵乘法 matmul op, 把matrix1和matrix2作为输入
    # 返回值product代表矩阵乘法的结果
    product = tf.matmul(matrix1, matrix2)

    # 启动默认图
    sess = tf.Session()

    # 函数调用run(product) 触发了默认图中3个op的执行
    result = sess.run(product)

    print result

    sess.close()


def tensorflow_variable():
    # 变量维护图执行过程中的状态信息

    # 创建一个变量, 初始化为标量0
    state = tf.Variable(0, name="counter")

    # 创建一个op, 起作用是使state增加1
    one = tf.constant(1)
    new_value = tf.add(state, one)
    update = tf.assign(state, new_value)

    # 启动图后, 变量必须先经过'初始化' (init) op 初始化
    init_op = tf.initialize_all_variables()

    # 启动图, 运行op
    with tf.Session() as sess:
        # 运行'init' op
        sess.run(init_op)

        # 打印state的初始值
        print sess.run(state)

        # 运行op, 更新state, 打印state
        for _ in range(3):
            sess.run(update)
            print sess.run(state)


def tensorflow_fetch():
    """需要获取的tensor值, 在op的一次运行中获得(而不是逐个去获取tensor)
    """

    input1 = tf.constant(3.0)
    input2 = tf.constant(2.0)
    input3 = tf.constant(5.0)

    intermed = tf.add(input2, input3)
    mul = tf.mul(input1, intermed)

    with tf.Session() as sess:
        result = sess.run([mul, intermed])
        print result


def tensorflow_feed():
    input1 = tf.placeholder(tf.types.float32)
    input2 = tf.placeholder(tf.types.float32)
    output = tf.mul(input1, input2)

    with tf.Session() as sess:
        result = sess.run([output], feed_dict={input1: [7.], input2: [2.]})
        print result

tensorflow_feed()



















