---
layout: post
title: 快速定位Java线上内存OOM问题
slug: locate-java-oom
date: 2020-01-02 18:00
status: publish
author: 177
categories: 
  - 极客
tags:
  - Java
  - IT
excerpt: Java线上内存OOM问题，如何快速定位？？？
---

[notice]Java的OOM问题，各式各样。[/notice]

## 1、使用top查看PID

## 2、使用jmap -heap PID 查询JVM内存信息，确认是否是内存本身就分配的不够

## 3、使用jmap -histo:live PID | more 查找实例列表中最耗内存的对象

其中按照所占内存排序，分别是实例数，所在内存大小，类名。通过列表可以直观的

找到最耗内存的类名，然后到代码中找到对应的类进行排查问题。

## 4、确认是否资源耗尽

使用pstree -up PID 查看进程线程树

使用netstat查看网络连接数

还可以通过

ll /proc/${PID}/fd 查看句柄详情

ll /proc/${PID}/task 查看线程数

统计判断进程打开句柄和线程数，只需要简单统计一下

ll /proc/${PID}/fd | wc -l

ll /proc/${PID}/task | wc -l

pstree -p |wc -l