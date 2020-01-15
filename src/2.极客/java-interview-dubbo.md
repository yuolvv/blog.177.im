---
layout: post
title: Java面试题-Dubbo
slug: java-interview-dubbo
date: 2020-01-11 12:00
status: publish
author: 177
categories: 
  - 极客
tags:
  - Java
  - 面试题
  - IT
  - Dubbo
excerpt: Java面试题-Dubbo-收集整理
---

[notice]从网络上收集整理的Java面试题，如有侵权，请联系删除！[/notice]

## 1、测试和生产共用一套zookeeper，怎么保证消费不冲突 ？

方案1：服务发布的group设置为不同；

```
  <dubbo:reference id="comm1" timeout="100000" interface="com.acq.facade.CommService" group="comm102" version="1.0.0" retries="0" check="false">
  </dubbo:reference>

  <dubbo:reference id="comm2" timeout="100000" interface="com.acq.facade.CommService" group="comm103" version="1.0.0" retries="0" check="false">
  </dubbo:reference>
```

方案2：

```
<dubbo:service interface="com.foo.BarService" version="1.0.0" ></dubbo:service>
```

方案3：dubbo白名单（Filter过滤器）

1.新增一个类继承阿里巴巴的Filter

2.添加阿里巴巴的约定新建配置文件 com.alibaba.dubbo.rpc.Filter，指定上面的类

3.配置ip白名单

4.配置服务端dubbo配置

## 2、dubbo运行时,突然所有的zookeeper全部宕机,dubbo是否还会继续提供服务？

会的，dubbo根据本地缓存的服务地址进行服务调用

监控中心宕掉不影响使用，只是丢失部分采样数据

数据库宕掉后，注册中心仍能通过缓存提供服务列表查询，但不能注册新服务

注册中心对等集群，任意一台宕掉后，将自动切换到另一台

注册中心全部宕掉后，服务提供者和服务消费者仍能通过本地缓存通讯

服务提供者无状态，任意一台宕掉后，不影响使用

服务提供者全部宕掉后，服务消费者应用将无法使用，并无限次重连等待服务提供者恢复

## 3、服务提供者能实现失效踢出是什么原理？

基于zookeeper的临时节点原理

持久节点

所谓持久节点,是指在节点创建后,就一直存在,直到有删除操作来主动清除这个节点,也就是说不会因为创建该节点的客户端会话失效而消失

临时节点

临时节点的生命周期和客户端会话绑定,也就是说,如果客户端会话失效,那么这个节点就会自动被清除掉

## 4、创建的临时节点什么时候会被删除，是连接一断就删除吗？延时是多少？

连接断了之后，ZK不会马上移除临时数据，只有当SESSIONEXPIRED之后，才会把这个会话建立的临时数据移除。因此，用户需要谨慎设置Session_TimeOut

## 5、Dubbo在安全机制方面是如何解决的 ？

Dubbo通过Token令牌防止用户绕过注册中心直连，然后在注册中心上管理授权。Dubbo还提供服务黑白名单，来控制服务所允许的调用方。

## 6、说说核心的配置有哪些？

dubbo:service/ 服务提供者暴露服务配置

dubbo:reference/ 服务消费者引用服务配置

dubbo:protocol/ 服务提供者协议配置

dubbo:registry/ 注册中心配置

dubbo:application/ 应用信息配置

dubbo:provider/ 服务提供者缺省值配置

dubbo:consumer/ 服务消费者缺省值配置

dubbo:method/ 方法级配置