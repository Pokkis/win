### 1、ONVIF简介

Open Network Video Interface Forum，开放型网络视频接口论坛。008年5月，由安讯士(AXIS)联合博世(BOSCH)及索尼(SONY)公司三方携手共同成立一个国际开放型网络视频产品标准网络接口开发论坛，并以公开、开放的原则共同制定开放性行业标准。是一个提供开放网络视频接口的论坛组织。ONVIF规范描述了网络视频的模型、接口、数据类型以及数据交互的模式。



### 2、ONVIF的好处

ONVIF可以让不同厂商所提供的产品，均可以通过统一的语言来进行交流，增加了协同性和灵活性。

ONVIF协议并不是针对某些设备或某些特定的项目，其开放性和兼容性减少了不同方案设备的集成成本，提高了解决方案的灵活性。



### 3、ONVIF的服务端和客户端

我们以ONVIF for client（客户端ONVIF）和ONVIF for server(服务端 ONVIF)来分类。

IPC: ONVIF服务端（相对于VMS和NVR）

NVR:ONVIF客户端（相对于IPC） ONVIF服务端（相对于VMS）

VMS:ONVIF客户端（相对于IPC和NVR）



服务端：接收并处理客户端请求，最后返回处理结果；

客户端：对返回的信息进行解析，获取相关信息，实现信息交互。



相对于IPC，NVR作为ONVIF协议的客户端，获取设备相关信息；

相对于VMS，NVR作为ONVIF协议的服务端，负责协议的转发，使客户端顺 利的访问前端设备。



### 4、NVR、VMS和IPC之间的交互

![img](.\pic_resource\onvif_note\onvif_vms_nvr_ipc.jpg)



### 5、ONVIF设备之间的关联

服务端和客户端的ONVIF哪个重要？都重要，他们需要双向交互而并非单向处理。

ONVIF NVR 挂接 ONVIF IPC的性能和稳定性跟NVR和IPC都相关，并非一个做好就行。



### 6、目前监控行业上大家的“ONVIF标准”

IPC接入海康、大华NVR可以正常使用控制。

NVR接入海康、大华的IPC可以正常使用控制。



### 7、ONVIF的好和坏。

有A品牌的ONVIF IPC，B品牌的ONVIF NVR和C品牌的ONVIF NVR。

当A家的ONVIF IPC和B家ONVIF NVR可以对接，而不能和C家的ONVIF NVR对接，不能简单就得出C家ONVIF NVR不好的结论。此结论同样适用于IPC。



### 8、常用安防软件，Onvif，RSTP客户端 , 小工具



以下做安防软件产品:

VLC: 

Onvif Device Manager  :  https://sourceforge.net/projects/onvifdm/

Onvif Device Test Tool : 

海康卫视:  https://www.hikvision.com/cn/prlb_218.html

大华:  https://www.dahuatech.com/service/downloadlists/840.html

Blue Iris :  http://blueirissoftware.com/download/

Nx Witness : http://www.networkoptix.com/download-nx-witness-chinese/

GV-VMS ： http://www.geovision.com.tw/download/product/GV-VMS  使用很难！

EasyPlayer:  

 

ONVIF Device Test Tool是官方发布的onvif检测工具，该工具[只提供给ONVIF会员使用](https://www.unifore.net/ip-video-surveillance/a-brief-introduction-of-onvif-device-test-tool.html)，官网不提供下载

下载链接：http://www.happytimesoft.com/download.html（20190219更新：当前似乎该页面也已不提供下载）

[添加Onvif设备到Blue IRIS步骤](https://blog.csdn.net/u011430225/article/details/83651296)

