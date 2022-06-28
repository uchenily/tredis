参考:
https://benhoyt.com/writings/hash-table-in-c/
https://craftinginterpreters.com/hash-tables.html


设计原则:

hash表的key存的是字符串(char *类型), 不会是整数或者字符.

hash表内部复制一份key, 外部的key自己处理. 如果外部(调用者)用的是一个数组存key, 那么用这个数组存不同的key时也能生效, 如果用的是一个malloc出来的指针存key, 需要调用者自己在用完后释放空间.

hash表的value是一个指针(void *类型), 外部传进来. 应该是malloc出来的. hash表销毁时会将所有的value指针指向的空间通过free函数自动释放掉.
