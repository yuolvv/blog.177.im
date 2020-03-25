---
layout: post
title: 有趣的Docker镜像
slug: fun-docker-images
date: 2020-01-01 18:00
status: publish
author: 177
categories: 
  - 极客
tags:
  - Docker
  - IT
excerpt: 收集的一些实用有趣的 Docker 镜像
---

[notice] docker一键安装脚本 ：wget -qO- get.docker.com | sh [/notice]

## rastasheep/ubuntu-sshd （带ssh的ubuntu）

地址：[https://hub.docker.com/r/rastasheep/ubuntu-sshd/](https://hub.docker.com/r/rastasheep/ubuntu-sshd/)

sudo docker run -d -p 22:22 rastasheep/ubuntu-sshd:16.04

用户名、密码为root

## itscaro/debian-ssh （带ssh的debian） 

地址：[https://hub.docker.com/r/itscaro/debian-ssh/](https://hub.docker.com/r/itscaro/debian-ssh/)

sudo docker run -d -p 22:22  itscaro/debian-ssh

用户名、密码为root

## tutum/centos （带ssh的centos） 

地址：[https://hub.docker.com/r/tutum/centos/](https://hub.docker.com/r/tutum/centos/)

 sudo docker run -d -p 22:22  tutum/centos

（centos7：tutum/centos:centos7 ）

用户名root，密码随机，请执行 docker logs <CONTAINER_ID> 查看 

## alexwhen/docker-2048（游戏2048）

地址：[https://hub.docker.com/r/alexwhen/docker-2048/](https://hub.docker.com/r/alexwhen/docker-2048/)

sudo docker run -d -p 80:80 alexwhen/docker-2048

## dorowu/ubuntu-desktop-lxde-vnc（noVNC、Firefox51）

地址：[https://hub.docker.com/r/dorowu/ubuntu-desktop-lxde-vnc/](https://hub.docker.com/r/dorowu/ubuntu-desktop-lxde-vnc/)

docker run -it -p 80:80 dorowu/ubuntu-desktop-lxde-vnc

Browse http://localhost/

## consol/centos-xfce-vnc （VNC、noVNC、密码、chrome、Firefox45）

地址：[https://hub.docker.com/r/dorowu/ubuntu-desktop-lxde-vnc/](https://hub.docker.com/r/dorowu/ubuntu-desktop-lxde-vnc/)

run -it -p 5901:5901 -p 6901:6901 -e "VNC_PW=my-new-password" -e VNC_RESOLUTION=800x600 consol/centos-xfce-vnc

默认VNC密码：vncpassword

VNC-Server (default VNC port 5901)

noVNC - HTML5 VNC client (default http port 6901)

其他相关

consol/centos-xfce-vnc: Centos7 with Xfce4 UI session

consol/ubuntu-xfce-vnc: Ubuntu with Xfce4 UI session

consol/centos-icewm-vnc: dev     Centos7 with IceWM UI session

consol/ubuntu-icewm-vnc: dev      Ubuntu with IceWM UI session 

## fish/peerflix-server （支持磁力，种子）

地址：[https://hub.docker.com/r/dorowu/ubuntu-desktop-lxde-vnc/](https://hub.docker.com/r/dorowu/ubuntu-desktop-lxde-vnc/)

docker run -it  -p 9000:9000 fish/peerflix-server

Browse http://localhost:9000/

## jpillora/cloud-torrent（种子下载，搜索）

地址：[https://hub.docker.com/r/jpillora/cloud-torrent/](https://hub.docker.com/r/jpillora/cloud-torrent/)

docker run -d -p 3000:3000 -v /path/to/my/downloads:/downloads jpillora/cloud-torrent

Browse http://localhost:3000/

 ## forsaken-mail（临时邮箱）

地址：[https://hub.docker.com/r/rockmaity/forsaken-mail/](https://hub.docker.com/r/rockmaity/forsaken-mail/)

docker run --name forsaken-mail -itd -p 25:25 -p 3000:3000 rockmaity/forsaken-mail

Browse http://localhost:3000/

