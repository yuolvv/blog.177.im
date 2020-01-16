---
layout: post
title: MySQL InnoDB Full Inverted INDEX
slug: mysql-innodb-full-inverted-index
date: 2020-02-04 12:00
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

在基于文本类型的列 ( char 、varchar 和 text ) 上创建全文 ( FULLTEXT ) 索引，并忽略任何定义为停用词的单词, 可以加快对这些列中包含的数据的查询和 DML 操作。

可以在使用 CREATE TABLE 语句创建表时指定全文索引，也可以在创建表之后使用 ALTER TABLE 或 CREATE INDEX 将全文索引添加到现有表中

全文索引的使用方式一般为使用 MATCH()... AGAINST 语法执行全文搜索。

## InnoDB 全文索引设计

InnoDB 全文索引采用了 「 倒排 」 索引的设计方式。倒排索引存储单词列表，并为每个单词存储单词出现的文档列表。为了支持邻近搜索，每隔单词的位置信息 ( 字节偏移 ) 也会被同时存储。

## InnoDB 全文索引表

创建 InnoDB 全文索引时，会同时创建一组索引表，如以下示例所示

```
mysql> CREATE TABLE opening_lines (
       id INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY,
       opening_line TEXT(500),
       author VARCHAR(200),
       title VARCHAR(200),
       FULLTEXT idx (opening_line)
       ) ENGINE=InnoDB;

mysql> SELECT table_id, name, space from INFORMATION_SCHEMA.INNODB_TABLES
       WHERE name LIKE 'test/%';
+----------+----------------------------------------------------+-------+
| table_id | name                                               | space |
+----------+----------------------------------------------------+-------+
|      333 | test/fts_0000000000000147_00000000000001c9_index_1 |   289 |
|      334 | test/fts_0000000000000147_00000000000001c9_index_2 |   290 |
|      335 | test/fts_0000000000000147_00000000000001c9_index_3 |   291 |
|      336 | test/fts_0000000000000147_00000000000001c9_index_4 |   292 |
|      337 | test/fts_0000000000000147_00000000000001c9_index_5 |   293 |
|      338 | test/fts_0000000000000147_00000000000001c9_index_6 |   294 |
|      330 | test/fts_0000000000000147_being_deleted            |   286 |
|      331 | test/fts_0000000000000147_being_deleted_cache      |   287 |
|      332 | test/fts_0000000000000147_config                   |   288 |
|      328 | test/fts_0000000000000147_deleted                  |   284 |
|      329 | test/fts_0000000000000147_deleted_cache            |   285 |
|      327 | test/opening_lines                                 |   283 |
+----------+----------------------------------------------------+-------+
```

前 6 个表 test/fts_*_index_* 表表示倒排索引表，也称之为 「 辅助索引表 」。

辅助索引表名称以 fts_ 为前缀，后缀为 index_ * 。每个索引表都通过索引表名称中与索引表的 table_id 匹配的十六进制值与数据表相互关联。

例如，test/opening_lines 表的 table_id 为 327，十六进制值为 0x147。如前面的示例所示，147十六进制值出现在与 test/ opening_lines 表关联的索引表的名称中

代表全文索引的 index_id 的十六进制值也出现在辅助索引表名称中。例如在辅助索引表 test/ fts_0000000000000147_00000000000001c9_index_1 名称中，十六进制值 1c9 的十进制值为 457 。可以通过在INFORMATION_SCHEMA.INNODB_INDEXES 表中查询此值来识别在 opening_lines 表 ( idx ) 上定义的索引 ( 457 )

```
mysql> SELECT index_id, name, table_id, space from INFORMATION_SCHEMA.INNODB_INDEXES
       WHERE index_id=457;
+----------+------+----------+-------+
| index_id | name | table_id | space |
+----------+------+----------+-------+
|      457 | idx  |      327 |   283 |
+----------+------+----------+-------+
```

如果主表是是在自己的文件表空间 ( file-per-table) 中创建的，那么索引表也会保存在这个自己的表空间中。

前面示例中显示的其它索引表称为公共辅助表，用于删除处理和存储全文索引的内部状态。与为每个全文索引创建的倒排索引表不同，这组表对于在特定表上创建的所有全文索引是通用的。

即使删除了全文索引，也会保留公共辅助表。删除某个全文索引时，将会保留为该索引创建的 FTS_DOC_ID 列。如果想要删除 FTS_DOC_ID 列，则需要重建表

公共的辅助表是用来管理 FTS_DOC_ID 列的

    (1)fts_*_deleted 和 fts_*_deleted_cache

    包含已删除但尚未从全文索引中删除其数据的文档的ID ( DOC_ID )

    fts_*_deleted_cache 是 fts_*_deleted 表的内存版本

    (2)fts_*_being_deleted 和 fts_*_being_deleted_cache

    包含已删除文档的文档 ID ( DOC_ID )，其数据当前正在从全文索引中删除

    fts_*_ being_deleted_cache 表是 fts_*_being_deleted 表的内存版本

    (3)fts_*_config

    存储有关全文索引的内部状态信息。最重要的是，它存储 FTS_SYNCED_DOC_ID ，用于标识已解析并刷新到磁盘的文档。

    从崩溃中恢复的时，FTS_SYNCED_DOC_ID 值用于标识尚未刷新到磁盘的文档，以便可以重新解析文档并将其添加回全文索引缓存

    要查看此表中的数据，请查询 INFORMATION_SCHEMA.INNODB_FT_CONFIG 表

当对传入的文档进行分词时。单个单词 ( 也称之为 Token ) 会和位置信息、关联的文档 ID ( DOC_ID ) 一起插入索引表中。

插入时会使用基于 「 字母顺序 」对 Token 的第一个单词进行完全排序，并将结果按排序权重分布到这 6 个索引表中。

倒排索引划分了 6 个辅助索引表以支持并行索引创建。默认情况下，会创建两个线程对单词和关联数据进行分词，排序和插入索引表。我们可以使用innodb_ft_sort_pll_degree 选项配置线程数。在大型表上创建全文索引时，可以考虑适当增加线程数。

注意：千万不要在 MyISAM 和 InnoDB 表的区别时说 「 InnoDB 不支持全文索引了 」 那已经是过去时了！

---

上面学习了 MySQL InnoDB 全文索引的相关的表。当插入文档时，会对其进行分词，也就是 Token 化，并将单个单词和相关数据插入到全文索引中。

## InnoDB 全文索引缓存

这个过程，即使要插入的文档非常小，也可能会导致在辅助索引表中进行大量的小插入，从而使这些表的并发访问成为性能的瓶颈。为了避免此问题，InnoDB 使用全文索引缓存 ( cache ) 来临时缓存最近要插入到辅助索引表中的行。

此内存缓存结构会一直持有插入的数据，直到缓存已满，然后批量将它们刷新到磁盘 ( 到辅助索引表 ) 。我们可以通过查询INFORMATION_SCHEMA.INNODB_FT_INDEX_CACHE 表以浏览最近插入的行的 Token 化的数据。

缓存和批处理刷新行为避免了对辅助索引表的频繁更新，这种频繁更新可能导致在繁忙的插入和更新时间期间出现并发访问问题。批处理机制还避免了对同一个单词的多次插入，并最大限度地减少了重复条目。这种机制不是单独刷新每个单词，而是将同一个单词的插入合并并作为单个条目刷新到磁盘，从而提高插入效率，同时保持辅助索引表尽可能小。

我们可以通过 innodb_ft_cache_size 配置项设置全文索引缓存大小 ( 基于每个表 )，这会影响全文索引缓存的刷新频率。 我们还可以使用innodb_ft_total_cache_size 配置选项为给定 MySQL 实例中的所有表定义一个全局全文索引缓存大小限制。

全文索引缓存存储与辅助索引表相同的信息。但是，全文索引缓存仅缓存最近插入的行的 Token 化的数据。当使用全文索引检索数据时，已刷新到磁盘 ( 到全文辅助表 ) 的数据不会返回到或更新到全文索引缓存中。只会直接查询辅助索引表中的数据，并在返回之前将辅助索引表的结果与全文索引缓存的结果合并。

## InnoDB 全文索引文档 ID 和 FTS_DOC_ID 列

InnoDB 使用一个称为 文档 ID ( DOC_ID ) 的唯一文档标识符将全文索引中的单词映射到单词出现的文档记录。这个映射需要用到数据表中的 FTS_DOC_ID 列。如果未定义 FTS_DOC_ID 列，InnoDB 会在创建全文索引时自动添加一个隐式的 FTS_DOC_ID 列。

以下示例演示了此行为。

下面的建表语句并没有定义 FTS_DOC_ID 列

```
mysql> CREATE TABLE opening_lines (
       id INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY,
       opening_line TEXT(500),
       author VARCHAR(200),
       title VARCHAR(200)
       ) ENGINE=InnoDB;
```

当使用 CREATE FULLTEXT INDEX 语法在表上创建全文索引时，会返回一个警告，报告 InnoDB 正在重建表以添加 FTS_DOC_ID 列

```
mysql> CREATE FULLTEXT INDEX idx ON opening_lines(opening_line);
Query OK, 0 rows affected, 1 warning (0.19 sec)
Records: 0  Duplicates: 0  Warnings: 1

mysql> SHOW WARNINGS;
+---------+------+--------------------------------------------------+
| Level   | Code | Message                                          |
+---------+------+--------------------------------------------------+
| Warning |  124 | InnoDB rebuilding table to add column FTS_DOC_ID |
+---------+------+--------------------------------------------------+
```

同样的警告也会出现在使用 ALTER TABLE 将全文索引添加到没有 FTS_DOC_ID 列的表中。

但是，如果在 CREATE TABLE 时创建了全文索引但未指定 FTS_DOC_ID 列，则 InnoDB 会添加一个隐藏的 FTS_DOC_ID 列，而不会发出警告。

在 CREATE TABLE 创建表时就定义 FTS_DOC_ID 列比在已加载数据的表上创建全文索引有着更快的性能。

    如果在加载数据之前就在表上定义了 FTS_DOC_ID 列，那么创建全文索引时，就不用添加新列，也就不必重建该表及其索引
    如果你并不关心 CREATE FULLTEXT INDEX 性能，那么请忽略 FTS_DOC_ID 列以让 InnoDB 为我们自动创建。InnoDB 会自动创建一个隐藏的 FTS_DOC_ID 列，并在该列上创建一个唯一索引 FTS_DOC_ID_INDEX
    如果你想要创建自己的 FTS_DOC_ID 列，则必须将该列定义为 BIGINT UNSIGNED NOT NULL 并命名为 FTS_DOC_ID ( 全部大写 )

    如下例所示

```
    mysql> CREATE TABLE opening_lines (
        FTS_DOC_ID BIGINT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY,
        opening_line TEXT(500),
        author VARCHAR(200),
        title VARCHAR(200)
        ) ENGINE=InnoDB;
```

> 注意： 并不需要将 `FTS_DOC_ID` 列添加 `AUTO_INCREMENT` 约束，但添加了 `AUTO_INCREMENT` 约束可以使加载数据更容易

* 如果你选择自己定义 FTS_DOC_ID 列，则你需要负责管理列以避免空值 ( NULL ) 或重复值。 FTS_DOC_ID 值无法重用，意味着 FTS_DOC_ID值必须不断增加

为了解决这个问题，你可以选择在 FTS\_DOC\_ID 列上创建必须的唯一索引 `FTS_DOC_ID_INDEX` ( 全部大写 )

```
    mysql> CREATE UNIQUE INDEX FTS_DOC_ID_INDEX on opening_lines(FTS_DOC_ID);
```

    如果你没有为 FTS_DOC_ID 创建 FTS_DOC_ID_INDEX，InnoDB 会自动创建它。

        注意： FTS_DOC_ID_INDEX 不能定义为降序索引，因为 InnoDB SQL 解析器不使用降序索引 

最大使用的 FTS_DOC_ID 值与新的 FTS_DOC_ID 值之间的允许间隔为 65535

为避免重建表，删除全文索引时将保留 FTS_DOC_ID 列。

---

InnoDB 全文索引中的那些处理逻辑，包括删除索引逻辑和事务逻辑。

## InnoDB 全文索引删除处理

删除具有全文索引列的记录可能会带来辅助索引表中的大量小删除，使得对这些表的并发访问成为性能的瓶颈。

为避免此问题，每当从索引表中删除记录时，已删除文档的文档 ID ( DOC_ID ) 将记录在特殊的 FTS_*_DELETED 表中，同时全文索引仍然会保留索引记录。全文查询返回结果前，会自动过滤掉 FTS_*_ DELETED 表中存储的已删除的文档 ID。

这种设计的好处是删除快速且廉价。缺点是删除记录后索引的大小不会立即减少。

要删除已删除记录的全文索引条目，需要设置 innodb_optimize_fulltext_only = ON 在索引表上运行 OPTIMIZE TABLE 以重建全文索引。

## InnoDB 全文检索事务处理

因为使用了缓存和批处理。InnoDB 全文索引的事务处理是非常特殊的。

具体来说，全文索引的更新和插入在事务提交时才真正处理，这意味着全文搜索只能看到已提交的数据。也就是不受事务处理四个级别的控制。

下面示例演示了这种事务处理方式。 全文搜索仅在提交插入的行后返回结果

```
mysql> CREATE TABLE opening_lines (
       id INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY,
       opening_line TEXT(500),
       author VARCHAR(200),
       title VARCHAR(200),
       FULLTEXT idx (opening_line)
       ) ENGINE=InnoDB;

mysql> BEGIN;

mysql> INSERT INTO opening_lines(opening_line,author,title) VALUES
       ('Call me Ishmael.','Herman Melville','Moby-Dick'),
       ('A screaming comes across the sky.','Thomas Pynchon','Gravity\'s Rainbow'),
       ('I am an invisible man.','Ralph Ellison','Invisible Man'),
       ('Where now? Who now? When now?','Samuel Beckett','The Unnamable'),
       ('It was love at first sight.','Joseph Heller','Catch-22'),
       ('All this happened, more or less.','Kurt Vonnegut','Slaughterhouse-Five'),
       ('Mrs. Dalloway said she would buy the flowers herself.','Virginia Woolf','Mrs. Dalloway'),
       ('It was a pleasure to burn.','Ray Bradbury','Fahrenheit 451');

mysql> SELECT COUNT(*) FROM opening_lines WHERE MATCH(opening_line) AGAINST('Ishmael');
+----------+
| COUNT(*) |
+----------+
|        0 |
+----------+

mysql> COMMIT;

mysql> SELECT COUNT(*) FROM opening_lines WHERE MATCH(opening_line) AGAINST('Ishmael');
+----------+
| COUNT(*) |
+----------+
|        1 |
+----------+
```

## MySQL InnoDB 全文索引监控

如果你想要监视和检查 InnoDB 全文索引的特殊文本处理方面，可以通过查询以下 INFORMATION_SCHEMA 表实现

    INNODB_FT_CONFIG
    INNODB_FT_INDEX_TABLE
    INNODB_FT_INDEX_CACHE
    INNODB_FT_DEFAULT_STOPWORD
    INNODB_FT_DELETED
    INNODB_FT_BEING_DELETED

你还可以通过查询 INNODB_INDEXES 和 INNODB_TABLES 表来查看全文索引和表的基本信息。