### RTSP 协议分析

**1.概述：**
 RTSP（Real Time Streaming Protocol），实时流传输协议，是TCP/IP协议体系中的一个应用层协议，由哥伦比亚大学、网景和RealNetworks公司提交的IETF RFC标准。该协议定义了一对多应用程序如何有效地通过IP网络传送多媒体数据。类似HTTP协议的流控制协议。它们都使用纯文本来发送信息，而且rtsp协议的语法也和HTTP类似，和HTTP协议相比RTSP协议所不同的地方是，RTSP协议是有状态的协议，而HTTP是无状态的协议。RTSP通过维护一个session来维护其状态的转换。RTSP协议的默认端口是554，默认的承载协议为TCP。

**2.RTSP的特性：**
（1）.流控分离：从控制逻辑上来说RTSP和FTP相似，控制流和数据流是分开的。
（2）.可扩展性：因为RTSP协议是基于文本的协议所以其具有较强的可扩展性。
（3）.安全：RTSP 使用网页安全机制。

**3.RTSP 协议格式：**
请求命令的格式为：
METHOD URL CR LF
Field1:value CR LF
Field2:value CR LF
......
Fieldn:value CR LF
CR LF

应答的格式为：
RTSP/major_version.minor_version status CR LF
Field1:value CR LF
Field2:value CR LF
......
Fieldn:value CR LF
CR LF

**4.RTSP的主要命令：**

![img](.\pic_resource\RTSP_note\RTSP_cmd.jpg)

**5.RTSP命令的状态转换表**

![img](.\pic_resource\RTSP_note\RTSP_cmd_state_change.jpg)

**6.RTSP状态码**

Status-Code = "100" ; Continue
| "200" ; OK
| "201" ; Created
| "250" ; Low on Storage Space
| "300" ; Multiple Choices
| "301" ; Moved Permanently
| "302" ; Moved Temporarily
| "303" ; See Other
| "304" ; Not Modified
| "305" ; Use Proxy
| "400" ; Bad Request
| "401" ; Unauthorized
| "402" ; Payment Required
| "403" ; Forbidden
| "404" ; Not Found
| "405" ; Method Not Allowed
| "406" ; Not Acceptable
| "407" ; Proxy Authentication Required
| "408" ; Request Time-out
| "410" ; Gone
| "411" ; Length Required
| "412" ; Precondition Failed
| "413" ; Request Entity Too Large
| "414" ; Request-URI Too Large
| "415" ; Unsupported Media Type
| "451" ; Parameter Not Understood
| "452" ; Conference Not Found
| "453" ; Not Enough Bandwidth
| "454" ; Session Not Found
| "455" ; Method Not Valid in This State
| "456" ; Header Field Not Valid for Resource
| "457" ; Invalid Range
| "458" ; Parameter Is Read-Only
| "459" ; Aggregate operation not allowed
| "460" ; Only aggregate operation allowed
| "461" ; Unsupported transport
| "462" ; Destination unreachable
| "500" ; Internal Server Error
| "501" ; Not Implemented
| "502" ; Bad Gateway
| "503" ; Service Unavailable
| "504" ; Gateway Time-out
| "505" ; RTSP Version not supported
| "551" ; Option not supported
| extension-code
extension-code = 3DIGIT
Reason-Phrase = *<TEXT, excluding CR, LF

### 以下是某省IPTV的RTSP协商过程：

```js
DESCRIBE rtsp://118.122.89.27:554/live/ch10083121594790060557.sdp?playtype=1&boid=001&backupagent=118.122.89.27:554&clienttype=1&time=20100929182111+08&life=172800&ifpricereqsnd=1&vcdnid=001&userid=123&mediaid=ch10083121594790060557&ctype=2&TSTVTimeLife=1800&contname=&authid=0&UserLiveType=1&nodelevel=3 RTSP/1.0
//媒体URL
Accept: application/sdp
//协商用于描述媒体信息协议
CSeq: 1
User-Agent:ZTE Ltd.co RTSP protocal verion 1.0 guid-2.1.1.100/B519D290-C0EC-EE35-7368-893BE4C0B347
//User Agnet信息，显示是中兴的服务器，1.0的版本。如果是Helix服务器的话会有Helix 服务器的标识。
x-NAT:2.1.1.100:20081
//主要用于NAT穿透
x-zmssRtxSdp: yes

RTSP/1.0 200 OK
//应答编号
Server: ZXUSS100 1.0
Cache-Control: no-cache
Content-Base: rtsp://118.122.89.27:554/live/ch10083121594790060557.sdp/
Content-Length: 320
//内容长度信息
Content-Type: application/sdp
//描述内容信息所用的协议
CSeq: 1
Date: Wed, 29 Sep 2010 10:20:38 GMT
Expires: Wed, 29 Sep 2010 10:20:38 GMT
// SDP 描述信息
v=0
o=- 296874273 1 IN IP4 118.122.89.27
s=envivio
c=IN IP4 0.0.0.0
b=AS:1500
t=0 0
a=range:clock=20100929T095038.00Z-20100929T102038.00Z
m=video 5140 RTP/AVPF 33 96
a=control:trackID=2
a=rtpmap:33 MP2T/90000
a=3GPP-Adaptation-Support:5
a=rtcp-fb:33 nack
a=rtpmap:96 rtx/90000
a=fmtp:96 apt=33;rtx-time=0
SETUP rtsp://118.122.89.27:554/live/ch10083121594790060557.sdp/trackID=2 RTSP/1.0
//用于建立RTSP连接，协商传输用的协议。
CSeq: 2
3GPP-Adaptation: url=rtsp://118.122.89.27:554/live/ch10083121594790060557.sdp/trackID=2;size=1061400;target-time=2200
Transport: MP2T/RTP/UDP;unicast;destination=2.1.1.100;client_port=8360-8361,MP2T/RTP/TCP;unicast;destination=2.1.1.100;interleaved=0-1,MP2T/UDP;unicast;destination=2.1.1.100;client_port=8360-8361,MP2T/TCP;unicast;destination=2.1.1.100;interleaved=0-1
// MP2T/RTP/UDP表示是TS流用于RTP打包，基于UDP传输。MP2T/RTP/TCP表示是TS流用于RTP打包，基于TCP。
User-Agent:ZTE Ltd.co RTSP protocal verion 1.0 guid-2.1.1.100/B519D290-C0EC-EE35-7368-893BE4C0B347

RTSP/1.0 200 OK
Server: ZXUSS100 1.0
CSeq: 2
Date: Wed, 29 Sep 2010 10:20:38 GMT
Expires: Wed, 29 Sep 2010 10:20:38 GMT
Session: 65565885
Transport: MP2T/RTP/UDP;unicast;destination=2.1.1.100;client_port=8360-8361;server_port=13306-13307;source=118.122.89.29
// 通过协商MP2T/RTP/UDP表示是TS流用于RTP打包，基于UDP传输。server端端口：13306-13307。client端端口：8360-8361
3GPP-Adaptation:url=rtsp://118.122.89.27:554/live/ch10083121594790060557.sdp/trackID=2;size=1061400;target-time=2200

PLAY rtsp://118.122.89.27:554/live/ch10083121594790060557.sdp?playtype=1&boid=001&backupagent=118.122.89.27:554&clienttype=1&time=20100929182111+08&life=172800&ifpricereqsnd=1&vcdnid=001&userid=123&mediaid=ch10083121594790060557&ctype=2&TSTVTimeLife=1800&contname=&authid=0&UserLiveType=1&nodelevel=3 RTSP/1.0
//播放视频。对于有些视频可能会分别要对音频视频进行play。
CSeq: 3
Session: 65565885
User-Agent:ZTE Ltd.co RTSP protocal verion 1.0 guid-2.1.1.100/B519D290-C0EC-EE35-7368-893BE4C0B347
Scale: 1.0
Range: npt=end-
//npt=end-在IPTV2.0里面有其自身的定义（可参看上海电信IPTV2.0标准）

RTSP/1.0 200 OK
Server: ZXUSS100 1.0
CSeq: 3
Range: npt=end-
Scale: 1.0
Session: 65565885
RTP-Info: url=rtsp://118.122.89.29:13306/live/ch10083121594790060557.sdp/trackID=2

......

PLAY rtsp://118.122.89.27:554/live/ch10083121594790060557.sdp?playtype=1&boid=001&backupagent=118.122.89.27:554&clienttype=1&time=20100929182111+08&life=172800&ifpricereqsnd=1&vcdnid=001&userid=123&mediaid=ch10083121594790060557&ctype=2&TSTVTimeLife=1800&contname=&authid=0&UserLiveType=1&nodelevel=3 RTSP/1.0
//play的另外一种用法。用于快进和快退。
CSeq: 43
Session: 65565885
User-Agent:ZTE Ltd.co RTSP protocal verion 1.0 guid-2.1.1.100/B519D290-C0EC-EE35-7368-893BE4C0B347
Scale: -32.0
//Scale: -32.0，表示以32倍速快退。Scale: 32.0，表示以32倍速快进。
Range: npt=now-
RTSP/1.0 200 OK
Server: ZXUSS100 1.0
CSeq: 43
Range: clock=20100929T102609.02Z-20100929T095637.75Z
Scale: -32.0
Session: 65565885
RTP-Info: url=rtsp://118.122.89.36:10084/live/ch10083121594790060557.sdp/trackID=2;seq=22277;rtptime=1792329138

GET_PARAMETER rtsp://118.122.89.27:554/live/ch10083121594790060557.sdp?playtype=1&boid=001&backupagent=118.122.89.27:554&clienttype=1&time=20100929182111+08&life=172800&ifpricereqsnd=1&vcdnid=001&userid=123&mediaid=ch10083121594790060557&ctype=2&TSTVTimeLife=1800&contname=&authid=0&UserLiveType=1&nodelevel=3 RTSP/1.0
//获取当前的一些播放参数。
CSeq: 44
Session: 65565885
User-Agent:ZTE Ltd.co RTSP protocal verion 1.0 guid-2.1.1.100/B519D290-C0EC-EE35-7368-893BE4C0B347
x-Timeshift_Range
//请求时移的范围
x-Timeshift_Current
//获取当前的时间点

RTSP/1.0 200 OK
//返回当前的媒体信息。
Server: ZXUSS100 1.0
CSeq: 44
Session: 65565885
x-Timeshift_Range: clock=20100929T095638.83Z-20100929T102638.83Z
x-Timeshift_Current: clock=20100929T102530.20Z;rtptime=1788844914

中间省去了45，46，47三个包。通过CSeq: 48字段可以看出来

PLAY rtsp://118.122.89.27:554/live/ch10083121594790060557.sdp?playtype=1&boid=001&backupagent=118.122.89.27:554&clienttype=1&time=20100929182111+08&life=172800&ifpricereqsnd=1&vcdnid=001&userid=123&mediaid=ch10083121594790060557&ctype=2&TSTVTimeLife=1800&contname=&authid=0&UserLiveType=1&nodelevel=3 RTSP/1.0
用于恢复正常的播放速度。
CSeq: 48
Session: 65565885
User-Agent:ZTE Ltd.co RTSP protocal verion 1.0 guid-2.1.1.100/B519D290-C0EC-EE35-7368-893BE4C0B347
Scale: 1.0
Range: npt=beginning-

RTSP/1.0 200 OK
Server: ZXUSS100 1.0
CSeq: 48
Range: clock=20100929T095730.00Z-20100929T102730.61Z
//可以时移的范围。
Scale: 1.0
Session: 65565885
RTP-Info: url=rtsp://118.122.89.36:10084/live/ch10083121594790060557.sdp/trackID=2;seq=39900;rtptime=1637595010
＜中间在Descript应答中所用的ＳＤＰ协议将会在以后中描述＞
```



### 参考

https://www.cnblogs.com/qingquan/archive/2011/07/14/2106834.html

https://www.cnblogs.com/qingquan/archive/2011/07/15/2107867.html