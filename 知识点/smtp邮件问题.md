# smtp邮件

*问题：gmail587端口无法使用*

## 1、邮件端口区分

gmail:

25:明文传输

465：支持SSL/TLS

587：支持TLS/STARTTLS

## 2、ssl和tls联系

**传输层安全性协议**（英语：**T**ransport **L**ayer **S**ecurity，缩写：**TLS**）及其前身**安全套接层**（英语：**S**ecure **S**ockets **L**ayer，缩写：**SSL**）是一种[安全协议](https://zh.wikipedia.org/wiki/安全协议)，目的是为[互联网](https://zh.wikipedia.org/wiki/網際網路)通信提供安全及数据[完整性](https://zh.wikipedia.org/wiki/完整性)保障。

## 3、openssl

在[计算机网络](https://zh.wikipedia.org/wiki/计算机网络)上，**OpenSSL**是一个[开放源代码](https://zh.wikipedia.org/wiki/開放原始碼)的[软件](https://zh.wikipedia.org/wiki/軟體)[库](https://zh.wikipedia.org/wiki/函式庫)包，应用程序可以使用这个包来进行安全通信，避免窃听，同时确认另一端连线者的身份。这个包广泛被应用在互联网的网页服务器上。

其主要[库](https://zh.wikipedia.org/wiki/函式庫)是以[C语言](https://zh.wikipedia.org/wiki/C語言)所写成，实现了基本的[加密](https://zh.wikipedia.org/wiki/加密)功能，实现了[SSL与TLS](https://zh.wikipedia.org/wiki/傳輸層安全協議)协议。OpenSSL可以运行在[OpenVMS](https://zh.wikipedia.org/wiki/OpenVMS)、 [Microsoft Windows](https://zh.wikipedia.org/wiki/Microsoft_Windows)以及绝大多数[类Unix](https://zh.wikipedia.org/wiki/类Unix)操作系统上（包括[Solaris](https://zh.wikipedia.org/wiki/Solaris)，[Linux](https://zh.wikipedia.org/wiki/Linux)，[Mac OS X](https://zh.wikipedia.org/wiki/Mac_OS_X)与各种版本的开放源代码[BSD](https://zh.wikipedia.org/wiki/BSD)操作系统）。

## 4、openssl使用



## 5、telnet smtp 163邮箱

### 5.1 命令交互

c:telnet smtp.163.com 25

s:

Trying 123.126.97.5...
Connected to smtp.163.com.
Escape character is '^]'.
220 163.com Anti-spam GT for Coremail System (163com[20141201])



c:HELO Localhost

s:250 OK

c:AUTH LOGIN

s:334 dXNlcm5hbWU6(username:)

c:Y2R5bmhiQDE2My5jb20=

s:334 UGFzc3dvcmQ6(Password:)

c:MTM0MzQ1NjU4MDU=

s:235 Authentication successful

c:MAIL FROM: <cdynhb@163.com>

s:250 Mail OK

c:RCPT TO: <cdynhb@163.com>

s:250 Mail OK

c:DATA

s:354 End data with <CR><LF>.<CR><LF>

c:From: <cdynhb@163.com>

c:To: <cdynhb@163.com>

c:Subject: fisrt use telnet 163

c:hello smtp!

```
telnet smtp.163.com 25
Trying 123.126.97.1...
Connected to smtp.163.com.
Escape character is '^]'.
220 163.com Anti-spam GT for Coremail System (163com[20141201])
HELO Localhost
250 OK
AUTH LOGIN
334 dXNlcm5hbWU6
Y2R5bmhiQDE2My5jb20=
334 UGFzc3dvcmQ6
MTM0MzQ1NjU4MDU=
235 Authentication successful
MAIL FROM: <cdynhb@163.com>
250 Mail OK
RCPT TO: <cdynhb@163.com>
250 Mail OK
DATA
354 End data with <CR><LF>.<CR><LF>
From: <cdynhb@163.com>
Subject: fisrt use telnet 163
hello smtp!
<CR><LF>.<CR><LF>
.
250 Mail OK queued as smtp1,GdxpCgA3EfrftHFfZSmwAQ--.167S2 1601287569
```

发送附件：

```
开头：
--boundary=_zx
Content-Type: application/octet-stream;
charset="gb18030";name="%s"
Content-Disposition: attachment; 
filename="%s"
Content-Transfer-Encoding: base64

中间：data

结尾：--boundary=_zx--
```



### 5.2 SMTP返回码含义

- 220 服务就绪 221 服务关闭传输信道 421 服务未就绪，关闭传输信道
- 235 用户验证成功 535 用户验证失败
- 250 要求的邮件操作完成
- 334 等待用户输入验证信息
- 354 开始邮件输入，以.结束
- 503 错误的命令序列
- 550 要求的邮件操作未完成，邮箱不可用（例如，邮箱未找到，或不可访问）