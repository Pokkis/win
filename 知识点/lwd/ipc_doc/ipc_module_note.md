### IPC module note

#### 1、语音对讲模块

##### 1.1、对讲功能初始化

SS_NETMAIN_RequestTalk 第一参数为 0，初始化播放语音的设备；

```c++
DBG("HandleTalkReqStart sock:%d\n", GetSocketnum());
DEV_USER_S    stUserConfig = {0};
SS_PARAM_GET_USER_CONFIG(0, &stUserConfig);
SS_NETMAIN_RequestTalk(0,0,0,0,NULL,0,stUserConfig.szUserName,stUserConfig.szPassword);
```

##### 1.2、播放对讲的数据

SS_NETMAIN_RequestTalk 第一个参数为 1，按照规定的数据头部，传递要播放的音频数据；

```c++
void XMSession::FRAME_HEAD(FRAME_HEAD_S *n,int audioLen)
{
	n->wBgnFlag = FRAME_BGN_FLAG;
	n->bySize = sizeof(FRAME_HEAD_S);
	n->byFrameType = FRAME_TYPE_A;
	n->dwFrameSize = audioLen;
	n->dwTimestamp = HiMFS_ConvertPtsToMs(0);
	n->stAudioInfo.wAencType		= PT_AENC_G711A;
	n->stAudioInfo.wChnNum		= 1;
	n->stAudioInfo.wBitsPerSamp	= 16;
	n->stAudioInfo.dwSampleRate  	= AI_SAMPLE_RATE_8000;
	n->stAudioInfo.dwBitrate		= PT_G726_16K;
	n->wEndFlag	= FRAME_END_FLAG;
	return;
}

xmaiMsgHeader *pXmaiMsgHeader = (xmaiMsgHeader *)msg;
DEV_USER_S	  stUserConfig;
SS_PARAM_GET_USER_CONFIG(0, &stUserConfig);
int audioLen = pXmaiMsgHeader->dataLen;
char audiobuf[2048] = {0};
FRAME_HEAD((FRAME_HEAD_S *)audiobuf,audioLen);
memcpy(audiobuf+sizeof(FRAME_HEAD_S), msg+sizeof(xmaiMsgHeader)+sizeof(AudioDataHeader),audioLen-sizeof(AudioDataHeader));
if(SS_NETMAIN_RequestTalk(1,0,0,0,audiobuf,audioLen-sizeof(AudioDataHeader)+sizeof(FRAME_HEAD_S),stUserConfig.szUserName,stUserConfig.szPassword) < 0)
{
	ERR("SS_NETMAIN_RequestTalk\n");
}
```

##### 1.3、对讲功能反初始化

SS_NETMAIN_RequestTalk 第一个参数为 2，反初始化播放音频的设备；

```c++
DBG("HandleTalkReqStop sock:%d\n", GetSocketnum());
DEV_USER_S    stUserConfig = {0};
SS_PARAM_GET_USER_CONFIG(0, &stUserConfig);
SS_NETMAIN_RequestTalk(2,0,0,0,NULL,0,stUserConfig.szUserName,stUserConfig.szPassword);
```



#### 2、日志模块 SYSLOG

##### 2.1、日志级别

```c++
/*
日志模块 在 DailyRecord.[hc] 中实现，使用 linux 下的 syslog 功能。
再将写入磁盘的日志文件，软连接到 web 访问权限的文件夹下，通过 web 页面可以访问 磁盘上的日志文件。
日志级别定于在 linux 下 <syslog.h> 头文件
*/
/**********************************************
form <syslog.h>
priority:
	LOG_EMERG		0		//emerg
    LOG_ALERT 		1		//alert
    LOG_CRIT 		2		//crit
    LOG_ERR 		3		//err
    LOG_WARNING 	4		//warning
    LOG_NOTICE 		5		//notice
    LOG_INFO 		6		//info
    LOG_DEBUG 		7		//debug
************************************************/
```

##### 2.2、日志查看方式

```
web 访问日志方式：
	http://ip/debug.asp
	LOG_EMERG 级别，对应 磁盘文件 /param/syslog。
	应用或系统重启不清除历史日志数据，直到文件超过指定大小 24Kb 或者 48Kb 被重写。
	
	http://ip/log[0-4].asp, eg: http://ip/log0.asp, http://ip/log1.asp etc.
	LOG_ERR, LOG_WARNING, LOG_INFO, LOG_DEBUG 级别日志，对应 磁盘文件 /syslog/baselog[0-4]。
    应用或系统重启会清除历史日志数据。
	当前一个 index 的日志超过设置的大小时，就写入下一个 index 的日志文件中，最有一个超过设置的大小时，写入第一个日志文件。	
	5 个日志文件呈环形的写入策略。
```

##### 2.3、日志接口

日志模块源码：[DailyRecord.h](./ipc_module_note_res/log_module_ref/DailyRecord.h), [DailyRecord.c](./ipc_module_note_res/log_module_ref/DailyRecord.c)

```c++
//日志模块初始化，写日志接口，日志模块反初始化，
int SS_SYSLOG_INIT();
int SS_SYSLOG_UNINIT() ;
void SS_SYSLOG_FUNC(int priority, const char *fmt,...);
```

注：根据日志模块代码逻辑，设备中 /etc/syslog.cfg 文件可以控制日志输出级别。日志级别配置文件：[syslog.cfg](./ipc_module_note_res/log_module_ref/syslog.cfg)

##### 2.4、日志使用方式(建议)

子模块日志接口宏调用参考：[dotsdk_define.h](./ipc_module_note_res/log_module_ref/dotsdk_define.h)

```c++
//各个子模块在使用日志时， 建议用 宏调用定义子模块专用的日志写接口，便于日志格式定义和日志内容检索
#include <syslog.h>

#define M_EMERG		LOG_EMERG
#define M_ALERT		LOG_ALERT
#define M_CRIT		LOG_CRIT
#define M_ERR		LOG_ERR
#define M_WARNING	LOG_WARNING
#define M_NOTICE	LOG_NOTICE
#define M_INFO		LOG_INFO
#define M_DEBUG		LOG_DEBUG

#define M_NAME		"DOTSDK"
#define m_syslog(priority, fmt, args...) do { \
	 SS_SYSLOG_FUNC(priority, "%s [%s:%5d] " fmt, (const char *)M_NAME, (const char *)__FILE__,__LINE__,## args); \
} while(0)
```

##### 2.5、日志记录初始化信息

独立模块的初始化 和 去初始化 函数尽量使用 可持久保存的日志，即 LOG_EMERG 级别日志，便于查看初始化流程和问题排查。举例：

```c++
int dotsdk_main()
{
#if DOTSDK_MODULE
	struct timeval cut_time;
	struct tm *p_tm;
	gettimeofday(&cut_time, NULL);
	p_tm = gmtime(&cut_time.tv_sec);
	char timeStr[32] = {0};
	snprintf(timeStr, 32, "%04d%02d%02d%02d%02d%02d.%06ld",
			1900 + p_tm->tm_year, 1 + p_tm->tm_mon, p_tm->tm_mday,
			p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec, cut_time.tv_usec);

	m_syslog(M_EMERG, "DOTSDK_MODULE %s init, begin time: %s, dev run time: %d \n", __FUNCTION__, timeStr, g_nDevRunTime);
	int ret;
	if (g_dotsdk_main_loop) {
		m_syslog(M_ERR, "%s dotsdk has been started \n", __FUNCTION__);
		return DOTSDK_RST_OK;
	}

	g_dotsdk_main_loop = 1;
	ret = pthread_create(&g_dotsdk_main_pid, NULL, dotsdk_main_proc, NULL);
	if (ret != 0) {
		m_syslog(M_ERR, "pthread_create dotsdk_main_proc failed \n");
		return DOTSDK_RST_ERR;
	}

	ret = pthread_detach(g_dotsdk_main_pid);
	if (ret != 0) {
		m_syslog(M_ERR, "pthread detached dotsdk_main_proc failed \n");
		return DOTSDK_RST_ERR;
	}
	
	gettimeofday(&cut_time, NULL);
	p_tm = gmtime(&cut_time.tv_sec);
	snprintf(timeStr, 32, "%04d%02d%02d%02d%02d%02d.%06ld",
			1900 + p_tm->tm_year, 1 + p_tm->tm_mon, p_tm->tm_mday,
			p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec, cut_time.tv_usec);
	m_syslog(M_EMERG, "DOTSDK_MODULE %s init, end time: %s, dev run time: %d \n", __FUNCTION__, timeStr, g_nDevRunTime);
#endif
	return DOTSDK_RST_OK;
}

int dotsdk_main_exit()
{
#if DOTSDK_MODULE
	m_syslog(M_EMERG, "DOTSDK_MODULE %s uninit \n", __FUNCTION__);

	if (!g_dotsdk_main_loop) {
		m_syslog(M_DEBUG, "%s dotsdk has been stopped \n", __FUNCTION__);
		return DOTSDK_RST_OK;
	}
	
	g_dotsdk_main_loop = 0;
#endif
	return DOTSDK_RST_OK;
}
```

##### 2.6、日志记录模块编译日期

主要模块和更新较频繁的模块，可以在模的 makefile 文件中加入编译时的时间宏，在模块初始化函数中使用该时间宏，将模块的编译时间记录到持久级别的日志中。这样做的目的是，在调试或者问题排查时有可靠的版本信息做参考。

模块 makefile 文件添加 XXX_MODULE_COMPILE_TIME ：

```makefile
SDK_COMPILE_TIME = $(shell date +"%Y%m%d%H%M%S")
$(warning "SDK_COMPILE_TIME=$(SDK_COMPILE_TIME)")

CFLAGS += -DSDK_COMPILE_TIME=$(SDK_COMPILE_TIME)
```

模块主函数使用时间宏：

```c++
#define     _MACROSTR(x) 		#x
#define     MACROSTR(x) 		_MACROSTR(x)

#define     VERSION_INFO_SIZE   64
#define     SDK_VERSION         "bjvms_sdk_19A_1.0.0.4"
#define     SDK_BUILDTIME       MACROSTR(SDK_COMPILE_TIME)

static char g_version_buildtime[VERSION_INFO_SIZE] = {0};

const char *GetSdkVersion()
{
    snprintf(g_version_buildtime, VERSION_INFO_SIZE, "%s_%s", SDK_VERSION, SDK_BUILDTIME);
    return g_version_buildtime;
}
```

#### 3、变焦变倍控制

```c++
typedef enum {
    FOCUS_ZOOM_STOP = 0,  /* 停止 */
    FOCUS_ZOOM_OUT = 1,   /* 缩小(变焦拉远、变倍缩小) */
    FOCUS_ZOOM_IN = 2,    /* 放大(变焦拉近、变倍放大) */
} FOCUS_ZOOM_EN;

typedef struct {
    FOCUS_ZOOM_EN act;
    int value;            /* 用作变焦、变倍相关命令回调时，仅步进变焦、步进变倍有效，表示步进值，范围1~5，对应的步长由设备自行控制；用作变焦、变倍到达边界通知时，0表示离开边界，1表示到达边界，目前仅到达边界需要通知 */
} FocusZoomInfo_t;

/* 步进变倍 */
int ipc_set_zoom_step(FocusZoomInfo_t *info, int *limit_status)
{
	if (info == NULL) {
		m_syslog(M_ERR, "Invalid arguments, info == NULL \n");
		return IPC_RST_ERR;
	}

#if 0
	PTZ_CTRL_S nPanCtrl = {0};
	nPanCtrl.byChannelNo = 0;
	nPanCtrl.byRes1 = 0;
	nPanCtrl.byRes2 = 0xFF;
	nPanCtrl.byData1 = 50;
	nPanCtrl.byData2 = 0;
	uint8_t stopType = PTZ_ZOOMADD_STOP;

	/* not stop action */
	if (FOCUS_ZOOM_OUT == info->act) {
	    m_syslog(M_DEBUG, "recv zoom stop zoom out cmd\n");
	    nPanCtrl.byCommand = PTZ_ZOOMSUB;
		stopType = PTZ_ZOOMSUB_STOP;
	} else if (FOCUS_ZOOM_IN == info->act) {
	    m_syslog(M_DEBUG, "recv zoom stop zoom in cmd\n");
	    nPanCtrl.byCommand = PTZ_ZOOMADD;
		stopType = PTZ_ZOOMADD_STOP;
	} else {
	    m_syslog(M_ERR, "Invalid action: %d \n", info->act);
	    return IPC_RST_ERR;
	}

	SS_PARAM_PTZ_CTRL(&nPanCtrl);

	/* stop */
	nPanCtrl.byCommand = stopType;
	SS_PARAM_PTZ_CTRL(&nPanCtrl);
#else
	uint8_t cmd = PTZ_ZOOMADD_STOP;
	uint8_t stopType = PTZ_ZOOMADD_STOP;
		
	if (FOCUS_ZOOM_OUT == info->act) {
	    m_syslog(M_DEBUG, "recv zoom step zoom out cmd, step %d\n", info->value);
		cmd = PTZ_ZOOMSUB;
		stopType = PTZ_ZOOMSUB_STOP;
	} else if (FOCUS_ZOOM_IN == info->act) {
	    m_syslog(M_DEBUG, "recv zoom step zoom in cmd, step %d\n", info->value);
		cmd = PTZ_ZOOMADD;
		stopType = PTZ_ZOOMADD_STOP;
	} else {
	    m_syslog(M_ERR, "Invalid action: %d \n", info->act);
        return IPC_RST_ERR;
	}

	SS_AF_Control(cmd, 0);
	SS_AF_Control(stopType, 0);
#endif

	//TODO return limit status
	
	return IPC_RST_OK;
}

/* 连续变倍 */
int ipc_set_zoom_continuously(FocusZoomInfo_t *info, int *limit_status)
{
	if (info == NULL) {
		m_syslog(M_ERR, "Invalid arguments, info == NULL \n");
		return IPC_RST_ERR;
	}

	uint8_t cmd = PTZ_ZOOMADD_STOP;
	if (FOCUS_ZOOM_STOP == info->act) {
        m_syslog(M_DEBUG, "recv zoom continuously stop zoom cmd\n");
		cmd = PTZ_ZOOMADD_STOP;
    } else if (FOCUS_ZOOM_OUT == info->act) {
        m_syslog(M_DEBUG, "recv zoom continuously zoom out cmd\n");
		cmd = PTZ_ZOOMSUB;
    } else if (FOCUS_ZOOM_IN == info->act) {
        m_syslog(M_DEBUG, "recv zoom continuously zoom in cmd\n");
		cmd = PTZ_ZOOMADD;
    } else {
    	m_syslog(M_ERR, "Invalid action: %d \n", info->act);
        return IPC_RST_ERR;
    }

	SS_AF_Control(cmd, 0);

	//TODO return limit status

	return IPC_RST_OK;
}
```

#### 4、HTTP Server 模块

##### 4.1、HTTP Server 模块私钥 打包部署

修改打包配置文件，filepath.xml，加入 HTTPS 私钥的打包部署。
打包路径
./packettool/paltform/configs/https/server-certificate-chain.pem
./packettool/paltform/configs/https/server-private-key.pem

部署路径
/data/https/server-certificate-chain.pem
/data/https/server-private-key.pem

filepath.xml 修改：

```
<Item>
<src>../../paltform/configs/https/server-certificate-chain.pem</src>
<dst>/data/https/server-certificate-chain.pem</dst>
</Item>
<Item>
<src>../../paltform/configs/https/server-private-key.pem</src>
<dst>/data/https/server-private-key.pem</dst>
</Item>
```

#### 5、设备升级 定制的 uImage

```shell
##查看定制的 linux 内核版本，或者 uname -a 命令
cat /proc/version

Linux version 3.18.20 (root@wang) (gcc version 4.9.4 20150629 (prerelease) (Hisilicon_v500_20150831) ) #6 SMP Mon Jan 22 11:07:07 CST 2018

##查看定制的 linux 系统分区
dmesg | grep kernel

Kernel command line: mem=512M console=ttyAMA0,115200 root=/dev/mtdblock2 coherent_pool=4M rootfstype=jffs2 rw mtdparts=hi_sfc:512k(boot),3M(kernel),12800k(rootfs)
Memory: 497180K/524288K available (4101K kernel code, 200K rwdata, 1136K rodata, 216K init, 339K bss, 27108K reserved, 0K highmem)
Virtual kernel memory layout:
0x000000080000-0x000000380000 : "kernel"
Freeing unused kernel memory: 216K (c0526000 - c055c000)
hi3519v101_base: module license 'Proprietary' taints kernel.
Disabling lock debugging due to kernel taint

##升级定制的 linux 内核
dd if=/mnt/3519V101/uImage-19v-200715 of=/dev/mtdblock1

##对比升级前后 linux 内核版本
cat /proc/version 

Linux version 3.18.20 (root@wang) (gcc version 4.9.4 20150629 (prerelease) (Hisilicon_v500_20170427) ) #7 SMP Wed Jul 15 17:41:42 CST 2020
```

#### 6、设备推流

##### 6.1、VLC 取流

VLC 取 rtsp 协议媒体流

```
vlc 打开网络串流，av0_0 为主码流，av0_1 为次码流:
rtsp://128.128.20.77:554/av0_0
rtsp://128.128.20.77:554/av0_1
```

VLC 取广播 udp 协议 TS 媒体流

```
udp://@238.1.1.1:40001
```

##### 6.2、GB 协议取流

//TODO

##### 6.3、onvif 协议取流

//TODO



#### 7、中兴 5G 模块调试

##### 7.1、模块文档参考

[ZM9000_5G_M.2模块产品硬件规格书V1.3.pdf](./ipc_module_note_res/ZTE_5G_doc_ref/ZM9000_5G_M.2模块产品硬件规格书V1.3.pdf)

[中兴通讯ZM9000_X55模块软件开发指导手册V2.8.pdf](./ipc_module_note_res/ZTE_5G_doc_ref/中兴通讯ZM9000_X55模块软件开发指导手册V2.8.pdf)

[中兴通讯ZM9000_X55模块AT指令手册_V2.8.pdf](./ipc_module_note_res/ZTE_5G_doc_ref/中兴通讯ZM9000_X55模块AT指令手册_V2.8.pdf)

[中兴通讯ZM9000模块软件FAQ.pdf](./ipc_module_note_res/ZTE_5G_doc_ref/中兴通讯ZM9000模块软件FAQ.pdf)

[27007-a70.pdf](./ipc_module_note_res/ZTE_5G_doc_ref/27007-a70.pdf)

[Windows 下模块启动 ZM9000setup1.0.0.7.exe.zip](./ipc_module_note_res/ZTE_5G_doc_ref/ZM9000setup1.0.0.7.exe.zip)

##### 7.1、模块调试记录

[ZTE_5G_debug.md](./ipc_module_note_res/ZTE_5G_doc_ref/ZTE_5G_debug.md)

##### 7.2、模块升级流程

[ZTE_5G_upgrage.md](./ipc_module_note_res/ZTE_5G_doc_ref/ZTE_5G_upgrage.md)







### Tools usage

##### tcpdump

https://www.jianshu.com/p/23427a80fc9d

https://www.cnblogs.com/ggjucheng/archive/2012/01/14/2322659.html

```
tcpdump tcp -i eth1 -t -s 0 -c 100 and dst port ! 22 and src net 192.168.1.0/24 -w ./target.cap

(1)tcp: ip icmp arp rarp 和 tcp、udp、icmp这些选项等都要放到第一个参数的位置，用来过滤数据报的类型
(2)-i eth1 : 只抓经过接口eth1的包
(3)-t : 不显示时间戳
(4)-s 0 : 抓取数据包时默认抓取长度为68字节。加上-S 0 后可以抓到完整的数据包
(5)-c 100 : 只抓取100个数据包
(6)dst port ! 22 : 不抓取目标端口是22的数据包
(7)src net 192.168.1.0/24 : 数据包的源网络地址为192.168.1.0/24
(8)-w ./target.cap : 保存成cap文件，方便用ethereal(即wireshark)分析

使用tcpdump抓取HTTP包
tcpdump  -XvvennSs 0 -i eth0 tcp[20:2]=0x4745 or tcp[20:2]=0x4854
0x4745 为"GET"前两个字母"GE",0x4854 为"HTTP"前两个字母"HT"
```



