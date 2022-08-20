# openssl

# 1、简介

**OpenSSL**是一个[开放源代码](https://zh.wikipedia.org/wiki/開放原始碼)的[软件](https://zh.wikipedia.org/wiki/軟體)[库](https://zh.wikipedia.org/wiki/函式庫)包，应用程序可以使用这个包来进行*安全通信*，避免窃听，同时*确认另一端连线者的身份*。这个包广泛被应用在互联网的网页服务器上。

其主要[库](https://zh.wikipedia.org/wiki/函式庫)是以[C语言](https://zh.wikipedia.org/wiki/C語言)所写成，实现了基本的[加密](https://zh.wikipedia.org/wiki/加密)功能，实现了[SSL与TLS](https://zh.wikipedia.org/wiki/傳輸層安全協議)协议。

[1]: https://zh.wikipedia.org/wiki/OpenSSL

OpenSSL支持许多不同的加密算法：

- [加密](https://zh.wikipedia.org/wiki/加密)

  

  [AES](https://zh.wikipedia.org/wiki/高级加密标准)、[Blowfish](https://zh.wikipedia.org/wiki/Blowfish_(密码学))、[Camellia](https://zh.wikipedia.org/wiki/Camellia)、[Chacha20](https://zh.wikipedia.org/wiki/Salsa20#ChaCha20)、[Poly1305](https://zh.wikipedia.org/wiki/Poly1305)、[SEED](https://zh.wikipedia.org/w/index.php?title=SEED&action=edit&redlink=1)、[CAST-128](https://zh.wikipedia.org/w/index.php?title=CAST-128&action=edit&redlink=1)、[DES](https://zh.wikipedia.org/wiki/資料加密標準)、[IDEA](https://zh.wikipedia.org/wiki/國際資料加密演算法)、[RC2](https://zh.wikipedia.org/w/index.php?title=RC2&action=edit&redlink=1)、[RC4](https://zh.wikipedia.org/wiki/RC4)、[RC5](https://zh.wikipedia.org/wiki/RC5)、[TDES](https://zh.wikipedia.org/wiki/三重資料加密演算法)、[GOST 28147-89](https://zh.wikipedia.org/w/index.php?title=GOST_28147-89&action=edit&redlink=1)[[15\]](https://zh.wikipedia.org/wiki/OpenSSL#cite_note-gost-15)、[SM4](https://zh.wikipedia.org/wiki/SM4)

- [密码散列函数](https://zh.wikipedia.org/wiki/密碼雜湊函數)

  

  [MD5](https://zh.wikipedia.org/wiki/MD5)、[MD4](https://zh.wikipedia.org/wiki/MD4)、[MD2](https://zh.wikipedia.org/w/index.php?title=MD2&action=edit&redlink=1)、[SHA-1](https://zh.wikipedia.org/wiki/SHA-1)、[SHA-2](https://zh.wikipedia.org/wiki/SHA-2)、[SHA-3](https://zh.wikipedia.org/wiki/SHA-3)、[RIPEMD-160](https://zh.wikipedia.org/wiki/RIPEMD-160)、[MDC-2](https://zh.wikipedia.org/w/index.php?title=MDC-2&action=edit&redlink=1)、[GOST R 34.11-94](https://zh.wikipedia.org/w/index.php?title=GOST_R_34.11-94&action=edit&redlink=1)[[15\]](https://zh.wikipedia.org/wiki/OpenSSL#cite_note-gost-15)、[BLAKE2](https://zh.wikipedia.org/wiki/BLAKE#BLAKE2)、[Whirlpool](https://zh.wikipedia.org/wiki/Whirlpool)[[16\]](https://zh.wikipedia.org/wiki/OpenSSL#cite_note-16)、[SM3](https://zh.wikipedia.org/wiki/SM3)

- [公开密钥加密](https://zh.wikipedia.org/wiki/公开密钥加密)

  

  [RSA](https://zh.wikipedia.org/wiki/RSA)、[DSA](https://zh.wikipedia.org/wiki/DSA)、[ECDSA](https://zh.wikipedia.org/wiki/ECDSA)、[ECDHE](https://zh.wikipedia.org/wiki/ECDHE)、[迪菲-赫尔曼密钥交换](https://zh.wikipedia.org/wiki/迪菲-赫爾曼密鑰交換)、[椭圆曲线密码学](https://zh.wikipedia.org/wiki/椭圆曲线密码学)、[X25519](https://zh.wikipedia.org/wiki/Curve25519)、[Ed25519](https://zh.wikipedia.org/w/index.php?title=Ed25519&action=edit&redlink=1)、[X448](https://zh.wikipedia.org/w/index.php?title=X448&action=edit&redlink=1)、[Ed448](https://zh.wikipedia.org/w/index.php?title=Ed448&action=edit&redlink=1)、[GOST R 34.10-2001](https://zh.wikipedia.org/w/index.php?title=GOST_R_34.10-2001&action=edit&redlink=1)[[15\]](https://zh.wikipedia.org/wiki/OpenSSL#cite_note-gost-15)、[SM2](https://zh.wikipedia.org/wiki/SM2)

（从1.0版开始，[椭圆曲线迪菲-赫尔曼密钥交换](https://zh.wikipedia.org/wiki/橢圓曲線迪菲-赫爾曼金鑰交換)用于支持[前向安全性](https://zh.wikipedia.org/wiki/前向安全性)）[[17\]](https://zh.wikipedia.org/wiki/OpenSSL#cite_note-17))

# 2、如何进行移植

### 2.1 openssl版本详情

|                             版本                             |    发布时间    |                             备注                             |       最近更新版本       |
| :----------------------------------------------------------: | :------------: | :----------------------------------------------------------: | :----------------------: |
|                            0.9.1                             | 1998年12月23日 |                    OpenSSL项目的正式开启                     | 0.9.1c（1998年12月23日） |
|                            0.9.2                             | 1999年3月22日  |                          取代0.9.1c                          |  0.9.2b（1999年4月6日）  |
|                            0.9.3                             | 1999年5月25日  |                          取代0.9.2b                          | 0.9.3a（1999年5月27日）  |
|                            0.9.4                             |  1999年8月9日  |                          取代0.9.3a                          |  0.9.4（1999年4月9日）   |
|                            0.9.5                             | 2000年2月28日  |                          取代0.9.4                           |  0.9.5a（2000年4月1日）  |
|                            0.9.6                             | 2000年9月24日  |                          取代0.9.5a                          | 0.9.6m（2004年3月17日）  |
|                            0.9.7                             | 2002年12月31日 |                          取代0.9.6m                          | 0.9.7m（2007年2月23日）  |
|                            0.9.8                             |  2005年7月5日  |                          取代0.9.7m                          | 0.9.8zh（2015年12月3日） |
|                            1.0.0                             | 2010年3月29日  |                          取代0.9.8n                          | 1.0.0t（2015年12月3日）  |
| 1.0.1[[8\]](https://zh.wikipedia.org/wiki/OpenSSL#cite_note-openssl_1.0.1_notes-8) | 2012年3月14日  | 取代1.0.0维护至2016年12月31日支持RFC 6520 TLS/DTLS心跳支持[SCTP](https://zh.wikipedia.org/wiki/流控制传输协议)[RFC 5705](https://tools.ietf.org/html/rfc5705) TLS key material exporter[RFC 5764](https://tools.ietf.org/html/rfc5764) DTLS-SRTP协商Next Protocol NegotiationPSS signatures in certificates, requests and [证书吊销列表](https://zh.wikipedia.org/wiki/证书吊销列表)Support for password based recipient info for CMS支持TLS v1.2以及TLS v1.1Preliminary FIPS capability for unvalidated 2.0 FIPS module支持[安全远程密码协议](https://zh.wikipedia.org/w/index.php?title=安全远程密码协议&action=edit&redlink=1)（SRP） | 1.0.1u（2016年9月22日）  |
| 1.0.2[[9\]](https://zh.wikipedia.org/wiki/OpenSSL#cite_note-openssl_1.0.2_notes-9) | 2015年1月22日  | 取代1.0.1维护至2019年12月31日（长期维护）Suite B支持TLS 1.2和DTLS 1.2支持DTLS 1.2TLS自动选择[椭圆曲线](https://zh.wikipedia.org/wiki/椭圆曲线)（EC）用于设置TLS支持的签名算法和曲线的APISSL_CONF配置API支持TLS [Brainpool](https://zh.wikipedia.org/w/index.php?title=Brainpool&action=edit&redlink=1)支持[应用层协议协商](https://zh.wikipedia.org/wiki/应用层协议协商)CMS支持[RSA-PSS](https://zh.wikipedia.org/w/index.php?title=RSA-PSS&action=edit&redlink=1)、[RSA-OAEP](https://zh.wikipedia.org/w/index.php?title=RSA-OAEP&action=edit&redlink=1)、[ECDH](https://zh.wikipedia.org/wiki/ECDH)及X9.42 DH | 1.0.2u（2019年12月20日） |
| 1.1.0[[10\]](https://zh.wikipedia.org/wiki/OpenSSL#cite_note-openssl_1.1.0_notes-10) | 2016年8月25日  | 取代1.0.2h预计维护到2019年9月[[11\]](https://zh.wikipedia.org/wiki/OpenSSL#cite_note-openssl_1.1.1_release_blog-11)支持[BLAKE](https://zh.wikipedia.org/wiki/BLAKE#BLAKE2) ([RFC 7693](https://tools.ietf.org/html/rfc7693))支持[ChaCha20](https://zh.wikipedia.org/wiki/ChaCha20)-[Poly1305](https://zh.wikipedia.org/wiki/Poly1305)（[RFC 7539](https://tools.ietf.org/html/rfc7539)）支持[X25519](https://zh.wikipedia.org/wiki/X25519)（[RFC 7748](https://tools.ietf.org/html/rfc7748)）支持[DANE](https://zh.wikipedia.org/w/index.php?title=DANE&action=edit&redlink=1)和[证书透明度](https://zh.wikipedia.org/wiki/证书透明度)支持扩[CCM](https://zh.wikipedia.org/wiki/CCM)密码包支持扩展主密钥移除SSLv2移除对Kerberos密码包的支持将RC4和3DES从libssl从DEFAULT密码包中移除将DSS、SEED、IDEA、CAMELLIA以及AES-CCM从DEFAULT密码包中移除将40和56位密码从libssl中移除 | 1.1.0l（2019年9月10日）  |
| **1.1.1**[[12\]](https://zh.wikipedia.org/wiki/OpenSSL#cite_note-12) | 2018年9月11日  | 将持续维护至少五年（预计到2023年9月）[[11\]](https://zh.wikipedia.org/wiki/OpenSSL#cite_note-openssl_1.1.1_release_blog-11)支持[TLS1.3](https://zh.wikipedia.org/wiki/傳輸層安全性協定#TLS_1.3)[[13\]](https://zh.wikipedia.org/wiki/OpenSSL#cite_note-13)[[14\]](https://zh.wikipedia.org/wiki/OpenSSL#cite_note-14)支持[SHA-3](https://zh.wikipedia.org/wiki/SHA-3)支持[X448](https://zh.wikipedia.org/w/index.php?title=X448&action=edit&redlink=1)以及[Ed448](https://zh.wikipedia.org/w/index.php?title=Ed448&action=edit&redlink=1)（[RFC 7748](https://tools.ietf.org/html/rfc7748)）支持[SipHash](https://zh.wikipedia.org/w/index.php?title=SipHash&action=edit&redlink=1)支持[ARIA (cipher)](https://zh.wikipedia.org/w/index.php?title=ARIA_(cipher)&action=edit&redlink=1)支持多素数[RSA](https://zh.wikipedia.org/wiki/RSA加密演算法)（[RFC 8017](https://tools.ietf.org/html/rfc8017)）支持[SM2](https://zh.wikipedia.org/wiki/SM2)、[SM3](https://zh.wikipedia.org/wiki/SM3)以及[SM4](https://zh.wikipedia.org/wiki/SM4)移除[Heartbleed](https://zh.wikipedia.org/wiki/心脏出血漏洞)移除对QNX支持的支持 | 1.1.1g（2020年4月21日）  |

### 2.2 下载源码

在[官网](https://www.openssl.org/source/)下载所需版本源码，我下载的版本是`openssl-1.1.1h.tar.gz`。

### 2.3 编译安装

[参考](https://blog.csdn.net/sean_8180/article/details/86477271)

1. 解压源码包：`tar xzvf /mnt/hgfs/sdk/openssl/openssl-1.1.1h.tar.gz -C /disk4/opt/openssl/`
2. 配置`openssl`:`./config no-asm shared no-async --prefix=$(pwd)/install --cross-compile-prefix=arm-himix200-linux-`
3. 删除makefile中`-m32`编译选项
4. make && make install
5. 可以根据需要使用intall文件夹下的动态或者静态库
6. 如果使用动态库，只需将`libcrypto.so.1.1`和`libssl.so.1.1`拷贝到

备注：

`make clean`':清除Makefile以及中间生成文件使用，可用于错误配置makefile使用

`no-asm`: 在交叉编译过程中不使用汇编代码代码加速编译过程

`shared`: 生成动态连接库

`no-async`: 交叉编译工具链没有提供`GNU C`的`ucontext`库

`--prefix=`: 安装路径

`--cross-compile-prefix=`: 交叉编译工具



# 3、如何在海思运行使用

1. 将编译安装好的install





