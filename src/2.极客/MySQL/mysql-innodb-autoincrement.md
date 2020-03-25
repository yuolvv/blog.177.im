---
layout: post
title: MySQL InnoDB AUTO_INCREMENT
slug: mysql-innodb-autoincrement
date: 2020-02-02 12:00
status: publish
author: 177
categories: 
  - 极客
tags:
  - MySQL
  - IT
excerpt: MySQL InnoDB-收集整理
---

[notice]从网络上收集整理的资料，如有侵权，请联系删除！[/notice]

想必你已经很熟悉 MySQL Innodb 中的 AUTO_INCREMENT，它是一个约束条件，如果某个字段添加了这个约束条件，插入数据的时候，如果没有给该字段指定一个值，那么它就会自动插入一个自增长的值。

## AUTO_INCREMENT 约束

AUTO_INCREMENT 是 Innodb 提供的一种可配置的锁定机制，如果某个表的某一列具有 AUTO_INCREMENT 约束，那么向该表添加数据的时候可以很明显的提高 SQL 语句的性能和可伸缩性。

为了充分使用 Innodb 表的 AUTO_INCREMENT 机制，必须 将 AUTO_INCREMENT 字段 ( 或列，下面提到的 「 列 」 和字段可以等价 ) 定义为 「 索引 」 的一部分，这样就可以在表上使用索引执行下面的语句来查找最大的列值

```
SELECT MAX (ai_col ) FROM tablename;
```

ai_col 为定义了 AUTO_INCREMENT 约束的列。

通常情况下，为了最大化性能，添加了 AUTO_INCREMENT 约束的列要么独自成一个索引 ( 主索引 )，那么是组合索引中的第一列

    需要注意的是： 虽然我们日常使用中会把 AUTO_INCREMENT 添加为主键，但它其实也可以不是主键的。甚至可以不是唯一索引。 

AUTO_INCREMENT 不仅仅是一个字段约束条件，它还是一个 「 锁 」，也就是那个很少见到的 「 AUTO_INCREMENT 锁」。

本章节接下来的部分，我们就来介绍介绍这个 「 AUTO_INCREMENT 锁」的模式和行为，包括不同的 「 AUTO_INCREMENT 锁」模式的使用意义，以及 Innodb 如何初始化 「 AUTO_INCREMENT 计数器 」

## Innodb AUTO_INCREMENT 锁的模式

使用了 AUTO_INCREMENT 那么多次，我们已经知道它的主要作用就是产生一个不重复的 「 自增值 」。

我们知道，插入多条数据有两种插入方法，一种是一条一条的执行 INSERT INTO，另一种是 INSERT INTO VALUES(...),(...) 多条一起插入

这两种插入方法都能正确的自增 AUTO_INCREMENT 列，它们是如何做的呢 ？

这就仰赖了 AUTO_INCREMENT 锁，为了适应这两种插入方法，它同时也具有多种模式。

### 术语

在我们继续讲解之前，为了方便大家理解一些术语或概念，我们先罗列在此

1、 「 insert like 」 语句

所有可以在表中添加新行的语句，我们称之为 「 insert like 」 语句，例如

```
1.  `INSERT, INSERT ... SELECT`
2.  `REPLACE`
3.  `REPLACE ... SELECT`
4.  `LOAD DATA`
5.  其它的还有 「 simple-inserts 」、「 bulk-inserts 」和 「 mixed-mode 」 三种插入语句
```

2、 「 simple-inserts 」 语句

「 simple-inserts 」 是可以预先确定要插入的行数的语句 ( 最初处理语句时 )。包括不带子查询的 **单行** 和 **多行** `INSERT` 和`REPLACE` 语句，但不包括 `INSERT ... ON DUPLICATE KEY UPDATE` 语句

3、 「 Bulk inserts 」 批量插入

「 Bulk inserts 」是预先不知道要插入的行数（以及所需的自动增量值的数量）的语句。

包括 `INSERT ... SELECT`，`REPLACE ... SELECT` 和 `LOAD DATA` 语句，但不包括普通的 `INSERT`

在处理每一行时，InnoDB 都会重新为 `AUTO_INCREMENT` 列分配一个新值

4、 「 Mixed-mode inserts 」 混合模式插入

「 Mixed-mode inserts 」 是指「 simple-inserts 」 语句中，有些指定了 `AUTO_INCREMENT` 列的值，而另一些则没有。

例如下面的 SQL 语句，其中 `c1` 是表 `t1` 的 `AUTO_INCREMENT` 列

    INSERT INTO t1 (c1,c2) VALUES (1,'a'), (NULL,'b'), (5,'c'), (NULL,'d');

另一种类型的 「 Mixed-mode inserts 」 是 `INSERT ... ON DUPLICATE KEY UPDATE` ，这种语句最坏的情况下实际上是 `INSERT` 后跟 `UPDATE`，其中在更新阶段，可能会也可能不会为 `AUTO_INCREMENT` 列的分配值

### innodb_autoinc_lock_mode

我们先不讨论有几种模式，我们先来看看它是如何配置的。

「 AUTO_INCREMENT 锁」模式的配置变量为 innodb_autoinc_lock_mode ，我们可以通过下面的语句查看当前的模式是什么

```
show variables like 'innodb_autoinc_lock_mode';
```

在我的 5.7.22 的版本的 MySQL 中，输出结果为

```
mysql> show variables like 'innodb_autoinc_lock_mode';
+--------------------------+-------+
| Variable_name            | Value |
+--------------------------+-------+
| innodb_autoinc_lock_mode | 1     |
+--------------------------+-------+
```

配置参数 innodb_autoinc_lock_mode 有三个可选的值，分别是 0、1 和 2 ，分别代表着 「 传统 」，「 连续 」 或 「 交错 」 三种锁模式

在不同的版本下，innodb_autoinc_lock_mode 的默认值是不一样的，在 mysql >= 8.0.3 版本中是 2，也就是 「 交错 」 模式，而 mysql <= 8.0.2 版本中是 1，也就是 「 连续 」 模式

对于 8.0.3 版本中的这种变更，也反应了 Innodb 的默认 「 复制模式 」 已经从基于 SQL 语句 变更为基于 行 ( row )

基于 SQL 语句的复制需要 「 连续 」 模式的 「 AUTO_INCREMENT 锁」，以确保为给定的 SQL 语句序列以可预测和可重复的顺序分配自动增量值，而基于行的复制对 SQL 语句的执行顺序不敏感

## innodb_autoinc_lock_mode = 0 传统锁模式

传统锁模式是在 MySQL 5.1 中引入 innodb_autoinc_lock_mode 配置参数之前的默认模式。现在，传统锁模式存在的意义，仅仅是用于向后兼容，性能测试以及解决 「 混合模式插入 」 问题，因为语义方面可能存在差异。

在这种锁模式下，为了向具有 AUTO_INCREMENT 列的表中插入数据，所有的 「 insert like 」 语句都会获得一个特殊的 表级 AUTO-INC 锁，这种锁会自动添加到 SQL 语句的末尾 ( 不是事务的末尾 )，以确保以可预测且可重复的顺序为给定的 INSERT 语句序列分配自增值，并确保为任何给定语句分配的自增值都是连续的。

在基于 SQL 语句的 ( 主从 ) 复制环境中，在从服务器上运行复制 SQL 语句时，自增量列的值和主服务器的值相同，这样执行多个 INSERT 语句的结果是确定性的，并且从服务器的数据和主服务器的数据一摸一样。

如果多个 INSERT 语句生成的自增值是交错的，那么两个并发 INSERT 语句的结果将是不确定的，这样就无法使用基于 SQL 语句的复制模式将数据可靠地复制到从服务器

讲解的有点拗口，我们看看一些示例，假设存在一张表 t1，它的建表语句如下

```
CREATE TABLE t1 (
  c1 INT(11) NOT NULL AUTO_INCREMENT,
  c2 VARCHAR(10) DEFAULT NULL,
  PRIMARY KEY (c1)
) ENGINE=InnoDB;
```

把表建完后，我们假设有两个事务在运行，两个事务都是往 t1 表中插入数据，第一个事务使用一个事务使用 INSERT ... SELECT 语句插入 1000 行的，另一个事务使用简单的 INSERT 语句插入一行数据

```
Tx1: INSERT INTO t1 (c2) SELECT 1000 rows from another table ...
Tx2: INSERT INTO t1 (c2) VALUES ('xxx');
```

第一个事务 Tx1 ，因为 InnoDB 无法预先判断在 INSERT 语句从 SELECT 中检索了多少行，所以它会在每插入一条数据的时候分配一个自增值。这种情况下，会使用 表锁 ，会在 SQL 语句的末尾自动添加一个表锁，一次只能在表 t1 执行一条 INSERT 语句，这样就能保证每条 INSERT 语句的自增值是连续的且不会交错。

这样由 Tx1 INSERT ... SELECT 语句生成的自增值是连续的，并且 Tx2 事务中 INSERT 语句使用的 ( 单个 ) 自增值要小于或大于 Tx1 的所有自增值，结果具体取决于哪个语句先执行

这时候在主从复制或数据恢复时，只要以二进制日志重放 SQL 语句时 ( 使用基于语句的复制时或恢复方案中）以相同的顺序执行，那么重放的结果与 Tx1 和 Tx2 首次运行时的结果相同

如果前面的示例没有使用 「 表锁 」 ，那么 Tx2 中 INSERT 的自增列的值取决于语句执行的时间。如果 Tx2 的 INSERT 在 Tx1 的 INSERT 运行时 ( 而不是在它开始之前或完成之后 ) 执行，则两个 INSERT 语句分配的特定自增值是不确定的，并且可能因运行而异。

在 「 连续锁 」模式下，InnoDB 可以避免将表级 AUTO-INC 锁用于 「 insert like 」 语句，因为行数已预先知道，而且还可以确保基于语句的复制的确定性执行和安全性

在恢复或复制数据的重放 SQL 语句时如果不使用二进制日志，那么可以使用 「 交错锁 」模式用来消除表级 AUTO-INC 锁的使用，以获得更高的并发性和性能，但代价语句分配的自增值数字可能不是连续的，而且可能因为并发的执行而存在重复的数字


## innodb_autoinc_lock_mode = 1 连续锁模式

在此 「 连续锁 」 模式下，「 批量插入 」会首先获取一个特殊的表级 AUTO_INC 锁，并一直保持到所有语句执行结束才释放。连续锁模式适用于所有的批量插入语句，包括 INSERT ... SELECT、REPLACE ... SELECT 和 LOAD DATA 语句。

Innodb 存储引擎一次只会执行一个持有 AUTO-INC 锁的 SQL 语句。

如果批量插入操作的源表与目标表不同，则会先在源表中选择的第一行上执行共享锁，然后对目标表执行 AUTO-INC 锁。如果批量插入操作的源表和目标表是同一个表，则会先对所有选定的行执行共享锁之后再执行 AUTO-INC 锁

简单插入 「 Simple inserts 」 ( 即预先知道要插入的行数 ） 则是不同的，它会在互斥锁 ( 一个轻量级的锁 ) 的控制下获得所需数量的自增量值来避免表级 AUTO-INC 锁定。但这种获得自增值的互斥锁只在分配过程的持续时间内持有，而不是在语句完成之前。除非另一个事务首先持有 AUTO-INC 锁，否则不使用表级 AUTO-INC 锁。如果另一个事务持有 AUTO-INC 锁，则 「 简单插入 」会一直等待直到自己获得 AUTO-INC 锁，就好像它自己也是批量插入一样

这种锁定模式可以确保，在存在 INSERT 语句情况下，纵使事先不知道行数 ( 以及在语句执行时分配的自增值数 )，任何由 「 INSERT-like 」 分配的所有自增值都是连续的，且确保对基于语句的复制操作是安全的。

简而言之，这种锁模式显着提高了可伸缩性，同时可以安全地进行基于语句的复制。此外，与 「 传统锁」 模式一样，可以确保为任何给定语句分配的自增值数字是连续的。

从某些方面说，与 「 传统锁 」 模式相比，对于任何使用自增值的语句，语义并没有变化，除了下面这个重要的例外

这个例外就是 「 混合模式插入 」 (mixed-mode inserts ) ， 在混合模式插入中，那些多行的 「 简单插入 」中的某些行 ( 但不是所有行 ) 会显式为 AUTO\_INCREMENT 列提供一个值。对与这种插入模式，InnoDB 会分配比要插入的行数更多的自增值。

但这样也存在一个问题 ( 可以被忽略的问题 )，因为自动分配的所有值都是连续生成，因此可能高于最后执行的语句生成的自增值，也就是超过的没用到的自增值则被丢失了。

## innodb_autoinc_lock_mode = 2 交错锁模式

在 「 交错锁 」模式下，任何 「 INSERT-like 」语句都不会使用表级 AUTO-INC 锁，且多个语句可以同时执行。

这是最快且可扩展性最强的锁模式，但在二进制日志重放 SQL 语句中，会让使用基于语句

的复制或恢复方案变得不安全。

这种锁模式，会确保自增值是唯一的，且可以在所有同时执行的 「 INSERT-like 」 语句中保持单调递增

当然了，这种锁模式也是有缺点的，因为多个语句可以同时生成数字 (即自增值数字的分配会在语句之间交错进行 ) ，会造成任何给定语句插入的行生成的值可能不是连续的

## 总结以上3中模式

好复杂的 AUTO-INC 锁模式，翻译的我都头痛了，虽然在翻译的时候知道是啥意思，但是自己写出来，真的是一头雾水。

简单理解这三种锁模式

1、 传统锁模式 – 不管三七二十一，先用表级的 AUTO-INC 锁，直到语句插入完成，然后释放锁
2、 连续锁模式 – 不管三七二十一，先用互斥锁，然后生成所有插入行需要的自增值，然后释放互斥锁，最后使用这些自增值来插入数据。对于行数未知的，那就只能使用 「 传统锁 」 模式了，先锁起来，执行完毕了再释放，因为人家不知道要生成多少自增值啊
3、 交错模式 – 管它刮风下雨，需要的时候再生成，也管它连续与否，用了就是了…

---

以上，介绍了 MySQL Innodb AUTO_INCREMENT 锁的三种模式，分别为 「 传统模式 」 、 「 连续模式 」 、「 交错模式 」 ，这三种模式我们可以用同学聚会定餐馆来形象的描述下

1、 「 传统模式 」 – 老板，你家的店我包了，等到我同学聚会完了你再招待其它客人啊….其它客人：坑啊！ 你们快点吃啊，慢吞吞的…

2、 「 连续模式 」 – 老爸，我同学总共 50 个人，先给我来 50 个位置，有些同学可能没来，那就空着。剩下的位置，你可以招待其它的同学。有时候，如果不知道有多少同学 ( 健忘症犯了… )，就只能再回到传统模式了。

3、 「 交错模式 」 – 老板，我也不知道会来几个同学，反正，来了一个就坐一个位置，其它客人来了也可以坐 ( 假设老板家座位足够 )

这三种锁模式的优缺点是啥 ？

1、 「 传统模式 」 能保证所有的自增值是连续的，且不会浪费 ( 也就是会一直 123456789…自增下去，不会断掉 )，但其它的事务或语句要等到当前语句执行完才会继续执行

2、 「 交错模式 」解决了要 「 传统模式 」中要等待的问题，但也会引入新问题，就是可能造成自增值出现断层，比如 12345 缺了 4 这样。同时，在指定插入数据条数不确定的情况下，会回到 「 传统模式 」

3、 「 交错模式 」 解决了自增值断层的问题，但引入了自增值顺序混乱的问题，可能会导致自增值如下 13245687

交错模式在日常的 SELECT 语句中是不会出啥问题的，因为会按照自增值排序，出问题就处恢复数据或主从过程中的二进制日志回放，可能导致从库或者恢复的数据的自增值和源数据不一致。

好了，MySQL Innodb AUTO_INCREMENT 锁的三种模式我们就介绍到此了，接下来的部分我们讨论讨论下这三种模式在 MySQL Innodb 中的使用说明

## MySQL Innodb AUTO_INCREMENT 锁的使用说明

### 主从复制中的 AUTO_INCREMENT

主从复制时，如果你使用的是基于 SQL 语句的复制，需要将 innodb_autoinc_lock_mode 配置项的值设置为 0 或 1 ，而且主服务器和从服务器上的值必须相同。

如果设置 innodb_autoinc_lock_mode = 2，也就是使用 「 交错锁 」 模式，或者主从服务器上的值不相同，则不能确保从服务器上相同行的自增值和主服务器相同

如果你使用的是基于 行 ( row ) 或混合模式的主从复制，那么使用任何一个 AUTO_INCREMENT 锁模式都是安全的，因为基于行的复制对 SQL 语句的执行顺序不敏感。

在 交错锁 模式下，主从复制来说，任何基于语句的复制都是不安全的

    注意： 混合模式使用的是基于行的复制方式。 

### 「 丢失 」 ( lost ) 自增值和序列间隙

无论你使用的是哪种锁模式 ( 0 , 1 或 2 )，如果生成自增值的事务回滚，则这些自增量值将 「 丢失 」。

一旦为 AUTO_INCREMENT 列生成一个自增值后，无论 「 INSERT-like 」 语句是否完成，以及包含事务是否回滚，都无法回滚该自增值。

这些丢失的自增值不会被重复使用。因此，表的 AUTO_INCREMENT 列中的值可能存在间隙

### 为 AUTO_INCREMENT 列指定 NULL 或 0 时候

无论使用哪种 AUTO_INCREMENT 锁模式 ( 0, 1 和 2 )，如果用户在INSERT 语句中为 AUTO_INCREMENT 列指定的值为 NULL 或 0，那么 Innodb 将会忽略这些值并为该列生成一个新的自增值。

也就是说，不管是否为 AUTO_INCREMENT 列指定了值为 NULL ，还是指定了值为 0 ，或是未指定，Innodb 都会自动生成一个自增值作为该列的值。

### 为 AUTO_INCREMENT 列指定一个负值 ( < 0 )

无论使用哪种 AUTO_INCREMENT 锁模式 ( 0, 1 和 2 )，如果用户在INSERT 语句中为 AUTO_INCREMENT 列指定的值为负数，那么 Innodb 为 AUTO_INCREMENT 生成的值就是不确定的 ( is not defined )

也就是说，如果指定了一个负数，那么当前插入行的 AUTO_INCREMENT 列的值就是不确定的 ( is not defined )

### 如果 AUTO_INCREMENT 列的值大于指定列数据类型 ( 一般是整数类型 ) 的最大整数

无论使用哪种 AUTO_INCREMENT 锁模式 ( 0, 1 和 2 )，如果 AUTO_INCREMENT 列的值变得大于可以存储在指定整数类型中的最大整数，则最终的结果也是不确定的 ( is not defined )

### 「 批量插入 」 中的自增值的间隙

在 innodb_autoinc_lock_mode 配置项的值设置为 0 ( 传统锁模式 ) 或 1 ( 连续锁模式 ) 时，任何给定语句生成的自增值都是连续的，没有间隙。

因为在这两个模式下，「 批量插入 」 会使用表级别的 AUTO-INC 锁，且会一直持有直到所有语句运行结束。在表级别的 AUTO-INC 锁控制下，一次只能执行一个这样的语句

在 innodb_autoinc_lock_mode 配置项的值设置为 2 ( 交错锁模式 ） 时，「 批量插入 」 生成的自增值可能存在间隙，仅有的出现前提是同时执行 「 INSERT-like 」 语句

对于锁模式 1 和 2 ，连续的语句间也可能出现间隙，因为批量插入，可能并不知道每个语句所需的确切数量的自增值，可能会存在高估，如果一旦高估了，那么高估的自增值将会被抛弃且永远也不会使用到。

## 「 混合模式插入 」 中的自增值

「 混合模式插入 」 中，那些 「 简单插入 」中的某些语句 ( 但不是全部 ) 可能显式的为 AUTO_INCREMENT 列指定了值。

如果真的发生了这种情况，三种锁模式下的结果是各不相同的。

例如，假设 c1 列是表 t1 的 AUTO_INCREMENT 列，且假设最新自动生成的序列号为 100

```
CREATE TABLE t1 (
    c1 INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    c2 CHAR(1)
) ENGINE = INNODB;
```

现在，我们来看看下面这条混合模式插入语句

```
INSERT INTO t1 (c1,c2) VALUES (1,'a'), (NULL,'b'), (5,'c'), (NULL,'d');
```

在 innodb_autoinc_lock_mode 设置为 0 ( 传统锁模式下 )，刚刚插入的 4 条数据如下

```
mysql> SELECT c1, c2 FROM t1 ORDER BY c2;
+-----+------+
| c1  | c2   |
+-----+------+
|   1 | a    |
| 101 | b    |
|   5 | c    |
| 102 | d    |
+-----+------+
```

下一个可用的自增值为 103，因为自增值一次分配一个，而不是在语句执行开始时一次性分配。这时候无论是否同时并发的执行其它 「 INSERT-like 」 语句 ( 任何类型 )，此结果都是正确的

但如果设置 innodb_autoinc_lock_mode 的值为 1 ，也就是 「 连续锁模式 」，那么刚刚插入的 4 条数据如下

```
mysql> SELECT c1, c2 FROM t1 ORDER BY c2;
+-----+------+
| c1  | c2   |
+-----+------+
|   1 | a    |
| 101 | b    |
|   5 | c    |
| 102 | d    |
+-----+------+
```

虽然最终数据看起来一样。但是，行为结果还是不同的。

在这种情况下，下一个可用的自增值是 105，而不是 103，因为在处理语句时分配了四个自增值，但只使用了两个，也就是 103 和 104 永远的被丢失了。这时候无论是否同时并发的执行其它 「 INSERT-like 」 语句 ( 任何类型 )，此结果都是正确的

但如果设置 innodb_autoinc_lock_mode 的值为 2 ，也就是 「 交错锁模式 」，那么刚刚插入的 4 条数据如下

```
mysql> SELECT c1, c2 FROM t1 ORDER BY c2;
+-----+------+
| c1  | c2   |
+-----+------+
|   1 | a    |
|   x | b    |
|   5 | c    |
|   y | d    |
+-----+------+
```

x 和 y 的值是唯一的，并且比任何先前生成的行的都大。但是，x 和 y 的具体值取决于同时执行语句生成的自增值的数量。如果没有其它 「 INSERT-like 」语句并发执行，结果和 「 传统锁 」 模式是一样的。

最后，我们来看看下面这个 SQL 语句，假设最新自动生成的序列号为 100

```
INSERT INTO t1 (c1,c2) VALUES (1,'a'), (NULL,'b'), (101,'c'), (NULL,'d');
```

无论配置项 innodb_autoinc_lock_mode 的值设置为何种锁模式，该语句会抛出生成重复键错误 23000 ( Can’t write; duplicate key in table ) 。因为自增值 101 已经分配给了 (NULL,'b') ，这时候 (101,'c') 再使用 101 就会失败，当然了，前提是 AUTO_INCREMENT 列设置为主键或唯一索引

### 在一序列 INSERT 语句的中间修改 AUTO_INCREMENT 列值

在 MySQL 5.7 及更早版本中，在一序列 INSERT 语句的中间修改 AUTO_INCREMENT 列值可能会导致 「 重复条目 」 ( Duplicate entry ) 错误。例如，如果执行了将 AUTO_INCREMENT 列值更改为大于当前最大自增值的 UPDATE 操作，那些后续的未指定也未使用自增值的 INSERT 操作可能会遇到 「 重复条目 」 错误

在MySQL 8.0 及更高版本中，如果将 AUTO_INCREMENT 列值修改为大于当前最大自增值的值，那么新值将保持不变，后续 INSERT 操作将从新的较大值开始分配自增值

下面的示例演示了这种行为
```
mysql> CREATE TABLE t1 (
    -> c1 INT NOT NULL AUTO_INCREMENT,
    -> PRIMARY KEY (c1)
    ->  ) ENGINE = InnoDB;

mysql> INSERT INTO t1 VALUES(0), (0), (3);

mysql> SELECT c1 FROM t1;
+----+
| c1 |
+----+
|  1 |
|  2 |
|  3 |
+----+

mysql> UPDATE t1 SET c1 = 4 WHERE c1 = 1;

mysql> SELECT c1 FROM t1;
+----+
| c1 |
+----+
|  2 |
|  3 |
|  4 |
+----+

mysql> INSERT INTO t1 VALUES(0);

mysql> SELECT c1 FROM t1;
+----+
| c1 |
+----+
|  2 |
|  3 |
|  4 |
|  5 |
+----+

```

---

了解了 MySQL Innodb 中的 AUTO_INCREMENT 约束和 AUTO_INCREMENT 锁的模式。也了解了各种模式的影响和缺点。

当然了，上面的章节，很多人应该是不关心的。大家最关心的还是本章节的内容：「 MySQL InnoDB AUTO_INCREMENT 计数器如何初始化 」

## MySQL InnoDB AUTO_INCREMENT 计数器如何初始化

如果为 InnoDB 表指定了 AUTO_INCREMENT 列，那么内存中，该表对象会包含一个称为 「 自增值计数器 」 的特殊计数器，用于为 AUTO_INCREMENT 列分配新值时使用。

在 MySQL 5.7 及更早版本中，自增值计数器仅存储在内存中，而不是磁盘上。

为了在 MySQL 启动或重新启动后初始化 自增值计数器，在往含有 AUTO_INCREMENT 列的 InnoDB 表中第一次插入数据之前，会执行以下语句获取当前的 自增值

```
SELECT MAX(ai_col) FROM table_name FOR UPDATE;
```

    这个操作是隐式执行的，就是在插入时，如果 Innodb 发现内存中该表对象不包含自增计数器的时候，会执行上面的 SQL 语句来获取这个自增值 

在 MySQL 8.0 中，此行为已经变更了。每次更改时，会将当前自增值计数器的最大值将写入重做 ( redo ) 日志，并保存到每个检查点上的引擎专用系统表中。这种变更使得自增值计数器的当前最大值会在重启时保持不变。

    在原来的模式中，重启前，可能内存中的自增值计数器已经到了 1000+ ，但表中的实际最大值可能只有 100，那么重启后，自增值就会停留在 100，而 8.0 的变更中，最大值仍会保持重启前的 1000+ 

当正常关闭 MySQL 服务器后再重新启动它时，InnoDB 会使用存储在数据字典系统表中的当前最大自增值来初始化内存中的自增值计数器。

如果服务器发生了非正常崩溃，在崩溃恢复期间重新启动服务器时，InnoDB 仍会使用存储在数据字典系统表中的当前最大自增值初始化内存中自增值计数器，并且会扫描重做日志 ( redo log ) 以查找自上一个检查点以来写入的自增计数器的值。如果重做日志值大于内存中计数器值，则会应用重做日志值。

因此，在服务器崩溃的情况下，无法保证重用先前分配的自增值

当每一次执行 INSERT 或 UPDATE 操作会更改当前最大自增值时，会将新值将写入重做日志。但如果在将重做日志刷新到磁盘之前发生崩溃，那么在重新启动服务器后初始化自增计数器时可能会重用以前分配的值。

当然了，MySQL 8.0 或更高的版本中也可能会使用等效下面的 SQL 语句来初始化自增计数器

```
SELECT MAX(ai_col) FROM table_name FOR UPDATE;
```

但唯一可能发生的情况是: **导入没有 .cfg 元数据文件的表空间。 否则，会从 .cfg 元数据文件中读取当前最大自增计数器值

在 MySQL 5.7 及更早版本中，服务器重新启动时会忽略执行表选项中的 AUTO_INCREMENT = N，该选项一般用来在 CREATE TABLE 或 ALTER TABLE 语句中用于设置初始计数器值或更改现有计数器值。

但在 MySQL 8.0 及更高的版本中，服务器重新启动时并不会忽略表选项 AUTO_INCREMENT = N。 如果将自增计数器初始化为特定值，或者将自增计数器值更改为更大的值，则新值会在服务器重新启动时保持不变。

    注意

    ALTER TABLE ... AUTO_INCREMENT = N 语句仅仅只能用于将自增计数器的值修改为大于当前计数器的最大值，如果小于则是没有任何效果的。 

在 MySQL 5.7 及更早版本中，服务器在 ROLLBACK 操作之后立即重新启动可能会导致重用先前分配给回滚事务的自增值，从而可以有效地回滚当前最大自增值。

但在 MySQL 8.0 中，当前的最大自增量值是持久的，从而阻止了重用以前分配的值。

如果使用 SHOW TABLE STATUS 语句在初始化自增计数器之前检查表，InnoDB 将打开表并使用存储在数据字典系统表中的当前最大自增值初始化计数器值。并将该值存储在内存中以供以后插入或更新使用。

启动或重新启动服务器时，初始化自增计数器会使用表上的常规独占锁 ( for update ) 读取，并且会持有该独占锁直到事务结束。在初始化新创建的表的自增计数器时，InnoDB 遵循相同的过程。当然了，新创建的表可以使用 AUTO_INCREMENT = N 选项指定一个大于 0 的自增值

初始化自增计数器完成后，如果在插入行时未显式指定自增值，InnoDB 会隐式递增计数器并将新值分配给 AUTO_INCREMENT 列

只要服务器一直在运行，InnoDB 就会使用内存中的自增计数器。当服务器停止并重新启动时，InnoDB 会重新初始化自增计数器，如前所述。
my.cnf 配置文件中的 AUTO_INCREMENT

    在 my.cnf 配置文件中，可以使用 auto_increment_offset 配置选项确定 AUTO_INCREMENT 列值的起始点。默认设置为 1
    在 my_cnf 配置文件中，可以使用 auto_increment_increment 配置选项控制连续列值之间的间隔。默认设置为 1

一般情况下，在双主互相备份时，我们一般会指定一台服务器的两个配置项为

```
auto_increment_offset=1;
auto_increment_increment=2;
```

这样，这台服务器的自增值将会遵循 1 3 5 7 9 11 13 ，奇数数列

而在另一台服务器上指定

```
auto_increment_offset=2;
auto_increment_increment=2;
```

这样，这台服务器的自增值将会是 2 4 6 8 10 …. 偶数数列

这样，在互为主从的时候，就不会出现自增值重复的问题