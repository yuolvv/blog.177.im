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

## 1.单例设计模式

使用设计模式为了代码复用，增加可维护性。

设计模式的六大原则：开闭原则、里氏代换原则、依赖倒转原则、接口隔离原则、迪米特法则（最少知道原则）、合成/聚合复用原则

Singleton（创建）：保证一个类仅有一个实例，并提供一个访问它的全局访问点。如打印机

饿汉式单例模式

```
//饿汉模式：线程安全，耗费资源。
public class HugerSingletonTest {
    //该对象的引用不可修改。还可以将对象的创建放到静态代码块中。
    private static final HugerSingletonTest ourInstance = new HugerSingletonTest();

    public static HugerSingletonTest getInstance() {
        return ourInstance;
    }
    private HugerSingletonTest() {
    }
}
```

懒汉式：非线程安全

```
public class Singleton {
    private static Singleton ourInstance;

    public static Singleton getInstance() {
        if (null == ourInstance) {
            ourInstance = new Singleton();
        }
        return ourInstance;
    }
    private Singleton() {
    }
}
```

懒汉式，线程安全：给方法加锁，消耗资源。

```
public class Singleton {
    private static Singleton ourInstance;

    public synchronized static Singleton getInstance() {
        if (null == ourInstance) {
            ourInstance = new Singleton();
        }
        return ourInstance;
    }
    private Singleton() {
    }
}
```

懒汉式，线程安全：双重检查锁。

```
public class Singleton {
    private static Singleton ourInstance;

    public synchronized static Singleton getInstance() {
        if (null == ourInstance) {
            synchronized (Singleton.class) {
                if (null == ourInstance) {
                    ourInstance = new Singleton();
                }
            }
        }
        return ourInstance;
    }
    private Singleton() {
    }
}
```

分析：
JVM会进行指令重排序，原本的步骤应该是先给 singleton 分配内存，然后调用 Singleton 的构造函数来初始化成员变量，形成实例，最后将singleton对象指向分配的内存空间，但有可能步骤会打乱，就会出现实例非空但没有初始化，抛出异常。将singleton声明成 volatile ，就可以解决该问题。

懒汉式，线程安全：静态内部类

```
public class Singleton {
    private static class SingletonHodler {
        private static Singleton ourInstance = new Singleton();
    }

    public synchronized static Singleton getInstance() {
        return SingletonHodler.ourInstance;
    }

    private Singleton() {
    }
}
```

懒汉式，线程安全：枚举

```
enum SingletonTest {  
    INSTANCE;  
}
```

单例模式在JDK8源码中的使用：Runtime.getRuntime()方法（饿汉式单例模式）

## 2.适配器设计模式

适配器模式（结构）中的角色：目标接口（Target）：客户所期待的接口、需要适配的类（Adaptee）、适配器（Adapter）。

对象适配器（采用对象组合方式实现）

```
//适配器类实现了目标接口
public class Adapter implements Target{
    private Adaptee adaptee ;
    public Adapter() {
        super();
        this.adaptee = new Adaptee();
    }
    @Override
    public void getHeadset2() {
        adaptee.getHeadset3();
    }
    public static void main(String args[]){
        Target target = new Adapter();
        //表面上调用的是2孔插座方法，但其实调用的三孔插座方法。
        target.getHeadset2();
    }
}
interface Target{
    //两孔插座
    void getHeadset2();
}
class Adaptee{
    public void getHeadset3(){
        System.out.println("我是三孔插座！");
    }
}
```

适配器模式在JDK源码的使用

Arrays.asList()，其中Arrays是目标类，内部类ArrayList是适配器类，而Objects.requireNonNull(array);需要适配的类。

InputStreamReader .read()，其中Reader 是目标类，InputStreamReader是适配器类，而StreamDecoder 是需要适配的类。

## 3.模板方法设计模式

模板方法设计模式（行为）使用了继承机制，在抽象类中定义一个模板方法，该方法引用了若干个抽象方法（由子类实现）或具体方法（子类可以覆盖重写）

模板方法设计模式在JDK源码的使用:Collections.sort()、InputStream.read()等