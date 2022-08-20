# [【HISI系列】之SDK编码器开发框架](https://www.cnblogs.com/SoaringLee/p/10532180.html)

------

##### 1、 前言

    本文介绍hisi35xx系列，例如HI3519V101编码器开发的大体流程框架，重点分析HISI硬件H264/H265编码器的编码流程和框架。

##### 2、HISI 硬件编码器开发框架

###### 2.1、系统层初始化

```c
HI_MPI_SYS_GetVersion	:获取MPP的版本号
HI_MPI_SYS_Exit			:去初始化MPP系统
HI_MPP_VB_Exit			:去初始化MPP视频缓存池
HI_MPI_VB_SetConfig		:设置MPP视频缓存池属性
HI_MPI_VB_Init			:初始化MPP视频缓存池
HI_MPI_SYS_SetConfig	:配置系统控制参数
HI_MPI_SYS_Init			:初始化MPP系统
```

###### 2.2、编码层初始化

在创建编码通道之前，需要设置静态属性，包括编码器属性、码率控制属性和GOP模式属性。

```c
HI_MPI_VENC_CreatChn		:创建编码通道
HI_MPI_VENC_StartRecvFrame  :开启编码通道接收输入图像
```

###### 2.3、编码层Buffer的设置

通过缓存块的方式设置YUV Buffer:

```c
HI_MPI_VB_GetBlock		  :获取一个缓存块
HI_MPI_VB_Handle2PhysAddr :获取一个缓存块的物理地址
HI_MPI_SYS_MmapCache	  :存储带Cache映射接口
HI_MPI_VB_Handle2PoolId	  :获取一个缓存块所在缓存池的ID
```

###### 2.4、编码

```c
HI_MPI_VENC_GetChnAttr	:获取编码通道的编码属性。 HI_MPI_VENC_SetChnAttr 设置编码通道的编码属性
HI_MPI_VENC_GetRcParam	:获取通道码率控制高级参数。 HI_MPI_VENC_SetRcParam  设置编码通道码率控制高级参数
HI_MPI_SYS_MmzFlushCache:刷新cache里的内容到内存并且使cache里的内容无效
HI_MPI_VENC_SendFrame	:支持用户发送原始图像进行编码
HI_MPI_VENC_SendFrameEx	:支持用户发送原始图像及该图的QPMAP表信息进行编码
HI_MPI_VENC_GetStream	:获取编码码流
```

###### 2.5、资源释放、关闭通道

```c
HI_MPI_SYS_Munmap		：存储反映射接口
HI_MPI_VB_ReleaseBlock	: 释放一个已经获取的缓存块
HI_MPI_SYS_MmzFree		: 在用户态释放MMZ内存
HI_MPI_VENC_StopRecvFrame:停止编码通道接收输入图像
HI_MPI_VENC_DestroyChn   :销毁编码通道
HI_MPI_SYS_Exit			 :去初始化MPP系统
HI_MPP_VB_Exit			 :去初始化MPP视频缓存池
```

##### 3、HISI开发经验

由于hisi每一款芯片对各编码协议（如H264/H265/JPEG/MOTION-JPEG）的支持情况不一，有些函数只适用于某几个编码协议，有些函数只适用于某一个编码协议，因此在构建编码器框架时，编码协议类型及编码通道号应在整个框架中传递。

##### 4、参考和文档下载

HISI MPP文档：https://download.csdn.net/download/wjx2000/10878917
SDK编码器开发框架参考：https://blog.csdn.net/listener51/article/details/85344531