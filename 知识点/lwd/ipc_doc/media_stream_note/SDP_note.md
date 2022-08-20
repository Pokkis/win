## SDP协议

### 1、SDP协议介绍

​    SDP 完全是一种会话描述格式（对应的[RFC2327](https://www.rfc-editor.org/rfc/rfc2327.txt)，最新[ draft-ietf-mmusic-rfc4566bis-37](https://www.ietf.org/id/draft-ietf-mmusic-rfc4566bis-37.txt)） ，它不属于传输协议 ，它只使用不同的适当的传输协议，包括会话通知协议（SAP）、会话初始协议（SIP）、实时流协议（RTSP）、MIME 扩展协议的电子邮件以及超文本传输协议（HTTP）。SDP协议是也是基于文本的协议，这样就能保证协议的可扩展性比较强，这样就使其具有广泛的应用范围。SDP 不支持会话内容或媒体编码的协商，所以在流媒体中只用来描述媒体信息。媒体协商这一块要用RTSP来实现．

流媒体协议sdp信息，附带在describe报文中有rtsp服务端发出，主要目的，告之会话的存在和给出参与该会话所必须的信息，sdp会话完全是文本形式，采用UTF-8编码的ISO 10646字符集

标准的SDP规范主要包括SDP描述格式和SDP结构，其中SDP结构里面最重要的两项内容是会话描述信息和媒体描述信息。一般会话级描述从v=开始一直到第一个媒体描述为止，媒体描述是从m=开始一直到下一个媒体描述m=的位置之前。会话级的作用域是整个会话，其位置从v=开始到第一个媒体描述m=为止；媒体级描述 是对单个媒体流即音频流、视频流和字幕流等的单个媒体描述，如果有多个流则用多组媒体级描述。其中每个媒体级描述就是从m=开始到下一个媒体描述m=为止。

#### sdp描叙符包括：

- 会话名和目的
- 会话激活的时间区段
- 构成会话的媒体
- 接收这些媒体所需要的信息（地址，端口，格式）
- 会话所用的带宽信息
- 会话负责人的联系信息

#### 媒体信息包括：

- 媒体类型（视频，音频等）
- 传送协议（RTP/UDP/IP H.320等）
- 媒体格式（H,264视频，MPEG视频等）
- 媒体地址和端口

### 2、SDP协议格式

SDP描述由许多文本行组成，文本行的格式为<类型>=<值>，<类型>是一个字母，<值>是结构化的文本串，其格式依<类型>而定。

**＜type＞ = < value > [CRLF]** 

type: 该字节为单字节（如： v，o, m等）区分大小写，=号俩侧部允许有空格

value: 为结构化文本串

#### 2.1、会话描叙格式介绍

v=0

```
协议版本，protocol version 必选字段，给出sdp的版本号，目前为0版本，无子版本号
```

o=(用户名)（会话标识）（版本）（网络类型）（地址类型）（地址）

```
会话源，owner 必选字段，如果不存在用户登录名，该字段标志位“-” 会话标识为一随机数字串 版本为该会话公告的版本 \r\n 网络类型为文本串， \n “IN”表示internet 地址类型为文本串，目前定义为“IP4”和“IP6”两种地址
```

s=(会话名)

```
会话名，session name，会话名称，必选字段，每个会话描述必须只有一个会话名
```

i=(会话信息)

```
会话信息，此字段并非必须，建议包括进来用于描叙相应会话文字性说明，每个会话描叙最多只能有一个
```

u=(URL)

```
URL，此字段并非必须,提供url的描叙信息
```

c=(网络类型)（地址类型）（连接地址）

```
连接数据
```



#### 2.1、时间描述

t = time the session is active

```
会话活动时间，必选字段，表示的是该会话的开始到结束时间，如果是持久会话，则时间值填0，这样的时间是NTP时间，单位是秒，格式是 ：
t=<start time> <stop time>。
```

r = * 

```
0或多次重复次数
```

#### 2.3、媒体描述

m = media name and transport address

```
媒体名称和传输地址，可选字段，但是一般音视频会话至少有音频流或者视频流，所以一般也是都会有的，如果多个流则有多个，一般就是从这个m=到下个媒体会话m=结束，格式：
m=<media type> <port> <transport> <fmt list>

<media type>：媒体类型， audio或者video
<port>：媒体端口，要么是收流端口，要么是发流端口，这样我们就知道从哪个端口进行发流和收流。
<transport>：传输协议，表示码流的传输协议是什么，如果UDP则为RTP/AVP代表码流用RTP Over UDP，要么是TCP/RTP/AVP即RTP Over TCP，表示用TCP传输RTP码流。
<fmt list>:媒体格式，这里表示的负载类型，一般表示的视频的H.264 H.265等,音频则是 G7xx、AAC、Opus等类型。

m=video 0 RTP/AVP 96
```

i = * 

```
媒体标题
```

c = * 

```
连接信息 — 如果包含在会话层则该字段可选
```

b = * 

```
带宽信息
```

k = * 

```
加密密钥
```

a = attribute

```
0 个或多个会话属性行，可选字段，表示的媒体的属性，进一步的描述媒体信息，可以有多个属性，其中比较重要的属性就是rtpmap和fmtp，格式是：
a=<type>
a=<type>:<value>

格式：a=rtpmap:（净荷类型）（编码名）/（时钟速率）[(编码参数)]
a=control:（音/视频连接信息）
a=control:rtsp://192.168.1.197/h264stream0/trackID=0
a=rtpmap:96 H264/90000

rtpmap属性，表示媒体流传输协议的RTP具体内容：
a=rtpmap:<payload type> <encoding name>/<clock rate>[/encoding parameters]
rtpmap:是rtp map即RTP参数映射表
<payload type>：负载类型，对应表示RTP包中的音视频数据负载类型，比如RTP的数据类型是H.264，那么这里就是96。
<encoding name>:编码器名称，这里主要指的RTP承载音视频编码数据类型，当然可以是标准数据也可以私有数据，如VP8 VP9 H.264等。
<sample rate>:采样率，音视频里面都有时间戳的概念，所以这里表示的音视频的采样率，对音视频同步非常重要。比如视频的90000，音频的8000、48000等。
<encoding parameters>：编码参数，可以表示视频的分辨率，帧率，音频的单声道双声道等信息。

fmtp属性，在rtpmap基础上进一步描述音视频具体编码参数信息：
格式：a=fmtp:<payload type> <format specific parameters>
fmtp，格式参数，即 format  parameters；
<payload type>：负载类型，同样对应 RTP 包中的音视频数据负载类型；
< format specific parameters>：指具体参数，或者说对音视频编码信息的一次处理。该信息从编码器得到，比如视频的SPS\PPS等，用于解码端的播放器初始化。
```

### 3、SDP协议例子

```
v=0
o=StreamingServer 3677033027 1437537780000 IN IP4 192.168.1.44
s=\demo.mp4
u=http:///
e=admin@
c=IN IP4 0.0.0.0
b=AS:1398
t=0 0
a=control:*
a=x-copyright: MP4/3GP File hinted with GPAC 0.5.0-rev4065 (C)2000-2005 - http://gpac.sourceforge.net
a=range:npt=0- 216.52167

m=video 0 RTP/AVP 96
b=AS:1242
a=3GPP-Adaptation-Support:1
a=rtpmap:96 H264/90000
a=control:trackID=65536
a=fmtp:96 profile-level-id=42000A; packetization-mode=1; sprop-parameter-sets=Z0IACpZUBQHogA==,aM44gA==
a=framesize:96 640-480

m=audio 0 RTP/AVP 97
b=AS:156
a=3GPP-Adaptation-Support:1
a=rtpmap:97 mpeg4-generic/48000/1
a=control:trackID=65537
a=fmtp:97 profile-level-id=41; config=1188; streamType=5; mode=AAC-hbr; objectType=64; constantDuration=1024; sizeLength=13; indexLength=3; indexDeltaLength=3
1234567891011121314151617181920212223242526272829

```



```
v=0
//sdp版本号，一直为0,rfc4566规定
o=- 7017624586836067756 2 IN IP4 127.0.0.1
// o=<username> <sess-id> <sess-version> <nettype> <addrtype> <unicast-address>
//username如何没有使用-代替，7017624586836067756是整个会话的编号，2代表会话版本，如果在会话
//过程中有改变编码之类的操作，重新生成sdp时,sess-id不变，sess-version加1
s=-
//会话名，没有的话使用-代替
t=0 0
//两个值分别是会话的起始时间和结束时间，这里都是0代表没有限制
a=group:BUNDLE audio video data
//需要共用一个传输通道传输的媒体，如果没有这一行，音视频，数据就会分别单独用一个udp端口来发送
a=msid-semantic: WMS h1aZ20mbQB0GSsq0YxLfJmiYWE9CBfGch97C
//WMS是WebRTC Media Stream简称，这一行定义了本客户端支持同时传输多个流，一个流可以包括多个track,
//一般定义了这个，后面a=ssrc这一行就会有msid,mslabel等属性
m=audio 9 UDP/TLS/RTP/SAVPF 111 103 104 9 0 8 106 105 13 126
//m=audio说明本会话包含音频，9代表音频使用端口9来传输，但是在webrtc中一现在一般不使用，如果设置为0，代表不
//传输音频,UDP/TLS/RTP/SAVPF是表示用户来传输音频支持的协议，udp，tls,rtp代表使用udp来传输rtp包，并使用tls加密
//SAVPF代表使用srtcp的反馈机制来控制通信过程,后台111 103 104 9 0 8 106 105 13 126表示本会话音频支持的编码，后台几行会有详细补充说明
c=IN IP4 0.0.0.0
//这一行表示你要用来接收或者发送音频使用的IP地址，webrtc使用ice传输，不使用这个地址
a=rtcp:9 IN IP4 0.0.0.0
//用来传输rtcp地地址和端口，webrtc中不使用
a=ice-ufrag:khLS
a=ice-pwd:cxLzteJaJBou3DspNaPsJhlQ
//以上两行是ice协商过程中的安全验证信息
a=fingerprint:sha-256 FA:14:42:3B:C7:97:1B:E8:AE:0C2:71:03:05:05:16:8F:B9:C7:98:E9:60:43:4B:5B:2C:28:EE:5C:8F3:17
//以上这行是dtls协商过程中需要的认证信息
a=setup:actpass
//以上这行代表本客户端在dtls协商过程中，可以做客户端也可以做服务端，参考rfc4145 rfc4572
a=mid:audio
//在前面BUNDLE这一行中用到的媒体标识
a=extmap:1 urn:ietf:params:rtp-hdrext:ssrc-audio-level
//上一行指出我要在rtp头部中加入音量信息，参考 rfc6464
a=sendrecv
//上一行指出我是双向通信，另外几种类型是recvonly,sendonly,inactive
a=rtcp-mux
//上一行指出rtp,rtcp包使用同一个端口来传输
//下面几行都是对m=audio这一行的媒体编码补充说明，指出了编码采用的编号，采样率，声道等
a=rtpmap:111 opus/48000/2
a=rtcp-fb:111 transport-cc
//以上这行说明opus编码支持使用rtcp来控制拥塞，参考https://tools.ietf.org/html/draft-holmer-rmcat-transport-wide-cc-extensions-01
a=fmtp:111 minptime=10;useinbandfec=1
//对opus编码可选的补充说明,minptime代表最小打包时长是10ms，useinbandfec=1代表使用opus编码内置fec特性
a=rtpmap:103 ISAC/16000
a=rtpmap:104 ISAC/32000
a=rtpmap:9 G722/8000
a=rtpmap:0 PCMU/8000
a=rtpmap:8 PCMA/8000
a=rtpmap:106 CN/32000
a=rtpmap:105 CN/16000
a=rtpmap:13 CN/8000
a=rtpmap:126 telephone-event/8000
a=ssrc:18509423 cname:sTjtznXLCNH7nbRw
//cname用来标识一个数据源，ssrc当发生冲突时可能会发生变化，但是cname不会发生变化，也会出现在rtcp包中SDEC中，
//用于音视频同步
a=ssrc:18509423 msid:h1aZ20mbQB0GSsq0YxLfJmiYWE9CBfGch97C 15598a91-caf9-4fff-a28f-3082310b2b7a
//以上这一行定义了ssrc和WebRTC中的MediaStream,AudioTrack之间的关系，msid后面第一个属性是stream-d,第二个是track-id
a=ssrc:18509423 mslabel:h1aZ20mbQB0GSsq0YxLfJmiYWE9CBfGch97C
a=ssrc:18509423 label:15598a91-caf9-4fff-a28f-3082310b2b7a
m=video 9 UDP/TLS/RTP/SAVPF 100 101 107 116 117 96 97 99 98
//参考上面m=audio,含义类似
c=IN IP4 0.0.0.0
a=rtcp:9 IN IP4 0.0.0.0
a=ice-ufrag:khLS
a=ice-pwd:cxLzteJaJBou3DspNaPsJhlQ
a=fingerprint:sha-256 FA:14:42:3B:C7:97:1B:E8:AE:0C2:71:03:05:05:16:8F:B9:C7:98:E9:60:43:4B:5B:2C:28:EE:5C:8F3:17
a=setup:actpass
a=mid:video
a=extmap:2 urn:ietf:params:rtp-hdrext:toffset
a=extmap:3 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
a=extmap:4 urn:3gpp:video-orientation
a=extmap:5 http://www.ietf.org/id/draft-hol ... de-cc-extensions-01
a=extmap:6 http://www.webrtc.org/experiments/rtp-hdrext/playout-delay
a=sendrecv
a=rtcp-mux
a=rtcp-rsize
a=rtpmap:100 VP8/90000
a=rtcp-fb:100 ccm fir
//ccm是codec control using RTCP feedback message简称，意思是支持使用rtcp反馈机制来实现编码控制，fir是Full Intra Request
//简称，意思是接收方通知发送方发送幅完全帧过来
a=rtcp-fb:100 nack
//支持丢包重传，参考rfc4585
a=rtcp-fb:100 nack pli
//支持关键帧丢包重传,参考rfc4585
a=rtcp-fb:100 goog-remb
//支持使用rtcp包来控制发送方的码流
a=rtcp-fb:100 transport-cc
//参考上面opus
a=rtpmap:101 VP9/90000
a=rtcp-fb:101 ccm fir
a=rtcp-fb:101 nack
a=rtcp-fb:101 nack pli
a=rtcp-fb:101 goog-remb
a=rtcp-fb:101 transport-cc
a=rtpmap:107 H264/90000
a=rtcp-fb:107 ccm fir
a=rtcp-fb:107 nack
a=rtcp-fb:107 nack pli
a=rtcp-fb:107 goog-remb
a=rtcp-fb:107 transport-cc
a=fmtp:107 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42e01f
//h264编码可选的附加说明
a=rtpmap:116 red/90000
//fec冗余编码，一般如果sdp中有这一行的话，rtp头部负载类型就是116，否则就是各编码原生负责类型
a=rtpmap:117 ulpfec/90000
//支持ULP FEC，参考rfc5109
a=rtpmap:96 rtx/90000
a=fmtp:96 apt=100
//以上两行是VP8编码的重传包rtp类型
a=rtpmap:97 rtx/90000
a=fmtp:97 apt=101
a=rtpmap:99 rtx/90000
a=fmtp:99 apt=107
a=rtpmap:98 rtx/90000
a=fmtp:98 apt=116
a=ssrc-group:FID 3463951252 1461041037
//在webrtc中，重传包和正常包ssrc是不同的，上一行中前一个是正常rtp包的ssrc,后一个是重传包的ssrc
a=ssrc:3463951252 cname:sTjtznXLCNH7nbRw
a=ssrc:3463951252 msid:h1aZ20mbQB0GSsq0YxLfJmiYWE9CBfGch97C ead4b4e9-b650-4ed5-86f8-6f5f5806346d
a=ssrc:3463951252 mslabel:h1aZ20mbQB0GSsq0YxLfJmiYWE9CBfGch97C
a=ssrc:3463951252 label:ead4b4e9-b650-4ed5-86f8-6f5f5806346d
a=ssrc:1461041037 cname:sTjtznXLCNH7nbRw
a=ssrc:1461041037 msid:h1aZ20mbQB0GSsq0YxLfJmiYWE9CBfGch97C ead4b4e9-b650-4ed5-86f8-6f5f5806346d
a=ssrc:1461041037 mslabel:h1aZ20mbQB0GSsq0YxLfJmiYWE9CBfGch97C
a=ssrc:1461041037 label:ead4b4e9-b650-4ed5-86f8-6f5f5806346d
m=application 9 DTLS/SCTP 5000
c=IN IP4 0.0.0.0
a=ice-ufrag:khLS
a=ice-pwd:cxLzteJaJBou3DspNaPsJhlQ
a=fingerprint:sha-256 FA:14:42:3B:C7:97:1B:E8:AE:0C2:71:03:05:05:16:8F:B9:C7:98:E9:60:43:4B:5B:2C:28:EE:5C:8F3:17
a=setup:actpass
a=mid:data
a=sctpmap:5000 webrtc-datachannel 1024
```



视频”a=fmtp”字段的解析 参考 RFC3984的8.2节

音频config描述符的解析 参考 RFC 3016



### 4、SDP在RTSP、国标GB28181、WebRTC中的实践 ：

无论你是用微信进行视频电话还是开Zoom视频会议，按照OSI网络七层参考模型，我们进行这些活动之前一般都要先建立一组会话。在建立会话的过程中，我们需要描述下会话的一些信息，描述这种会话能力时用到了SDP协议，也就是会话描述协议Session Description Protocol，协议详细内容在RFC4566中规定。

这么说可能不够直白，大白话解释就是：由于Web端、IOS、Android、PC、MAC端的差异性导致它们对音视频的支持能力不同，所以我们进行一些音视频会话之前，需要交互下彼此的音视频编解码能力、网络带宽和传输协议等信息，这些需要协商的信息需要用SDP来描述。

注意的是SDP虽然具备这些能力参数信息的描述功能，但是SDP并不是传输协议，需要用RTSP、SIP、HTTP等协议进行承载传输、交换，如果大家协调好了之后，就可以建立会话，完成真实的音视频码流传输，再完成解码和播放。

这篇文章主要讲下SDP协议格式和规范、具备哪些描述能力、最后再通过在RTSP和基于SIP的国标协议进行实例分析下，当然目前比较火的WebRTC在建立音视频会话前也是通过这套协议描述会话信息的。SDP应用在任何场景和行业标准中，一般都进行了裁剪和进一步的规范，如果你要了解所有的SDP信息，你可以参考RFC4566文档，如果需要了解在WebRTC中使用可以参考链接：https://www.ietf.org/archive/id/draft-nandakumar-rtcweb-sdp-08.txt

#### 4.1、SDP场景：

SDP一般用在媒体会话建立之前，可以适用于实时流媒体、点播、直播等领域，特别在音视频通话、视频会议、VoIP、视频监控等领域应用较多。媒体码流一般基于RTP传输，服务质量用RTCP协议保障。

但是SDP的交互不是所有音视频会话建立时都是必须的，假如双方提前约定好这些音视频会话创建需要的信息就不用这个步骤来交互彼此的SDP信息，比如HTTP-FLV、RTMP-FLV直播和点播方案，因为一旦采用了这套方案，这些音视频会话建立需要的信息都是确定的，但是这样会降低或者说没有充分发挥端到端的音视频能力，协商显得更加灵活点。

SDP作用：

SDP作用包括以下一些方面1)建立会话的详细信息，包括名称，网络，带宽等信息3)包含在会话中的媒体信息，包括：  媒体类型(video, audio, etc)  传输协议(RTP/UDP/IP, H.320, etc)  媒体格式(H.261 video, MPEG video, etc)  多播或远端（单播）地址和端口4)为接收媒体而需的信息5)使用的带宽信息6)可信赖的接洽信息

如果拓展，还可以描述会话的安全方案信息、服务质量信息等，其中WebRTC就在SDP的基础上进行了继续拓展，可以参考：

https://www.ietf.org/archive/id/draft-nandakumar-rtcweb-sdp-08.txt。

#### 4.2、SDP格式：

标准的SDP规范主要包括SDP描述格式和SDP结构，其中SDP结构里面最重要的两项内容是会话描述信息和媒体描述信息。

说了这么多，先上个SDP的示例，有个SDP的直观认识：

```
v=0
o=- 340982118323072764 2 IN IP4 127.0.0.1
...
m=audio 9 UDP/TLS/RTP/SAVPF 111 103 104 9 0 8 106 105 13 110 112 113 126
...
a=rtpmap:111 opus/48000/2
a=rtpmap:103 ISAC/16000
a=rtpmap:104 ISAC/32000
...

```

SDP是由多个<type>=<value>这样的表达式组成，其中是基于文本描述，这样做的好处是便于问题排查和调试，同时type是一个字符，value是一个字符串，=两边没有空格。

整个SDP 由这样一行行的key-value字符串组成，同时整个字符串组成了会话信息描述和多个媒体级别描述。至于会话信息和媒体信息描述的key和value到底怎么定义需要继续分析下各个字段，其中有些字段是必须的有些是可选项。

会话描述和媒体描述，一般会话级描述从v=开始一直到第一个媒体描述为止，媒体描述是从m=开始一直到下一个媒体描述m=的位置之前。也就是说SDP里面一般先从会话信息v=开始，然后后面跟几个m=的媒体描述组成。

会话级的作用域是整个会话，其位置从v=开始到第一个媒体描述m=为止；

媒体级描述 是对单个媒体流即音频流、视频流和字幕流等的单个媒体描述，如果有多个流则用多组媒体级描述。其中每个媒体级描述就是从m=开始到下一个媒体描述m=为止。

 ![img](.\pic_resource\SDP_note\sdp_into.png)

 

#### 4.3、SDP结构：

上面了解了SDP的基本信息，下面看下各个字段含义，当然字段非常多，只看一些常用和必须的，对于有些场景下的字段你需要参看SDP的RFC4566文档进一步了解，同时了解下各个行业的标准对这一块的规定，后面示例分析会介绍完整的应用字段解释。

##### 4.3.1、会话描述部分：

v=

```
v就是protocol version，必选字段，表示了SDP的版本号但是不包含子版本号，一般就是v=0。
```

o=

```
o是owner，必选字段，对会话发起者的一个信息描述，其中包含了用户名、会话ID、网络地址等信息。格式是：
o=<username> <session id> <version> <network  type> <address >
```

s=

```
s即session name，表示的会话名称，必选字段。在整个SDP里面只有一个会话名称，有且仅有一个这样的字段。
```

t=

```
t即time the session is active，必选字段，表示的是该会话的开始到结束时间，如果是持久会话，则时间值填0，这样的时间是NTP时间，单位是秒，格式是 ：
t=<start time> <stop time>。
```

##### 4.3.2、媒体描述部分：

这部分字段非常多，也重点介绍几个，其余字段用到需要参考RFC文档和后面的示例分析：

m=

```
m就是media name and transport address，可选字段，但是一般音视频会话至少有音频流或者视频流，所以一般也是都会有的，如果多个流则有多个，一般就是从这个m=到下个媒体会话m=结束，格式：
m=<media type> <port> <transport> <fmt list>

<media tyoe>：媒体类型， audio或者video
<port>：媒体端口，要么是收流端口，要么是发流端口，这样我们就知道从哪个端口进行发流和收流。
<transport>：传输协议，表示码流的传输协议是什么，如果UDP则为RTP/AVP代表码流用RTP Over UDP，要么是TCP/RTP/AVP即RTP Over TCP，表示用TCP传输RTP码流。
<fmt list>:媒体格式，这里表示的负载类型，一般表示的视频的H.264 H.265等,音频则是 G7xx、AAC、Opus等类型。
```

a=

```
a是attribute，可选字段，表示的媒体的属性，进一步的描述媒体信息，可以有多个属性，其中比较重要的属性就是rtpmap和fmtp，格式是：

a=<type>
a=<type>:<value>

rtpmap属性，表示媒体流传输协议的RTP具体内容：
a=rtpmap:<payload type> <encoding name>/<clock rate>[/encoding parameters]

rtpmap:是rtp map即RTP参数映射表
<payload type>：负载类型，对应表示RTP包中的音视频数据负载类型，比如RTP的数据类型是H.264，那么这里就是96。
<encoding name>:编码器名称，这里主要指的RTP承载音视频编码数据类型，当然可以是标准数据也可以私有数据，如VP8 VP9 H.264等。
<sample rate>:采样率，音视频里面都有时间戳的概念，所以这里表示的音视频的采样率，对音视频同步非常重要。比如视频的90000，音频的8000、48000等。
<encoding parameters>：编码参数，可以表示视频的分辨率，帧率，音频的单声道双声道等信息。

fmtp属性，在rtpmap基础上进一步描述音视频具体编码参数信息：
格式：
a=fmtp:<payload type> <format specific parameters>

fmtp，格式参数，即 format  parameters；
<payload type>：负载类型，同样对应 RTP 包中的音视频数据负载类型；
< format specific parameters>：指具体参数，或者说对音视频编码信息的一次处理。该信息从编码器得到，比如视频的SPS\PPS等，用于解码端的播放器初始化。
```

SDP的字段非常多，在不同场景下约束不同，下面看下在RTSP、国标SIP协议、WebRTC中的具体示例。

#### 4.4、示例分析：

##### 4.4.1、RTSP中的SDP：

RTSP即Real Transport Stream Protocol实时流媒体传输协议，一般和RTP、RTCP搭配使用，该协议用来进行媒体的控制和会话的建立，比如开始、暂停、倍速控制媒体文件的播放，RTP协议用来进行码流的传输，RTCP保障服务的Qos质量。该协议的应用场景在视频监控最多，一般的视频监控产品如摄像机、NVR等都原生支持RTSP协议，同时该协议在一些智能家居方面如智能音箱也有所使用，比如AWS Alexa在进行视频投屏时就支持该协议。

这里只探讨下RTSP协议的创建媒体会话时，用SDP交互会话信息时的情况，顺便给大家一个测试地址，然后用VLC播放视频抓包就可以学习RTSP、RTP协议，RTSP协议默认端口554，测试地址：

rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov

这是抓包在DESCRIBE信令的SDP信息：

![img](.\pic_resource\SDP_note\rtsp_sdp_info.png)

###### 会话描述协议：

v=0

```
解释：版本号，一般默认是0；
```

o=- 1422341101 1422341101 IN IP4 3.84.6.190

```
解释：会话发起者信息会话名称 网络类型 IP地址等信息；
```

s=BigBuckBunny_115k.mov

```
解释：会话名称
```

c=IN IP4 3.84.6.190

```
解释：格式：c=<networktype> <address type> <connection address>

链接信息，包含网络类型和IP地址等信息；
```

a=range:npt=0- 596.48

```
解释：用来表示媒体流的长度为596.48秒
```

###### 音频媒体描述信息：

m=audio 0 RTP/AVP 96

```
解释：表示该路会话的的audio是通过RTP来格式传送的，其payload值为96但是传送的端口还没有定。
```

a=rtpmap:96 mpeg4-generic/12000/2

```
解释：rtpmap的信息，表示音频为AAC的其sample采样率为12000双通道音频，其中mpeg4-generic代表了AAC的互联网媒体类型。
```

a=fmtp:96 profile-level-id=1;mode=AAC-hbr;sizelength=13;indexlength=3;indexdeltalength=3;config=1490

```
解释：这里面是AAC的详细编码和封装信息：
profile-level-id=1;mode=AAC-hbr;
profile中指定1表示低复杂度类型，mode=AAC-hbr代表编码模式；
sizelength=13;indexlength=3;indexdeltalength=3
涉及到AAC的AU Header,如果一个RTP包则只包含一个AAC包，则没有这三个字段,有则说明是含有AU Header字段，具体参考RTP对音频AAC的封装。
AU-size由sizeLength决定表示本段音频数据占用的字节数
AU-Index由indexLength决定表示本段的序号, 通常0开始
AU-Index-delta由indexdeltaLength 决定表示本段序号与上一段序号的差值

config=1490
十六进制：1490
二进制：0001 0100 1001 0000
十进制：2、9、2
分别代表aac的profile是2、9代表采样率是12000，通道个数是2立体声，具体参考AAC的ADTS头定义。
```

a=control:trackID=1

```
解释：通过媒体流1来发送音频；
```

 

###### 视频媒体描述信息：

m=video 0 RTP/AVP 97

```
解释：表示该路会话的的audio是通过RTP来格式传送的，其payload值为97但是传送的端口还没有定。
```

a=rtpmap:97 H264/90000

```
解释：表示该路会话的的Video是通过RTP来格式传送的，其payload值为97,编码器是H264，采样率90000。
```

a=fmtp:97 packetization-mode=1;profile-level-id=42C01E;sprop-parameter-sets=Z0LAHtkDxWhAAAADAEAAAAwDxYuS,aMuMsg==

```
解释：这里面包含一些RTP封包模式，视频质量等级，视频的SPS、PPS等信息。

表示该路会话的的audio是通过RTP来格式传送的，其payload值为97。
当 packetization-mode 的值为 1 时RTP打包H.264的NALU单元必须使用非交错(non-interleaved)封包模式.
当 profile-level-id的值为 42C01E 时， 第一个字节0x42表示 H.264 的 profile_idc类型Baseline profile , 第二字节代表profile_iop,各个Bit代表视频序列遵循的条款，第三个字节表示 H.264 的 Profile 级别，0x1E即30代表了levle_idc为3，即30/3，具体信息参考H.264的SPS PPS即可。
sprop-parameter-sets是SPS和PPS的的Base64之后的字符串，中间以逗号分割，后面会专门写篇文章介绍下，主要描述了编码器的参数信息，对初始化播放器有帮助；
```



a=cliprect:0,0,160,240

```
解释：一些offer和answer的加密属性；
```

a=framesize:97 240-160

```
解释：RTP负载类型97，帧宽和高分别为240*160
```

a=framerate:24.0

```
解释：最大帧率速度为24帧/s
```

a=control:trackID=2

```
解释：通过媒体流1来发送视频；
```

##### 4.4.2、基于SIP协议国标GB28181中的SDP：

国标协议也是基于SIP协议开发的，所以这里的SDP协议是在给前端设备下发INVITE信令的回复中带上来的，这里的SDP主要是为了不同的厂家，使用 GB 对接的时候，上级要能正常看下级推送过来的摄像头的视频，回放，以及球机控制等等的功能。

现在看一个抓包文件中INVITE回复携带的SDP描述信息：

![img](E:\document\doc\doc_pic\gb28181_sdp_info)

###### 会话信息描述国标的规定：

v=0

```
v字段给出了 SDP 的版本，当前规范版本是 0，这个版本没有小号版本。
```

o=

```
源（客户端的SIP编号）<用户名><会话 ID><会话版本><网络类型><地址类型><单播地址>

如 32028100001320000001 0 0 IN IPV4 192.168.0.101
各个字段解释：
<用户名> 用户登录的源主机名字，如果不能提供则用"-"表示，用户名不能包含空格。这里一般是摄像机的国标 ID
<会话 ID> 是一个字符串，<用户名><会话 ID><网络类型><单播地址>这个组合形成该会话的唯一标识。用 0 标识的居多
<会话版本> 会话版本号，推荐使用 NTP 时间戳。用 0 标识的居多
<网络类型> 目前是 IN 代表 internet，未来可能会有其他值。
<地址类型> 目前只有 IPV4 和 IPV6 两种，目前主要是 IPV4,。
<单播地址> 创建会话的主机地址。一般为媒体服务器的地址。

注意：有时候处于某种原因，用户名和 IP 不想明确表示，只要保证 o 字段全球唯一，用户名和 IP 可以随机。
```

s=

```
请求媒体流的操作类型，play 代表实况；playback 代表回放。download 代表下载，Talk 代表语音。
```

u=

```
行应填写视音频文件的 URI。 该 URI 取值有两种方式: 简捷方式和普通方式。
简捷方式常用 摄像机 ID：其他参数格式。如 32028100001320000001:10111
普通方式采用 http://存储设备 ID[/文件夹]* /文件名, [/文件夹]* 为 0-N 级文件夹。
简捷方式中冒号后面文件类型，如果s=playback时，则0有时代表的全天录像，1代表事件录像等，一般默认为3.有些海康平台这里进行了区分，如果值填错会导致回放录像失败。
```

c=

```
<网络类型><地址裂类型><链接地址>
如 IN IPV4 192.168.0.100  
```

t=

```
t字段在回放和下载时，t 行值为开始时间和结束时间。使用的时间为 UNIX 时间戳，需要用 UNIX 时间戳转为北京时间。如果是直播则是0.
```

###### 媒体信息描述国标的规定：

m=

```
m 字段描述媒体类型，媒体端口，媒体协议，以及媒体负载方式

例：
m=video 6000 RTP/AVP 96------媒体类型视频或视音频，传输端口 6000，RTP over UDP，负载类型 96
m=video 6000 TCP/RTP/AVP 96------媒体类型视频或视音频，传输端口 6000，RTP over TCP，负载类型 96
m=audio 6000 RTP/AVP 8------媒体类型为音频，传输端口 6000，RTP over UDP，负载类型 8
```

a=

```
a=rtpmap: <payload type> <encoding name>/<clockrate> [/<encoding parameters> ]中的<encoding name>这里参考RTSP分析即可，要说的一点是这里可以携带设备厂商的编码类型，如果发现这里不是标准的，则解码和播放一般都存在问题；
a=downloadspeed: 下载倍速(取值为整型) ；
a=filesize: 文件大小(单位:Byte) , a 字段可携带文件大小参数, 用于下载时的进度计算。

下面可以参考IETF RFC4571的规定，解析setup connection recvonly等属性：
a=setup:TCP 连接方式(表示本 SDP 发送者在 RTP over TCP 连接建立时是主动还是被动发起 TCP 连接, “active”为主动, “passive”为被动)
a=connection:new (表示采用 RTP over TCP 传输时新建或重用原来的 TCP 连接, 可固定采用新建 TCP 连接的方式)
a=recvonly 只接受（收流端）只用于媒体，不用于控制协议
a=sendonly只发送（发流端）只用于媒体，不用于控制协议
y 字段：由 10 位十进制整数组成的字符串，表示 SSRC 值
第一位为 0 代表实况，为 1 则代表回放， 第二位至第六位由监控域 ID 的第 4 位到第 8 位组成,最后 4 位为不重复的 4 个整数 ；
```

有了以上基础分析国标SIP中的SDP信息就非常简单了，不再赘述。

##### 4.4.3、WebRTC中的SDP：

WebRTC中的SDP信息比较关键，是分析代码流程和驱动整个业务运转起来的关键，同时WebRTC规范也对SDP的RFC4566规范进行了进一步的规范，也已经成为SDP草案，具体参考：https://www.ietf.org/archive/id/draft-nandakumar-rtcweb-sdp-08.txt

其中SDP包含了下面几个板块的内容，比RTSP和SIP中的更丰富更强大：

其中会话描述、网络描述、媒体流描述和SDP的RFC4566规范是一致的，同时增加了安全描述和服务质量QOS描述，我们进行了P2P抓包：

![img](.\pic_resource\SDP_note\webrtc_sdp_base_info.png)

![img](.\pic_resource\SDP_note\webrtc_sdp_pos_info.png)

WebRTC中的SDP 是由一个会话层和多个媒体层组成的， 而对于每个媒体层，WebRTC 又将其细划为四部分，即媒体流、网络描述、安全描述和服务质量描述。其中，安全描述起到两方面的作用，一方面是进行网络连通性检测时，对用户身份进行认证；另一方面是收发数据时，对用户身份的认证，以免受到对方的攻击；服务质量描述指明启动哪些功能以保证音视频的质量，如启动带宽评估，当用户发送数据量太大超过评估的带宽时，要及时减少数据包的发送；启动防拥塞功能，当预测到要发生拥塞时，通过降低流量的方式防止拥塞的发生等等，这些都属于服务质量描述的范畴。

总结起来就是，SDP 是由一个会话层与多个媒体层组成，每个媒体层又分为媒体流描述、网络描述、安全描述和服务质量描述，而每种描述下面又需要你参考草案来解析和理解。

#### 4.5、总结：

这篇文章主要介绍了下SDP协议的内容、格式和规范，以及通过RTSP、SIP、WebRTC中三个例子分析了下SDP中各个字段和应用。平时学习时有这个整体框架就行，一些文中没出现的字段需要你查标准的RFC文档进行学习和理解。

同时在GB28181协议中，由于各个厂家对有些字段理解不规范，导致有歧义经常会出现连接摄像头失败，拉流失败等问题，需要在实践中解决和兼容。

from:https://mp.weixin.qq.com/s?__biz=MzI0NTMxMjA1MQ==&mid=2247484139&idx=1&sn=6759a1e205bbd1026c1effb79e9e06ba&chksm=e9513db7de26b4a1ae6f2bb0d7906e2424350cbcf55a881bb6c527902e6bb06d3e7db0ba9e87&scene=21#wechat_redirect





### 参考：

[SDP协议 - 简书](https://www.jianshu.com/p/94b118b8fd97)

[SDP在RTSP、国标GB28181、WebRTC中的实践 - DoubleLi - 博客园](https://www.cnblogs.com/lidabo/p/13361549.html)



