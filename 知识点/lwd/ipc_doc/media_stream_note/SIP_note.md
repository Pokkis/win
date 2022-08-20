### 1、SIP 协议简介：

SIP（Session Initiation Protocol，会话发起协议）是由IETF提出的 IP 电话信令协议.它的主要目的是为了解决 IP 网中的信令控制，以及同软交换的通信，从而构成下一代的增值业务平台，对电信、银行、金融等行业提供更好的增值业务。SIP协议是一种基于文本的类似于 HTTP 的协议，报文结构与 HTTP 也一样，即包括start-line，head，body。

```
start-line
message-header
CRLF
[ message-body ]
```

### 2、消息格式

#### 2.1、首行（start-line）

分请求行和状态行。

**请求行:** 由请求类型、请求目的地址和协议版本号构成。请求类型有：INVITE,ACK,OPTIONS,BYE,CANCEL 和 REGISTER。

**状态行:** 是被叫方向主叫方返回的状态信息，如1xx，2xx，3xx，4xx，5xx，6xx。

**请求类型：**

- **INVITE**：用于发起呼叫请求。INVITE消息包括消息头和数据区两部分。INVITE 消息头包含主、被呼叫的地址，呼叫主题和呼叫优先级等信息。数据区则是关于会话媒体的信息，可由会话描述协议SDP 来实现。
- **BYE**：当一个用户决定中止会话时，可以使用BYE 来结束会话。
- **OPTIONS**：用于询问被叫端的能力信息，但 OPTIONS 本身并不能发起呼叫。
- **ACK：** 对已收到的消息进行确认应答。
- **REGISTER**：用于用户向 SIP 服务器传送位置信息或地址信息。
- **CANCEL**：取消当前的请求，但它并不能中止已经建立的连接。

**状态类型：**

- **1xx**：临时消息：表示请求消息已经收到，后面将继续处理该请求。
- **2xx**：成功消息：表示请求已经被成功的理解、接受或执行。
- **3xx**：重定向消息：表示为了完成请求还需采取更进一步的动作。
- **4xx**：客户机错误：表示该请求含有语法错误或在这个服务器上不能被满足。
- **5xx**：服务器错误：表示该服务器不能处理一个明显有效的请求。
- **6xx**：全局性故障：表示该请求在任何服务器上都不能被实现。

常用的状态码举例：

| 状态码 | msg                     | 含义         |
| :----- | :---------------------- | :----------- |
| 100    | Trying                  | 试呼叫       |
| 180    | Ringing                 | 振铃         |
| 181    | Call is Being Forwarded | 呼叫正在前转 |
| 200    | OK                      | 成功响应     |
| 302    | Moved Temporarily       | 临时迁移     |
| 400    | Bad Request             | 错误请求     |
| 401    | Unauthorized            | 未授权       |
| 403    | Forbidden               | 禁止         |
| 404    | Not Found               | 用户不存在   |
| 408    | Request Timeout         | 请求超时     |
| 480    | Temporarily Unavailable | 暂时无人接听 |
| 486    | Busy Here               | 线路忙       |
| 504    | Server Time-out         | 服务器超时   |
| 600    | Busy Everywhere         | 全忙         |

#### 2.2、消息头（message-header）

发送`MESSAGE`消息给`user2`

```http
MESSAGE sip:user2@domain.com SIP/2.0
Via: SIP/2.0/TCP user1pc.domain.com;branch=z9hG4bK776sgdkse
Max-Forwards: 70
From: sip:user1@domain.com;tag=49583
To: sip:user2@domain.com
Call-ID: asd88asd77a@1.2.3.4
CSeq: 1 MESSAGE
Content-Type: text/plain
Content-Length: 18

user2, come here.
```

Header 字段含义说明：

| Header         | 含义说明                                                     | 举例                                                         |
| :------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| Via            | Via头域是被服务器插入request中，用来检查路由环的，并且可以使response根据via找到返回的路。为响应消息提供传输路径，当请求消息经过每一跳节点时，每一跳节点都把自身的IP地址信息放入顶层Via中。响应消息则沿着请求消息记录下的传输路径反向传输，首先移走指明自身IP地址信息的顶层消息头。 | Via: SIP/2.0/TCP user1pc.domain.com;branch=z9hG4bK776sgdkse  |
| Max-Forwards   | 用于表示这个包最多可以传送多少跳，每经过一跳都会减一当Max-Forwards==0系统会返回483。默认为70 | Max-Forwards: 70                                             |
| From           | 表示请求的发起者，给出标识会话发起者的URI，tag是必需的。     | From: sip:user1@domain.com;tag=49583                         |
| To             | 表示请求的接收者，`TO: 显示名<接收者URI>;tag=n`,显示名和tag可选。接收者URI是SIP网络中唯一标识接收终端的标识符。 | To: sip:user2@domain.com                                     |
| Call-ID        | 用于全局唯一标识正在建立的会话的标识符。 随机数加UAC标识信息，由本地设备(Client)生成，全局唯一，每次呼叫这个值唯一不变 | Call-ID: asd88asd77a@1.2.3.4                                 |
| Contact        | 包含源的URI信息，用来给响应方直接和源建立连接用              | Contact: [sip:192.168.100.1:1111](sip:192.168.100.1:1111)    |
| CSeq           | 用于标识同一会话中不同事务的序号，通常由一个用作序号的整型数和消息类型组成。整个会话操作过程由不同的事务组成，每一事务所涉及的消息的CSeq序号必须相同。 | CSeq: 1 MESSAGE                                              |
| Content-Type   | 指明消息体的类型 (SDP会话描述协议)                           | Content-Type: text/plain；Content-Type: application/sdp; Content-Type: application/cpim; |
| Content-Length | 指明消息体的字节大小                                         | Content-Length: 18                                           |
| Expires        | 有效期限                                                     | Expires: 7200                                                |

#### 2.3、消息体 （message-body）

SIP协议一个最主要的作用就是协商媒体信息。媒体信息通过message-body携带，基于SDP会话描述协议。

对于PSTN语音编码格式，主要有G711A、G711U、G729等。

SIP协商中主叫方会带上自己支持的所有音频编码列表到被叫方，被叫方一般在回铃时从主叫支持的类型中选出一种或多种自己支持的编码，返回主叫后，双人按顺序选出第一个支持的编码。

流程举例

![img](.\pic_resource\SIP_note\sip_regester.png)

（1） UAC发起register请求到UAS

（2） UAS如果需要验证账号信息，响应一个401 Unauthorized，其中包含用于密码加密的随机字符串

（3） UAC收到401后携带自己的账号信息（其中密码经过md5加密）重新发起一个register请求

（4） 服务器校验通过后返回200OK

（5） UAC发起invite请求，其中携带了主被叫等信息（From、To头域等）

（6） UAS一般会马上响应100trying告诉UAC正在尝试呼叫被叫

（7） 被叫响应后UAS返回180到UAC，其中可能携带彩铃媒体信息到UAC

（8） 被叫接听后返回200给UAC，其中可能携带会话媒体信息到UAC

（9） UAC返回ack到UAS确认收到200



### 3、Sip消息的两种会话模式

在Sip IM通信应用过程中，一般存在着两种会话模式：

- Session Model
- Pager Model

#### 3.1、Session Model

会话中，对于消息体内容`大于1300字节`时，一般采用`Session Model`。其会话建立过程如下图所示：

![ SIP协议Session Model](.\pic_resource\SIP_note\sip_session_model.png)

主叫方A呼叫被叫方B：

- 步骤1：`主叫方A`发送`INVITE`请求到`代理服务器`；
- 步骤2：`代理服务器`发送100 Trying 响应`主叫方A`；
- 步骤3~6：`代理服务器`搜索`被叫方B`的地址，获取地址后转发INVITE请求；
- 步骤7~9：`被叫方B`生成的180 振铃响应，返回给`主叫方A`；
- 步骤10~12：`被叫方B`生成的200 OK响应，返回给`主叫方A`；
- 步骤13~17：`主叫方A`收到`被叫方B`200 OK响应后，向`被叫方B`发送一个ACK，会话建立；
- 步骤18~20：会话结束后，任何参与者（A或B）都可以发送一个BYE请求来终止会话；
- 步骤21~23：`主叫方A`发送200 OK响应来确认BYE，会话终止。

注：`以上的整个流程称之为一个Dialog`

#### 3.2、Pager Model

在Sip消息中，对于消息体`不大于1300字节`时，一般采用`Pager Model`。Sip消息通信中采用`MESSAGE`方法，`MESSAGE`方法本身并`不建立Dialog`，在多数应用中，每条IM消息都是独立的，颇似分页消息。

##### 3.2.1 MESSAGE方法的由来

`RFC3428`对Sip协议进行了扩展，在Sip协议中增加了`MESSAGE`请求方法。采用Pager Model进行通信，传递不超过1300字节的数据。MESSAGE方法详细可参考 “SIP-RFC3428” https://tools.ietf.org/html/rfc3428 。

##### 3.2.2 MESSAGE方法消息体

当User1想给User2发送IM消息时，只需构造一个MESSAGE，发出去即可。
对于其消息体`body`可以是任何`MIME`格式。但必须支持`plain/text`格式，可以选择支持`message/cpim`、`message/sdp`格式，可能用`message/cpim`会好一点，因为已有的IM系统标准是`message/cpim`格式。

*注：想了解CPIM消息格式的同学可参考：*[CPIM 消息格式：](https://xiaxl.blog.csdn.net/article/details/104718006)https://xiaxl.blog.csdn.net/article/details/104718006
*注：想了解SDP消息格式的同学可参考：*[SDP 消息格式：](https://xiaxl.blog.csdn.net/article/details/104723834)https://xiaxl.blog.csdn.net/article/details/104723834

##### 3.2.3 Pager Model请求流程如下

以User1向User2发送MESSAGE消息为例：

![Pager Model](.\pic_resource\SIP_note\sip_pager_model.png)

- 步骤1：`User1`发送`MESSAGE`请求到`代理服务器`；
- 步骤2：`代理服务器`转发`User1`的MESSAGE请求给`USER2`；
- 步骤3：`User2`收到`User1`的消息后，回复200 OK给`代理服务器`；
- 步骤7~9：`代理服务器`转发200 OK回复给`User1`

### 4、重要头域解释

对于UAC或UAS来说，Sip协议是一种有状态的协议，同一个会话的前后相关的信令必须送到相同的UAC或UAS。Sip协议的via、record-route头域正是用于用来指示路由的字段。

其中几个相关字段说明如下：

via用于指示response返回的路径，不会影响后续request。

record-route用于指示同一callid后续请求的路径。

rport是接收方收到via为内网的请求时，加上 自己看到的发送方的ip（received）和端口（rport），用于NAT。

[RFC3581——SIP中的rport机制](https://www.cnblogs.com/xiaOt119/archive/2012/06/11/2545136.html)

### 5、Sip测试工具

SIPp是sip协议栈的一个免费开源的测试工具, 它模拟了一些基本的uac和uas功能，并建立和释放多个调用的INVITE和BYE的方法，可以读取xml和csv文件生成注册或呼叫流程，可以动态显示统计信息和动态调整呼叫速率。

模拟UAS：./sipp -sn uas -i 192.168.1.249 -p 5077 //监听5077端口

模拟UAC:./sipp -sn uac 192.168.1.249:5077 -i 192.168.1.249 -p 5078 -r 50 -rp 1000  //每1000ms发送50个呼叫

SIPp还支持使用脚本文件来自定义模拟的UAC或UAS的行为，具体可参考官网文档：http://sipp.sourceforge.net/index.html

### 6、协议开发

常见协议栈有：

1、pjsip：http://www.pjsip.org/

2、sofia：http://sofia-sip.sourceforge.net/

3、osip：https://www.gnu.org/software/osip/

### 7、SIP消息举例

这里举两个例子：

- MESSAGE消息（Pager Mode）
- REGISTER消息

#### 7.1、MESSAGE消息（Pager Model）

以`User1`发送`MESSAGE`消息给`user2`为例：

![Pager Model](.\pic_resource\SIP_note\sip_message.png)

##### 步骤1：`User1`发送`MESSAGE`请求到`代理服务器`

```http
MESSAGE sip:user2@domain.com SIP/2.0

Via: SIP/2.0/TCP user1pc.domain.com;branch=z9hG4bK776sgdkse
Max-Forwards: 70
From: sip:user1@domain.com;tag=49583
To: sip:user2@domain.com
Call-ID: asd88asd77a@1.2.3.4
CSeq: 1 MESSAGE
Content-Type: text/plain
Content-Length: 18

user2, come here.
```

##### 步骤2：`代理服务器`转发`User1`的MESSAGE请求给`USER2`

`代理服务器`收到`步骤1`请求，到数据库中查找`User2`（注册过程中生成数据库），随后生成`步骤2`的数据。

```http
MESSAGE sip:user2@domain.com SIP/2.0
Via: SIP/2.0/TCP proxy.domain.com;branch=z9hG4bK123dsghds
Via: SIP/2.0/TCP user1pc.domain.com;branch=z9hG4bK776sgdkse;
                                              received=1.2.3.4
Max-Forwards: 69
From: sip:user1@domain.com;tag=49394
To: sip:user2@domain.com
Call-ID: asd88asd77a@1.2.3.4
CSeq: 1 MESSAGE
Content-Type: text/plain
Content-Length: 18

user2, come here.
```

##### 步骤3：`User2`收到`User1`的消息后，回复200 OK给`代理服务器`

直接回应（200-OK） 没有Body，也不携带Contact头域

```http
SIP/2.0 200 OK
Via: SIP/2.0/TCP proxy.domain.com;branch=z9hG4bK123dsghds;
                                            received=192.0.2.1
Via: SIP/2.0/TCP user1pc.domain.com;;branch=z9hG4bK776sgdkse;
                                               received=1.2.3.4
From: sip:user1@domain.com;tag=49394
To: sip:user2@domain.com;tag=ab8asdasd9
Call-ID: asd88asd77a@1.2.3.4
CSeq: 1 MESSAGE
Content-Length: 0
```

##### 步骤4：`代理服务器`转发200 OK回复给`User1`

```
代理服务器`收到回复后，去掉最顶端的`Via`，转发如下消息给`User1
SIP/2.0 200 OK

Via: SIP/2.0/TCP user1pc.domain.com;branch=z9hG4bK776sgdkse;
                                              received=1.2.3.4
From: sip:user1@domain.com;;tag=49394
To: sip:user2@domain.com;tag=ab8asdasd9
Call-ID: asd88asd77a@1.2.3.4
CSeq: 1 MESSAGE
Content-Length: 0
```

#### 7.2、REGISTER消息

首先举例一个非鉴权注册消息。

7.2.1、非鉴权注册消息

`192.168.2.161`机器发送注册消息给`192.168.2.89`服务器：

```http
REGISTER sip:192.168.2.89 SIP/2.0

Via: SIP/2.0/UDP 192.168.2.161:10586
Max-Forwards: 70
From: <sip:01062237496@192.168.2.89>;tag=ca04c1391af3429491f2c4dfbe5e1b2e;epid=4f2e395931
To: <sip:01062237496@192.168.2.89>
Call-ID: da56b0fab5c54398b16c0d9f9c0ffcf2@192.168.2.161
CSeq: 1 REGISTER
Contact: <sip:192.168.2.161:10586>;methods="INVITE, MESSAGE, INFO, SUBSCRIBE, OPTIONS, BYE, CANCEL, NOTIFY, ACK, REFER"
User-Agent: RTC/1.2.4949 (BOL SIP Phone 1005)
Event: registration
Allow-Events: presence
Content-Length: 0
```

当注册成功（回送200 OK）时，服务器发送的res消息参考如下：

```http
SIP/2.0 200 OK

Via: SIP/2.0/UDP 192.168.2.161:10586
From: <sip:01062237496@192.168.2.89>;tag=ca04c1391af3429491f2c4dfbe5e1b2e;epid=4f2e395931
To: <sip:01062237496@192.168.2.89>;tag=-00834-14d0805b62bc026d
Call-ID: da56b0fab5c54398b16c0d9f9c0ffcf2@192.168.2.161
CSeq: 1 REGISTER
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,REGISTER,INFO,UPDATE,PRACK,REFER,SUBSCRIBE,NOTIFY,MESSAGE
Contact: sip:192.168.2.161:10586
Content-Length: 0
Expires: 3600
```

#### 7.2.2、鉴权注册消息

当需要鉴权注册时

- 请求端192.168.2.161发送注册消息给192.168.2.89服务器
- 服务器对192.168.2.161发送“401 Unauthorized”信息给请求端，提示请求端需要带上鉴权信息重新注册
- 请求端带上鉴权信息后（带有“Authorization”头字段）重新向服务器注册
- 服务器验证鉴权头的正确性，如果鉴权成功，给请求端发送200 OK消息。若失败，继续发送401消息。

请求端`192.168.2.161`发送注册消息给`192.168.2.89`服务器

```http
REGISTER sip:192.168.2.89 SIP/2.0

Via: SIP/2.0/UDP 192.168.2.161:8021
Max-Forwards: 70
From: <sip:01062237493@192.168.2.89>;tag=efca469543ce4788a6a6a2c7b66cd01f;epid=de4504430d
To: <sip:01062237493@192.168.2.89>
Call-ID: c88a247a74b54a8c9e676bdde3bba6c9@192.168.2.161
CSeq: 1 REGISTER
Contact: <sip:192.168.2.161:8021>;methods="INVITE, MESSAGE, INFO, SUBSCRIBE, OPTIONS, BYE, CANCEL, NOTIFY, ACK, REFER"
User-Agent: RTC/1.2.4949 (BOL SIP Phone 1005)
Event: registration
Allow-Events: presence
Content-Length: 0
```

服务器对`192.168.2.161`发送`401 Unauthorized`信息给请求端，提示请求端需要带上鉴权信息重新注册：

```http
SIP/2.0 401 Unauthorized

Via: SIP/2.0/UDP 192.168.2.161:8021
From: <sip:01062237493@192.168.2.89>;tag=efca469543ce4788a6a6a2c7b66cd01f;epid=de4504430d
To: <sip:01062237493@192.168.2.89>;tag=-001893-38ba013ba3dde36e
Call-ID: c88a247a74b54a8c9e676bdde3bba6c9@192.168.2.161
CSeq: 1 REGISTER
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,REGISTER,INFO,UPDATE,PRACK,REFER,SUBSCRIBE,NOTIFY,MESSAGE
Contact: <sip:192.168.2.89:14010>
Content-Length: 0
WWW-Authenticate: Digest realm="192.168.2.89", qop="auth", nonce="e17d377c3d2d9c343e26576a7fd04738481dfc10", opaque="", stale=FALSE, algorithm=MD5
```

请求端`192.168.2.161`通过`Authorization`头字段带上鉴权头信息，发送一个新的REGISTER消息：

```http
REGISTER sip:192.168.2.89 SIP/2.0

Via: SIP/2.0/UDP 192.168.2.161:8021
Max-Forwards: 70
From: <sip:01062237493@192.168.2.89>;tag=efca469543ce4788a6a6a2c7b66cd01f;epid=de4504430d
To: <sip:01062237493@192.168.2.89>
Call-ID: c88a247a74b54a8c9e676bdde3bba6c9@192.168.2.161
CSeq: 2 REGISTER
Contact: <sip:192.168.2.161:8021>;methods="INVITE, MESSAGE, INFO, SUBSCRIBE, OPTIONS, BYE, CANCEL, NOTIFY, ACK, REFER"
User-Agent: RTC/1.2.4949 (BOL SIP Phone 1005)
Authorization: Digest username="01062237493", realm="192.168.2.89", qop=auth, algorithm=MD5, uri="sip:192.168.2.89", nonce="e17d377c3d2d9c343e26576a7fd04738481dfc10", nc=00000001, cnonce="12660455546344082314666316435946", response="f57e47ce03162293b9ced07362ce2b79"
Event: registration
Allow-Events: presence
Content-Length: 0
```

服务器验证鉴权头的正确性，如果鉴权成功，给请求端发送200 OK消息。若失败，继续发送401消息

```http
SIP/2.0 200 OK

Via: SIP/2.0/UDP 192.168.2.161:8021
From: <sip:01062237493@192.168.2.89>;tag=efca469543ce4788a6a6a2c7b66cd01f;epid=de4504430d
To: <sip:01062237493@192.168.2.89>;tag=-001894-a5eb977c8969aa51
Call-ID: c88a247a74b54a8c9e676bdde3bba6c9@192.168.2.161
CSeq: 2 REGISTER
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,REGISTER,INFO,UPDATE,PRACK,REFER,SUBSCRIBE,NOTIFY,MESSAGE
Contact: sip:192.168.2.161:8021
Content-Length: 0
Expires: 3600
```

### 8、SIP 示例：

#### 8.1、注册

```js
REGISTER sip:130909115229300920@10.64.49.44:7100 SIP/2.0
Via: SIP/2.0/UDP 10.64.49.218:7100;rport;branch=z9hG4bK4162288924
From: <sip:130909113319427420@10.64.49.218:7100>;tag=382068091
To: <sip:130909113319427420@10.64.49.218:7100>
Call-ID: 143225205
CSeq: 1 REGISTER
Contact: <sip:130909113319427420@10.64.49.218:7100>
Max-Forwards: 70
User-Agent: Hikvision
Expires: 7200
Content-Length: 0

SIP/2.0 401 Unauthorized
Via: SIP/2.0/UDP 10.64.49.218:7100;rport=7100;branch=z9hG4bK4162288924
From: <sip:130909113319427420@10.64.49.218:7100>;tag=382068091
To: <sip:130909113319427420@10.64.49.218:7100>;tag=916944766
Call-ID: 143225205
CSeq: 1 REGISTER
WWW-Authenticate: Digest realm="hik", nonce="a8afe6fcbee6331d89d3eb0d3d19ce39", opaque="a853e4f25298413f9bf3a9aa6767857d", algorithm=MD5
User-Agent: Hikvision
Expires: 7200
Content-Length: 0

REGISTER sip:130909115229300920@10.64.49.44:7100 SIP/2.0
Via: SIP/2.0/UDP 10.64.49.218:7100;rport;branch=z9hG4bK3997518011
From: <sip:130909113319427420@10.64.49.218:7100>;tag=382068091
To: <sip:130909113319427420@10.64.49.218:7100>
Call-ID: 143225205
CSeq: 2 REGISTER
Contact: <sip:130909113319427420@10.64.49.218:7100>
Authorization: Digest username="admin", realm="hik", nonce="a8afe6fcbee6331d89d3eb0d3d19ce39", uri="sip:130909115229300920@10.64.49.44:7100", response="907ddb1bcc25174d7de4a96c947fb066", algorithm=MD5, opaque="a853e4f25298413f9bf3a9aa6767857d"
Max-Forwards: 70
User-Agent: Hikvision
Expires: 7200
Content-Length: 0

SIP/2.0 200 OK
Via: SIP/2.0/UDP 10.64.49.218:7100;rport=7100;branch=z9hG4bK3997518011
From: <sip:130909113319427420@10.64.49.218:7100>;tag=382068091
To: <sip:130909113319427420@10.64.49.218:7100>;tag=705514612
Call-ID: 143225205
CSeq: 2 REGISTER
Contact: <sip:130909113319427420@10.64.49.218:7100>
User-Agent: Hikvision
Date: 2013-09-10T16:01:51
Content-Length: 0
```



```
REGISTER sip:34020000002000000001@192.168.1.81:5060 SIP/2.0
Via: SIP/2.0/UDP 192.168.1.93:5060;rport;branch=z9hG4bK-3d09000-23b5dfc-TFyCL5ME
From: <sip:64000000002000000001@192.168.1.93:5060>;tag=pck2i7pI
To: <sip:64000000002000000001@192.168.1.93:5060>
Call-ID: b4359b71-eb2f-b6f4-2fce-8fd47e5888ff@192.168.1.93
CSeq: 5 REGISTER
Contact: <sip:64000000002000000001@192.168.1.93:5060>
Authorization: Digest username="34020000002000000001", realm="3402000000", nonce="bd2e4df9e3d9b280", uri="sip:64000000002000000001@192.168.1.93:5060", response="9c8411f2b96c5aef55eb136ba3f34655", algorithm=MD5
Max-Forwards: 70
User-Agent: iVMS 1.0
Expires: 200
Content-Length: 0

SIP/2.0 401 Unauthorized
To: <sip:64000000002000000001@192.168.1.93:5060>;tag=67239569_53173353_7ce4590f-587d-4d44-9690-96498367c675
Via: SIP/2.0/UDP 192.168.1.93:5060;rport=5060;branch=z9hG4bK-3d09000-23b5dfc-TFyCL5ME;received=192.168.1.93
CSeq: 5 REGISTER
Call-ID: b4359b71-eb2f-b6f4-2fce-8fd47e5888ff@192.168.1.93
From: <sip:64000000002000000001@192.168.1.93:5060>;tag=pck2i7pI
WWW-Authenticate: Digest realm="3402000000",nonce="31ada55697307236"
Content-Length: 0

REGISTER sip:34020000002000000001@192.168.1.81:5060 SIP/2.0
Via: SIP/2.0/UDP 192.168.1.93:5060;rport;branch=z9hG4bK-3d09000-3068be5-lnl05y0C
From: <sip:64000000002000000001@192.168.1.93:5060>;tag=pck2i7pI
To: <sip:64000000002000000001@192.168.1.93:5060>
Call-ID: b4359b71-eb2f-b6f4-2fce-8fd47e5888ff@192.168.1.93
CSeq: 6 REGISTER
Contact: <sip:64000000002000000001@192.168.1.93:5060>
Authorization: Digest username="34020000002000000001", realm="3402000000", nonce="31ada55697307236", uri="sip:64000000002000000001@192.168.1.93:5060", response="3526a5a5dd91a45e19d1e6a65704457f", algorithm=MD5
Max-Forwards: 70
User-Agent: iVMS 1.0
Expires: 200
Content-Length: 0

SIP/2.0 200 OK
To: <sip:64000000002000000001@192.168.1.93:5060>;tag=86156704_53173353_d7fb589f-4357-4fae-ae32-47c38d81fff1
Via: SIP/2.0/UDP 192.168.1.93:5060;rport=5060;branch=z9hG4bK-3d09000-3068be5-lnl05y0C;received=192.168.1.93
CSeq: 6 REGISTER
Call-ID: b4359b71-eb2f-b6f4-2fce-8fd47e5888ff@192.168.1.93
From: <sip:64000000002000000001@192.168.1.93:5060>;tag=pck2i7pI
Contact: <sip:64000000002000000001@192.168.1.93:5060>
Expires: 200
Date: 2017-05-19T18:09:17.033
Content-Length: 0
```

#### 8.2、心跳

```
MESSAGE sip:34020000002000000001@192.168.1.81:5060 SIP/2.0
Via: SIP/2.0/UDP 192.168.1.93:5060;rport;branch=z9hG4bK-3d09000-2a2b21b-nddru7fy
From: <sip:64000000002000000001@192.168.1.93:5060>;tag=UL61Qycy
To: <sip:34020000002000000001@192.168.1.81:5060>
Call-ID: 5be796e4-b5ab-592e-cc13-64f34ea57528@192.168.1.93
CSeq: 35 MESSAGE
Contact: <sip:192.168.1.93:5060>
Content-Type: Application/MANSCDP+xml
Max-Forwards: 70
User-Agent: iVMS 1.0
Content-Length:   151

<?xml version="1.0"?>
<Notify>
<CmdType>Keepalive</CmdType>
<SN>26</SN>
<DeviceID>64000000002000000001</DeviceID>
<Status>OK</Status>
</Notify>
SIP/2.0 200 OK
To: <sip:34020000002000000001@192.168.1.81:5060>;tag=69112542_53173353_89bb4bc2-7f74-42a8-a102-6b6490b6daa2
Via: SIP/2.0/UDP 192.168.1.93:5060;rport=5060;branch=z9hG4bK-3d09000-2a2b21b-nddru7fy;received=192.168.1.93
CSeq: 35 MESSAGE
Call-ID: 5be796e4-b5ab-592e-cc13-64f34ea57528@192.168.1.93
From: <sip:64000000002000000001@192.168.1.93:5060>;tag=UL61Qycy
Content-Length: 0
```

#### 8.3、注销

```
REGISTER sip:34020000002000000001@192.168.1.81:5060 SIP/2.0
Via: SIP/2.0/UDP 192.168.1.93:5060;rport;branch=z9hG4bK-3d09000-23b5dfc-TFyCL5ME
From: <sip:64000000002000000001@192.168.1.93:5060>;tag=pck2i7pI
To: <sip:64000000002000000001@192.168.1.93:5060>
Call-ID: b4359b71-eb2f-b6f4-2fce-8fd47e5888ff@192.168.1.93
CSeq: 5 REGISTER
Contact: <sip:64000000002000000001@192.168.1.93:5060>
Authorization: Digest username="34020000002000000001", realm="3402000000", nonce="bd2e4df9e3d9b280", uri="sip:64000000002000000001@192.168.1.93:5060", response="9c8411f2b96c5aef55eb136ba3f34655", algorithm=MD5
Max-Forwards: 70
User-Agent: iVMS 1.0
Expires: 0
Content-Length: 0

SIP/2.0 401 Unauthorized
To: <sip:64000000002000000001@192.168.1.93:5060>;tag=67239569_53173353_7ce4590f-587d-4d44-9690-96498367c675
Via: SIP/2.0/UDP 192.168.1.93:5060;rport=5060;branch=z9hG4bK-3d09000-23b5dfc-TFyCL5ME;received=192.168.1.93
CSeq: 5 REGISTER
Call-ID: b4359b71-eb2f-b6f4-2fce-8fd47e5888ff@192.168.1.93
From: <sip:64000000002000000001@192.168.1.93:5060>;tag=pck2i7pI
WWW-Authenticate: Digest realm="3402000000",nonce="31ada55697307236"
Content-Length: 0

REGISTER sip:34020000002000000001@192.168.1.81:5060 SIP/2.0
Via: SIP/2.0/UDP 192.168.1.93:5060;rport;branch=z9hG4bK-3d09000-3068be5-lnl05y0C
From: <sip:64000000002000000001@192.168.1.93:5060>;tag=pck2i7pI
To: <sip:64000000002000000001@192.168.1.93:5060>
Call-ID: b4359b71-eb2f-b6f4-2fce-8fd47e5888ff@192.168.1.93
CSeq: 6 REGISTER
Contact: <sip:64000000002000000001@192.168.1.93:5060>
Authorization: Digest username="34020000002000000001", realm="3402000000", nonce="31ada55697307236", uri="sip:64000000002000000001@192.168.1.93:5060", response="3526a5a5dd91a45e19d1e6a65704457f", algorithm=MD5
Max-Forwards: 70
User-Agent: iVMS 1.0
Expires: 0
Content-Length: 0

SIP/2.0 200 OK
To: <sip:64000000002000000001@192.168.1.93:5060>;tag=86156704_53173353_d7fb589f-4357-4fae-ae32-47c38d81fff1
Via: SIP/2.0/UDP 192.168.1.93:5060;rport=5060;branch=z9hG4bK-3d09000-3068be5-lnl05y0C;received=192.168.1.93
CSeq: 6 REGISTER
Call-ID: b4359b71-eb2f-b6f4-2fce-8fd47e5888ff@192.168.1.93
From: <sip:64000000002000000001@192.168.1.93:5060>;tag=pck2i7pI
Contact: <sip:64000000002000000001@192.168.1.93:5060>
Expires: 0
Date: 2017-05-19T18:09:17.033
Content-Length: 0
```

### 9、参考：

https://cloud.tencent.com/developer/article/1183318

https://www.cnblogs.com/xiaxveliang/p/12434170.html

https://www.w3cschool.cn/session_initiation_protocol/request_and_response_header_fields.html

https://www.cnblogs.com/my_life/articles/2282364.html

https://www.cnblogs.com/lynchyo/p/3520500.html