---
layout: post
title: MySQL InnoDB
slug: mysql-innodb
date: 2020-02-01 12:00
status: publish
author: 177
categories: 
  - 极客
tags:
  - MySQL
  - IT
excerpt: MySQL InnoDB-收集整理
---

[notice]从网络上收集整理的Java面试题，如有侵权，请联系删除！[/notice]

要创建 InnoDB 表，可以使用 CREATE TABLE 语句

CREATE TABLE t1 (a INT, b CHAR (20), PRIMARY KEY (a)) ENGINE=InnoDB;

如果你的 MySQL 实例将 InnoDB 定义为默认存储引擎 （ 默认情况下为默认存储引擎 )，则可以无需指定 ENGINE = InnoDB 子句。

如果你想要查询当前 MySQL 实例的默认存储引擎，可以使用下面的语句

```
mysql> SELECT @@default_storage_engine;
+--------------------------+
| @@default_storage_engine |
+--------------------------+
| InnoDB                   |
+--------------------------+
```

但是我们建议，无论任何时候，都使用 ENGINE = InnoDB 子句指定表的引擎为 InnoDB 。

因为如果没有指定，使用 mysqldump 或主从复制重放 CREATE TABLE 建表语句时，并不会自动添加 ENGINE = InnoDB。在默认存储引擎不是 InnoDB 的服务器上，表的存储引擎可能就不是 InnoDB 了。

每个表及其索引，可以存储在系统表空间 ( idbdata1 ) ，也可以存储在每个表自己的表空间 ( file-per-table ) ，还可以存储在通用的表空间 ( general tablespace )。

    当在 my.cnf 配置文件中启用 innodb_file_per_table（默认值）时，每个表及其索引会存储在每个表自己的独有表空间 ( file-per-table )
    相反，当禁用 innodb_file_per_table 配置项时，InnoDB 会在系统表空间中隐式创建 InnoDB 表。
    如果要在通用的空间内创建 InnoDB 表，则必须使用 CREATE TABLE ... TABLESPACE 语句。

当你启用 innodb_file_per_table 在每个表独自的文件表空间中创建一个表时，默认情况下，MySQL 会在 MySQL 服务器目录下的数据库目录中创建一个 .ibd 表空间文件。在 InnoDB 系统表空间中创建的表会存储在现有的 ibdata 文件中，该文件位于 MySQL 数据目录中。在通用表空间中创建的表会存储在现有的通用表空间 .ibd 文件中。可以在 MySQL 数据目录的内部或外部创建常规表空间文件。

在 InnoDB 内部，InnoDB 会将每个表的条目添加到数据字典中，该条目包含了数据库的名称。例如，如果在 test 数据库中创建了表 t1，则数据字典条目中的数据库名称为 test/t1 。 这意味着我们可以在不同的数据库中创建一个具有相同名称 t1 的表，并且表名不会在 InnoDB 中发生冲突。
InnoDB 表和行格式

InnoDB 表的默认行格式由 innodb_default_row_format 配置选项指定，可选的值有 DYNAMIC 、 COMPACT 和 REDUNDANT ，默认值为DYNAMIC。

dynamic 和 compact 行格式允许我们充分利用 InnoDB 的功能，例如表压缩和长列值的高效页外存储。要使用这些行格式，必须启用innodb_file_per_table ( 默认值 )

```
SET GLOBAL innodb_file_per_table=1;
CREATE TABLE t3 (a INT, b CHAR (20), PRIMARY KEY (a)) ROW_FORMAT=DYNAMIC;
CREATE TABLE t4 (a INT, b CHAR (20), PRIMARY KEY (a)) ROW_FORMAT=COMPRESSED;
```

或者，我们还可以使用 CREATE TABLE ... TABLESPACE 语句在通用表空间中创建 InnoDB 表。通用表空间支持所有行格式。

```
CREATE TABLE t1 (c1 INT PRIMARY KEY) TABLESPACE ts1 ROW_FORMAT=DYNAMIC;
```

CREATE TABLE ... TABLESPACE 语句也可用在系统表空间中创建具有 DYNAMIC 行格式的 InnoDB 表，以及具有 Compact 或 Redundant 行格式的表。

```
CREATE TABLE t1 (c1 INT PRIMARY KEY) TABLESPACE = innodb_system ROW_FORMAT=DYNAMIC;
```

InnoDB 表和主键 ( primary key )

应该始终为 InnoDB 表指定一个主键，且定义为主键的列应该符合以下要求：

1、 被最重要的查询引用
2、 永远不会留空 ( NULL )
3、 永远不会有重复值
4、 很少变更，一旦插入就永保不变

例如，在包含有关人员信息的表中，你不应该在 ( firstname，lastname ) 上创建主键，因为多个人可以使用相同的名称，而且有些人的姓氏是可空的，更何况，有些人会变更他们的姓名。

因为如此多的约束的存在，通常没有一组明显的列用作主键，因此你应该创建一个带有数字 id 的新列作为主键的全部或部分。为此，你可以声明一个带有 AUTO_INCREMENT 约束的列，以便在插入行时自动填充升序值。

```
# 主键 ID 可以作为不同表之间的关联指针
CREATE TABLE t5 (id INT AUTO_INCREMENT, b CHAR (20), PRIMARY KEY (id));

# 主键可以包含多列，但必须以一个自增列开始
CREATE TABLE t6 (id INT AUTO_INCREMENT, a INT, b CHAR (20), PRIMARY KEY (id,a));
```

尽管该表在没有定义主键的情况下可以正常工作，但主键涉及性能的许多方面，并且对于任何大型或经常使用的表都是关键的存在。因此，始终建议你在 CREATE TABLE 语句中指定主键。如果你先创建表，加载数据，然后运行 ALTER TABLE 以稍后添加主键，那么该操作比创建表时定义主键要慢得多
查看 InnoDB 表属性

要查看 InnoDB 表的属性，可以运行 SHOW TABLE STATUS 语句

```
mysql> SHOW TABLE STATUS FROM test LIKE 't%' \G;
*************************** 1. row ***************************
           Name: t1
         Engine: InnoDB
        Version: 10
     Row_format: Compact
           Rows: 0
 Avg_row_length: 0
    Data_length: 16384
Max_data_length: 0
   Index_length: 0
      Data_free: 0
 Auto_increment: NULL
    Create_time: 2015-03-16 15:13:31
    Update_time: NULL
     Check_time: NULL
      Collation: utf8mb4_0900_ai_ci
       Checksum: NULL
 Create_options:
        Comment:
```

也可以使用 InnoDB 信息模式系统表查询 InnoDB 表属性

```
mysql> SELECT * FROM INFORMATION_SCHEMA.INNODB_TABLES WHERE NAME='test/t1' \G
*************************** 1. row ***************************
     TABLE_ID: 45
         NAME: test/t1
         FLAG: 1
       N_COLS: 5
        SPACE: 35
   ROW_FORMAT: Compact
ZIP_PAGE_SIZE: 0
   SPACE_TYPE: Single
```