# -*- coding: utf-8 -*-
"""博客构建配置文件
"""

# For Maverick
site_prefix = "/"
source_dir = "../src/"
build_dir = "../dist/"
index_page_size = 10
archives_page_size = 20
enable_jsdelivr = {
    "enabled": True,
    "repo": "yuolvv/blog.177.im@gh-pages"
}

# 站点设置
site_name = "177博客"
site_logo = "${static_prefix}logo.png"
site_build_date = "2019-12-29T12:00+08:00"
author = "177"
email = "0@177.im"
author_homepage = "https://177.im"
description = "天青色等烟雨"
key_words = ['177.im', '177', '听风行', 'blog', '博客']
language = 'zh-CN'
external_links = [
    {
        "name": "听风行博客",
        "url": "https://blog.iyu.pub",
        "brief": "🏄‍ 虽千万人吾往矣"
    },
    {
        "name": "听风行官网",
        "url": "https://www.iyu.pub",
        "brief": "听风行的主页。"
    }
]
nav = [
    {
        "name": "首页",
        "url": "${site_prefix}",
        "target": "_self"
    },
    {
        "name": "归档",
        "url": "${site_prefix}archives/",
        "target": "_self"
    },
    {
        "name": "关于",
        "url": "${site_prefix}about/",
        "target": "_self"
    }
]

social_links = [
    {
        "name": "Twitter",
        "url": "https://twitter.com/tingfengxing",
        "icon": "gi gi-twitter"
    },
    {
        "name": "GitHub",
        "url": "https://github.com/yuolvv",
        "icon": "gi gi-github"
    },
    {
        "name": "Weibo",
        "url": "https://weibo.com/test2020/",
        "icon": "gi gi-weibo"
    }
]

head_addon = r'''
<meta http-equiv="x-dns-prefetch-control" content="on">
<link rel="dns-prefetch" href="//cdn.jsdelivr.net" />
'''

footer_addon = ''

body_addon = ''
