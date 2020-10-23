# zClock

使用esp8266驱动TIM1628驱动6个数码管显示时间

文档还在完善中....

![zClock](https://cdn.jsdelivr.net/gh/a2633063/Image/zClock/zClock.png)



建立了个QQ群,有问题可以加入来讨论:**459996006**  [点这里直接加群](//shang.qq.com/wpa/qunwpa?idkey=9104eabd6131d856b527ad89636fc603eb745a5d047e8b45d183165c8e607e59)  (收费群,请用android或pc入群. 支持我请加群或通过app端打赏我)(如果提示禁止入群,可能为付费群的一些问题,可以晚点再试)

也可以发送邮件给我:zip_zhang@foxmail.com  (邮件较多,可能会较晚回复)



> ### 作者声明
>
> 注意: 本项目主要目的为作者本人自己学习及使用DC1排插而开发,本着开源精神及造福网友而开源,仅个人开发,可能无法做到完整的测试,所以不承担他人使用本项目照成的所有后果.
>
> **严禁他人将本项目用户用于任何商业活动.个人在非盈利情况下可以自己使用,严禁收费代刷等任何盈利服务.**
>
> 有需要请联系作者:zip_zhang@foxmail.com



## 特性

勾选为已完成功能,未勾选为计划后续增加的功能.注意:计划完成功能并不代表一定会完成此功能,若


- [x] ds3231时间 	
- [x] 光敏电阻自动配置亮度/手动设置亮度
- [ ] 自动亮度档位调节
- [ ] ~~水银开关自动配置数码管方向~~ 当前硬件无水银开关
- [x] 手动设置屏幕显示方向
- [x] Web配置wifi
- [x] 自动网络校时
- [x] 日期时间交替显示
- [ ] 日期时间交替显示开关/设置
- [x] 在线升级

## 目录

[前言(必看)](#前言必看)

[开始](#开始)

[电路图](#电路图)

[拆机接线及烧录固件相关](#拆机接线及烧录固件相关)

[开始使用/使用方法](#开始使用/使用方法)

[其他内容](#其他内容)

​	[通信协议](#通信协议)

​	[FAQ](#FAQ)

[文档更新日志](#文档更新日志)



## 前言(必看)

- 除非写明了`如果你不是开发人员,请忽略此项`之类的字眼,否则,请**一个字一个字看清楚看完**整后再考虑动手及提问!很可能一句话就是你成功与否的关键!

- 烧录固件需要烧录器:一般的ttl串口烧录器即可,一般刷机的人应该都有,淘宝价格大概为2-5元

- 使用此固件,需要app端配合,见[SmartControl_Android_MQTT](https://github.com/a2633063/SmartControl_Android_MQTT).

- app只有android,因ios限制,本人不考虑免费做ios开发.(不要再问是否有ios端).

  

> 虽然没有ios端,但固件支持homeassistant,可以使用安卓APP配置完成后,连入homeassistant后,使用ios控制. APP主要仅为第一次使用配对网络及配置mqtt服务器时使用,之后可以用homeassistant控制不再使用app.

> 如果你不知道什么是mqtt或homeassistant,所有有关的内容可以跳过.

> 如果你有任何问题,可以直接在此项目中提交issue,或给我发送邮件:zip_zhang@foxmail.com,邮件标题中请注明[zClock].
>
> 





## 开始

整体流程如下:拆开DC1,将固件/烧录器/pc互相连接,在pc运行烧录软件进行烧录,烧录固件.

烧录完成后,首次使用前配对网络并配置mqtt服务器,之后就可以使用了.



## 电路图

电路图文件见 [PCB_LED_6_ESP8266.pdf](https://github.com/a2633063/zClock/blob/master/PCB_LED_6_ESP8266.pdf)

pcb文件见[PCB_LED_6_ESP8266_0625.PcbDoc](https://github.com/a2633063/zClock/blob/master/PCB_LED_6_ESP8266_0625.PcbDoc)



## 拆机接线及烧录固件相关

见[固件烧录](https://github.com/a2633063/zClock/wiki/固件烧录)

烧录固件完成后,即可开始使用



## 开始使用/使用方法

见[开始使用](https://github.com/a2633063/zClock/wiki/开始使用)



## 其他内容



### 通信协议

> 此项为专业开发人员准备,如果你不是开发人员,请跳过此项

为了开发人员开发自己的控制软件,将通信协议开放

所有通信协议开源,你可以自己开发控制app或ios端

见[通信协议](https://github.com/a2633063/zClock/wiki/通信协议)



### FAQ

见 [FAQ](https://github.com/a2633063/SmartControl_Android_MQTT/wiki/FAQ)


