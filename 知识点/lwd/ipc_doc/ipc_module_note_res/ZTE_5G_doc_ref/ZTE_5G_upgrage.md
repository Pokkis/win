##### 中兴 5G 模块 fota 升级流程

usb 串口 at 命令

```
echo ate0 > /dev/ttyUSB1
echo ati > /dev/ttyUSB1
echo at+zipcall=1 > /dev/ttyUSB1
echo at+zipcall? > /dev/ttyUSB1
echo at+zdm=1 > /dev/ttyUSB1
echo at+zdm=4 > /dev/ttyUSB1
echo at+zdm? > /dev/ttyUSB1
```

at 命令备份

```
echo ate0 > /dev/ttyUSB1
echo ati > /dev/ttyUSB1

//sim卡运营商对应apn,电信是ctnet,移动 cmnet,联通3gnet
echo at+zipcfg=cmnet,1,,,,fota > /dev/ttyUSB1
//echo at+zipcfg=3gnet,1,,,,fota > /dev/ttyUSB1
//echo at+zipcfg=ctnet,1,,,,fota > /dev/ttyUSB1

echo at+cfun=4 > /dev/ttyUSB1
echo at+cfun=1 > /dev/ttyUSB1
echo at+zipcall=1 > /dev/ttyUSB1
echo at+zipcall? > /dev/ttyUSB1
echo at+zdm=1 > /dev/ttyUSB1
echo at+zdm=4 > /dev/ttyUSB1
echo at+zdm? > /dev/ttyUSB1
```

##### 以上流程连续 5 次升级

版本号依次为：B17 -> 2B02 -> 2B03 -> 2B04 -> V1.2.0B01 -> V1.2.0B02

```
/**************** version ***************/
Manufacturer: ZTE CORPORATION
Model: ZM9000
Revision: BD_ZM9000V1.0.0B17
SVN: 01
IMEI: 860073040015790
+GCAP: +CGSM
/****************************************/

/**************** version ***************/
Manufacturer: ZTE CORPORATION
Model: ZM9000
Revision: BD_ZM9000V2.0.0B02
SVN: 01
IMEI: 860073040015790
+GCAP: +CGSM
/****************************************/

/**************** version ***************/
Manufacturer: ZTE CORPORATION
Model: ZM9000
Revision: BD_ZM9000V2.0.0B03
SVN: 01
IMEI: 860073040015790
+GCAP: +CGSM
/****************************************/

/**************** version ***************/
Manufacturer: ZTE CORPORATION
Model: ZM9000
Revision: BD_ZM9000V2.0.0B04
SVN: 01
IMEI: 860073040015790
+GCAP: +CGSM
/****************************************/

/**************** version ***************/
Manufacturer: ZTE CORPORATION
Model: ZM9000
Revision: BD_ZM9000V1.2.0B01
SVN: 01
IMEI: 860073040015790
+GCAP: +CGSM
/****************************************/

/**************** version ***************/
Manufacturer: ZTE CORPORATION
Model: ZM9000
Revision: BD_ZM9000V1.2.0B02
SVN: 01
IMEI: 860073040015790
+GCAP: +CGSM
/****************************************/
```



