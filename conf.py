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
# site_logo = "${static_prefix}logo.png"
site_logo = "${static_prefix}favicon.ico"
site_build_date = "2020-01-01T00:00+08:00"
author = "177"
email = "0@177.im"
author_homepage = "https://177.im"
description = "天青色等烟雨"
key_words = ['177.im', '177', '听风行', 'blog', '博客']
language = 'zh-CN'
background_img = '${static_prefix}bg/The_Great_Wave_off_Kanagawa.jpg'
external_links = [
    {
        "name": "听风行博客",
        "url": "https://blog.iyu.pub",
        "brief": "🏄‍ 虽千万人吾往矣"
    },
    {
        "name": "听风行官网",
        "url": "https://www.iyu.pub",
        "brief": "听风行的主页"
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
        "name": "友链",
        "url": "${site_prefix}links/",
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
        "url": "https://twitter.com/SerendipityNiko",
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
<link rel="dns-prefetch" href="//blog.177.im" />
<link rel="stylesheet" href="${static_prefix}brand_font/embed.css" />
<style>.brand{font-family:FZCuJinLFW,serif;font-weight: normal!important;}</style>
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-status-bar-style" content="black">
<link rel="apple-touch-icon" sizes="180x180" href="${static_prefix}favicon.ico">
<link rel="icon" type="image/png" sizes="32x32" href="${static_prefix}favicon.ico">
<link rel="icon" type="image/png" sizes="16x16" href="${static_prefix}favicon.ico">
<link rel="mask-icon" href="${static_prefix}favicon.ico" color="#505050">
<link rel="shortcut icon" href="${static_prefix}favicon.ico">
<meta name="application-name" content="177">
<meta name="apple-mobile-web-app-title" content="177">
<meta name="msapplication-TileColor" content="#000000">
<meta name="theme-color" content="#000000">
<meta name="baidu-site-verification" content="" />
'''

footer_addon = r'''
Whatever will be, Will be.
'''

body_addon = r'''
<script>
if(window.location.hash){
    var checkExist = setInterval(function() {
       if ($(window.location.hash).length) {
          $('html, body').animate({scrollTop: $(window.location.hash).offset().top-90}, 1000);
          clearInterval(checkExist);
       }
    }, 100);
}
</script>
<script>
if(window.navigator && navigator.serviceWorker) {
  caches.keys().then(function(cacheNames) {
    cacheNames.forEach(function(cacheName) {
      caches.delete(cacheName);
    });
  }).then(function(){
    console.log('Cache cleaned.');
  });
  navigator.serviceWorker.getRegistrations()
  .then(function(registrations) {
    for(let registration of registrations) {
      registration.unregister();
    }
  }).then(function(){
    console.log('Service Worker stopped.');
  });
}
</script>
'''
