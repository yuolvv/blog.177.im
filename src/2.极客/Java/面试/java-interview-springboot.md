---
layout: post
title: Java面试题-SpringBoot
slug: java-interview-springboot
date: 2020-01-09 12:00
status: publish
author: 177
categories: 
  - 极客
tags:
  - Java
  - 面试题
  - IT
  - SpringBoot
excerpt: Java面试题SpringBoot收集整理
---

[notice]从网络上收集整理的Java面试题，如有侵权，请联系删除！[/notice]

## 1、什么是 Spring Boot？

Spring Boot 是 Spring 开源组织下的子项目，是 Spring 组件一站式处理方案，主要是简化了使用 Spring 的难度，简省了繁重的配置，提供了各种启动器，开发者能快速上手。

## 2、为什么要用 Spring Boot？

Spring Boot 优点非常多，如：

    - 独立运行

    - 简化配置

    - 自动配置

    - 无代码生成和XML配置

    - 应用监控

    - 上手容易…..

## 3、Spring Boot 的核心配置文件有哪几个？它们的区别是什么？

    - Spring Boot 的核心配置文件是 application 和 bootstrap 配置文件。

    - application 配置文件这个容易了解，主要用于 Spring Boot 项目的自动化配置。

    - bootstrap 配置文件有以下几个应用场景。

    - 使用 Spring Cloud Config 配置中心时，这时需要在 bootstrap 配置文件中增加连接到配置中心的配置属性来加载外部配置中心的配置信息；

    - 少量固定的不能被覆盖的属性；

    - 少量加密/解密的场景；

## 4、Spring Boot 的配置文件有哪几种格式？它们有什么区别？

.properties 和 .yml，它们的区别主要是书写格式不同。

    1).properties

    2).yml

另外，.yml 格式不支持@PropertySource注解导入配置。

## 5、Spring Boot 的核心注解是哪个？它主要由哪几个注解组成的？

启动类上面的注解是@SpringBootApplication，它也是 Spring Boot 的核心注解，主要组合包含了以下 3 个注解：

@SpringBootConfiguration：组合了 @Configuration 注解，实现配置文件的功能。

@EnableAutoConfiguration：打开自动配置的功能，也可以关闭某个自动配置的选项，如关闭数据源自动配置功能：@SpringBootApplication(exclude = { DataSourceAutoConfiguration.class })。

@ComponentScan：Spring组件扫描。

## 6、开启 Spring Boot 特性有哪几种方式？

1）继承spring-boot-starter-parent项目

2）导入spring-boot-dependencies项目依赖

## 7、Spring Boot 需要独立的容器运行吗？

可以不需要，内置了 Tomcat/ Jetty 等容器。

## 8、运行 Spring Boot 有哪几种方式？

1）打包用命令或者者放到容器中运行

2）用 Maven/ Gradle 插件运行

3）直接执行 main 方法运行

## 9.springboot常用的starter有哪些?

spring-boot-starter-web 嵌入tomcat和web开发需要servlet与jsp支持

spring-boot-starter-data-jpa 数据库支持

spring-boot-starter-data-redis redis数据库支持

spring-boot-starter-data-solr solr支持

mybatis-spring-boot-starter 第三方的mybatis集成starter

## 10、Spring Boot 自动配置原理是什么？

注解 @EnableAutoConfiguration, @Configuration, @ConditionalOnClass 就是自动配置的核心，首先它得是一个配置文件，其次根据类路径下能否有这个类去自动配置。

## 11、Spring Boot 的目录结构是怎么的？

这个目录结构是主流及推荐的做法，而在主入口类上加上 @SpringBootApplication 注解来开启 Spring Boot 的各项能力，如自动配置、组件扫描等。

## 12、你如何了解 Spring Boot 中的 Starters？

Starters可以了解为启动器，它包含了一系列可以集成到应用里面的依赖包，你可以一站式集成 Spring 及其余技术，而不需要四处找示例代码和依赖包。

如你想使用 Spring JPA 访问数据库，只需加入 spring-boot-starter-data-jpa 启动器依赖就能使用了。

Starters包含了许多项目中需要用到的依赖，它们能快速持续的运行，都是一系列得到支持的管理传递性依赖。

## 13、如何在 Spring Boot 启动的时候运行少量特定的代码？

可以实现接口 ApplicationRunner 或者者 CommandLineRunner，这两个接口实现方式一样，它们都只提供了一个 run 方法

## 14、Spring Boot 有哪几种读取配置的方式？

Spring Boot 可以通过 @PropertySource,@Value,@Environment, @ConfigurationProperties 来绑定变量

## 15、Spring Boot 支持哪些日志框架？推荐和默认的日志框架是哪个？

Spring Boot 支持 Java Util Logging, Log4j2, Lockback 作为日志框架，假如你使用 Starters 启动器，Spring Boot 将使用 Logback 作为默认日志框架

## 16、SpringBoot 实现热部署有哪几种方式？

主要有两种方式：

Spring Loaded

Spring-boot-devtools

## 17、你如何了解 Spring Boot 配置加载顺序？

在 Spring Boot 里面，可以使用以下几种方式来加载配置。

1）properties文件；

2）YAML文件；

3）系统环境变量；

4）命令行参数；

等等……

## 18、Spring Boot 如何定义多套不同环境配置？

提供多套配置文件，如：

运行时指定具体的配置文件

## 19、Spring Boot 可以兼容老 Spring 项目吗，如何做？

可以兼容，使用@ImportResource注解导入老 Spring 项目配置文件。

## 20、保护 Spring Boot 应用有哪些方法？

在生产中使用HTTPS

使用Snyk检查你的依赖关系

更新到最新版本

启用CSRF保护

使用内容安全策略防止XSS攻击…

## 21、Spring Boot 2.X 有什么新特性？与 1.X 有什么区别？

配置变更

JDK 版本更新

第三方类库更新

响应式 Spring 编程支持

HTTP/2 支持

配置属性绑定

更多改进与增强…

## 22.什么是Spring Profiles？

Spring Profiles允许用户根据配置文件（dev，test，prod等）来注册bean。

因此，当应用程序在开发中运行时，只有某些bean可以加载，而在PRODUCTION中，某些其他bean可以加载。

假设我们的要求是Swagger文档仅适用于QA环境，并且禁用所有其他文档。这可以使用配置文件来完成。Spring Boot使得使用配置文件非常简单。

## 23.什么是Spring Batch？

Spring Boot Batch提供可重用的函数，这些函数在处理大量记录时非常重要，包括日志/跟踪，事务管理，作业处理统计信息，作业重新启动，跳过和资源管理。

它还提供了更先进的技术服务和功能，通过优化和分区技术，可以实现极高批量和高性能批处理作业。

简单以及复杂的大批量批处理作业可以高度可扩展的方式利用框架处理重要大量的信息。

## 24.什么是FreeMarker模板？

FreeMarker是一个基于Java的模板引擎，最初专注于使用MVC软件架构进行动态网页生成。使用Freemarker的主要优点是表示层和业务层的完全分离。

程序员可以处理应用程序代码，而设计人员可以处理html页面设计。最后使用freemarker可以将这些结合起来，给出最终的输出页面。

## 25.什么是JavaConfig？

Spring JavaConfig是Spring社区的产品，它提供了配置Spring IoC容器的纯Java方法。因此它有助于避免使用XML配置。

使用JavaConfig的优点在于：

面向对象的配置。由于配置被定义为JavaConfig中的类，因此用户可以充分利用Java中的面向对象功能。一个配置类可以继承另一个，重写它的@Bean方法等。

减少或消除XML配置。基于依赖注入原则的外化配置的好处已被证明。但是，许多开发人员不希望在XML和Java之间来回切换。

JavaConfig为开发人员提供了一种纯Java方法来配置与XML配置概念相似的Spring容器。

从技术角度来讲，只使用JavaConfig配置类来配置容器是可行的，但实际上很多人认为将JavaConfig与XML混合匹配是理想的。

类型安全和重构友好。JavaConfig提供了一种类型安全的方法来配置Spring容器。由于Java 5.0对泛型的支持，现在可以按类型而不是按名称检索bean，不需要任何强制转换或基于字符串的查找

## 26.启动类注解：

@SpringBootConfiguration:Spring Boot的配置类; 标注在某个类上，表示这是一个Spring Boot的配置类; @Configuration:配置类上来标注这个注解;

配置类 —– 配置文件;配置类也是容器中的一个组件;@Component@EnableAutoConfiguration:开启自动配置功能;

以前我们需要配置的东西，Spring Boot帮我们自动配置;@EnableAutoConfiguration告诉SpringBoot开启自动配置功能;这样自动配置才能生效;

Spring Boot在启动的时候从类路径下的META-INF/spring.factories中获取EnableAutoConfiguration指定的值，将这些值作为自动配置类导入到容器中，自动配置类就失效，帮我们进行自动配置工作

## 27.配置文件的加载顺序?

由jar包外向jar包内进行寻找;

优先加载带profile

jar包外部的application-{profile}.properties或application.yml(带spring.profile)配置文件

jar包内部的application-{profile}.properties或application.yml(带spring.profile)配置文件

再来加载不带profile

jar包外部的application.properties或application.yml(不带spring.profile)配置文件

jar包内部的application.properties或application.yml(不带spring.profile)配置文件

## 28.自动配置原理?

1)、SpringBoot启动的时候加载主配置类，开启了自动配置功能 @EnableAutoConfiguration

2)、@EnableAutoConfiguration 作用:将 类路径下 META-INF/spring.factories 里面配置的所有EnableAutoConfiguration的值加入到了容器中;每一个这样的 xxxAutoConfiguration类都是容器中的一个组件，都加入到容器中;用他们来做自动配置;

3)、每一个自动配置类进行自动配置功能;根据当前不同的条件判断，决定这个配置类是否生效?

一但这个配置类生效;这个配置类就会给容器中添加各种组件;这些组件的属性是从对应的properties类中获取 的，这些类里面的每一个属性又是和配置文件绑定的;

4)、所有在配置文件中能配置的属性都是在xxxxProperties类中封装者‘;配置文件能配置什么就可以参照某个功 能对应的这个属性类怎么用好自动配置，精髓:
– 1)、SpringBoot启动会加载大量的自动配置类
– 2)、我们看我们需要的功能有没有SpringBoot默认写好的自动配置类;
– 3)、我们再来看这个自动配置类中到底配置了哪些组件;(只要我们要用的组件有，我们就不需要再来配置了)
– 4)、给容器中自动配置类添加组件的时候，会从properties类中获取某些属性。我们就可以在配置文件中指定这 些属性的值;