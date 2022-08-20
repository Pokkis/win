#### 中兴 5G 模块调试记录：

##### 1、模块初始化命令：

```
ati		//get version, reveived info as follows
/**************** version ***************/
Manufacturer: ZTE CORPORATION
Model: ZM9000
Revision: BD_ZM9000V1.0.0B17
SVN: 01
IMEI: 860073040015790
+GCAP: +CGSM
/****************************************/

ate0			//set not echo
at+cfun=1		//1 online, 4 offline
at+cpin?		//SIM status
/******************************************************/
+CPIN: READY
/******************************************************/

at+csq			//signal intensity
/******************************************************/
+CSQ: 99,99
/******************************************************/

at+zrssi? 		//signal intensity
at+clip=1		//show ring num
at+cmgf=1		//set msg type

at+cgmr			//get sw version
/******************************************************/
BD_ZM9000V1.0.0B17
/******************************************************/

at^hwver		//get hw version, (error)
at+cgmm			//get module type
/******************************************************/
ZM9000
/******************************************************/

at+zpas?		//get sysinfo
/******************************************************/
+ZPAS: "NR5G-SA","PS_ONLY"
/******************************************************/

at+cops?		//get card type
/******************************************************/
+COPS: 0,0,"CHINA MOBILE CMCC",11
/******************************************************/
```

##### 2、问题排查

```
一般如果拨号不成功，查看
at+cpin?		//查看 SIM 状态
at+zrssi? 		//查看信号
at+zpas? 		//查看注册到4/5G
at+zcellinfo? 	//查看注册小区情况

at+csq 是获取信号强度
at+zrssi? 也是获取信号强度
5G 下 at+zrssi? 更可靠
```

手动启动网卡并分配 ip

ifconfig usb0 192.168.200.199 netmask 255.255.255.0 up

udhcpc 获取 ip（会失败）

udhcpc -i usb0 192.168.200.199

##### 2、代码片段：

```c++
typedef enum _SS_4G_MODEL_TYPE_E {
    NO_4G_MODEL = 0x0,
    WW_4G_MODEL = 0X1,
    HW_4G_MODEL = 0X2,
}SS_4G_MODEL_TYPE_E;

//byIspType：0 ADSL, 1 EVDO, 2 WCDMA, 3 TD-SCDMA
//byIspType 不同值时，apn 设置(szApn):
if (SysConfig.stNetBase3G.byIspType == 0) {
	strcpy(SysConfig.stNetBase3G.szApn, "");
	strcpy(SysConfig.stNetBase3G.stUserPwd.szUserName, "card");
	strcpy(SysConfig.stNetBase3G.stUserPwd.szPassword, "card");
	strcpy(SysConfig.stNetBase3G.szPhoneNumber, "#777");
	SysConfig.stNetBase3G.dw3GParam = 61; 
} else if (SysConfig.stNetBase3G.byIspType == 1) {
	strcpy(SysConfig.stNetBase3G.szApn, "3GPPP");
	strcpy(SysConfig.stNetBase3G.stUserPwd.szUserName, "");
	strcpy(SysConfig.stNetBase3G.stUserPwd.szPassword, "");
	strcpy(SysConfig.stNetBase3G.szPhoneNumber, "*99#");
	SysConfig.stNetBase3G.dw3GParam = 61;
} else if (SysConfig.stNetBase3G.byIspType == 2) {
	strcpy(SysConfig.stNetBase3G.szApn, "CMNET");
	strcpy(SysConfig.stNetBase3G.stUserPwd.szUserName, "");
	strcpy(SysConfig.stNetBase3G.stUserPwd.szPassword, "");
	strcpy(SysConfig.stNetBase3G.szPhoneNumber, "*98*1#");
	SysConfig.stNetBase3G.dw3GParam = 61;
}
```

