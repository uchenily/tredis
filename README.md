## Tredis

实现一个极其简单的kv数据库, 数据只保存在内存.

## 开始

```shell
make
make run
```

## 演示

```shell
(venv) [root@archlinux tredis]# make run
./tredis
==> help
Tiny redis works in memory.

USAGE:
  get [key]
  set [key] [value]
  del [key]
  quit
==> set key1 value1
OK
==> get key1
value1
==> get key2
(nil)
==> del key1
==> get key1
(nil)
==> quit

```
