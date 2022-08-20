# onvif交接说明

## 一、onvif修改：

​	我所做的onvif修改都是onvif的定制，无需更新至中性，主要包括：

1、onvif的osd获取、设置问题， 这些是客户使用其它或者新版nvr导致；

2、onvif的控制指令定制，比如某些旧版本无法onvif校时、屏蔽一些onvif控制命令等；

3、onvif的基本信息参数修改，比如客户定制的设备名、model等；

4、onvif上报matadata数据等；





## 二、onvif说明：

### 2.0 onvif配置文件

/usr/onvif/device：用于记录设备的基本信息，没有用过；

/usr/onvif/event：事件相关的配置文件，没有用过；

/usr/onvif/media2：用于控制是否支持h265，media2enable为1则支持h265，用过；

/usr/onvif/search：搜索相关配置，onvif://www.onvif.org/extension/unique_identifier可用于是否允许在大华NVR搜索出onvif协议，用过；

/usr/onvif/version：没有用过；

/usr/onvif/information：一些onvif Manufacturer、Model设置，用过；

/usr/onvif/profile：一些profile概要文件内容，没有用过；

/usr/onvif/usrcount：没有用过；



### 2.1 设备支持的onvif通信标准

ONVIF提供很多的profile概要文件，用于规范ONVIF设备端与ONVIF客户端的通信标准，主要有profile S、G、C、Q、A，不同的profile文件应用于不同的领域。我们的onvif只支持onvif profile s，应用于网络视频系统，内容包括：

1、视频和音频流

2、PTZ控制和继电器输出

3、视频配置和多播



### 2.2 控制命令交互

主程序：Cmdtask.c

onvif：onvif.c



### 2.3 onvif密码验证

ONVIF_AUTH_CHECK函数





# 三、使用工具

主要有:

ONVIF Device Manager:测试流程，只支持h264；

ONVIF Device Test Tool:测试接口；



