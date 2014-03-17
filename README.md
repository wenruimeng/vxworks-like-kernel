vxworks-like-kernel
===================

QQ讨论群：2749 83126 （加入请注名 vxworks）

A little kernel like vxworks with full of features. Origin release at http://code.google.com/p/vxworks-like-kernel/
Introduce at http://www.gandancing.com/archive/40.html  

在线测试地址
==========
	* 新增了jsmips的branch，可以让它在[浏览器中运行](http://jannson.github.io/vxworks)
	[jsmips](https://github.com/jannson/jsmips)是一个可以在浏览器当中运行的mips虚拟机。

Main Discription
===

A little real-time kernel for learning Vxworks, which has functions of
   * rt task (create/delay/delete)
   * binary/count/mutex sem
   * message box.
It can run on gxemul mips. You can use some classic test like s7_inversion1.c for it too.

Some links about it.
====
   * [http://www.gandancing.com/archive/40.html]
   * [http://www.cnblogs.com/gandancing/archive/2012/03/21/2410844.html]

招募开发者
==

前言
====

最近观察发现，论坛的vxworks版块还是有一些火的。<br/>
很多同学对vxworks内核的探究的脚步还是没有停止的。<br/>
实时内核到底有多神秘，有多难？——告诉你，其实内核调度部分才5000行代码不到。<br/>

功能
====

两年前为了公司vxworks的培训，自己翻看vxworks的代码学习并写了当时，也是现在的vxworks-like-kernel版本。（大概7000行代码）

功能具有：
  # 任务创建删除延迟等
  # 两进制信号量
  # 计数信号量
  # 互斥信号量，任务优先级反转
  # 消息队列
  # VxWorks 内存分区
  # 串口输入输出
  # 内存、对象、优先级队列可以在VS2005上运行

这个是一个最好的起步学习vxworks实时内核的版本，并且在我手里放了两年。一天看了《失控》这本书，突然觉得应该把它分享出去。现在，分享的时刻到了。

==接下来的工作==
同时，我希望这个项目能在未来更好的帮助同学们学习实时内核，让更多的人有更大的进步，希望更多的人能亲自参与进来改进它。
既然是参与开发，接下来的工作很简单：
  * 写一个编译运行的指导的帖子。
    以前我用cygwin的工具链编译，也可以用tornado mips编译。在gxemul虚拟机下运行。但是现在电脑里都把这些东西删掉了，所以没办法写了。有心人士使用过程中帮忙写一分。
  * 代码托管在google code上，希望有人能与我一起维护它。比如把各项信息写全，发布release等。
  * 以前都在windows安装cygwin写的，并且在gxemul上运行。（它完全可以不使用Tornado）
    希望有心人士能把它移植到linux下的mips工具链上编译，并运行在qemu上，让更多人方便使用qemu来源代码调试。让未来的学习更方便
  * 上面的任务是近期的。对于未来，我把它交给大家，希望大家来提意见。
  * 希望版主能帮忙增加它的人气，让更多的人参与发帖，最后增加论坛人气。

最后，希望奋战在技术领域的同学能从它得到进步，能工作开心^.^ 有空读读《失控》这本书，呵呵~
