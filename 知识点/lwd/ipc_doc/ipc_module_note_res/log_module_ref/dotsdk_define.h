#ifndef __DOTSDK_DEFINE_H__
#define __DOTSDK_DEFINE_H__


#ifndef true
#define true (1)
#endif
#ifndef false
#define false (0)
#endif

#define DOTSDK_MODULE			1	//dot sdk module Œ÷º“…Ò—€

#define DEV_CONFIG_MODE_TURNOFF 0
#define DEV_CONFIG_MODE_TURNON  1
#define DEV_CONFIG_MODE_AUTO    2


#define DEV_CONFIG_SDVIDEO       0
#define DEV_CONFIG_HDVIDEO       1
#define DEV_CONFIG_SUPERVIDEO    2

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
	 SS_SYSLOG_FUNC(priority, "%s [%s:%5d] " fmt, (const char *)M_NAME, (const char *)__FILE__,__LINE__,## args);	 \
 } while(0)


#ifndef DBG
#define DBG(fmt, args...) do { \
	 fprintf(stdout, "\033[m""[-DBG-] [%s:%5d] " fmt, (const char *)__FILE__,__LINE__,## args);	 \
 } while(0)
#endif

#ifndef ERR
#define ERR(fmt, args...) do { \
	 fprintf(stderr, "\033[1;31m""[ERR!] [%s:%5d] " fmt, (const char *)__FILE__,__LINE__,## args);	\
 } while(0)
#endif


#endif	//__DOTSDK_DEFINE_H__