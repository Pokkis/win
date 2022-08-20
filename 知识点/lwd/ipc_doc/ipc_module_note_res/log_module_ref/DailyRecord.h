
#ifndef   _DAILY_RECORD_H_
#define   _DAILY_RECORD_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/watchdog.h>

#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <linux/rtc.h>

#include <syslog.h>
#include <sys/statfs.h>


#ifdef __cplusplus
extern "C" {
#endif


#if defined(HI3518E_ARCH) || defined(HI3518EV200_ARCH)
#define MAX_SYSLOG_BUFSIZE		(32 + 4*1024)
#define MAX_SYSLOG_MEMSIZE		(128 * 1024)
#define MAX_SYSLOG_FILENUM		5
#define MAX_SYSLOG_FILESIZE		(24*1024)
#else
#define MAX_SYSLOG_BUFSIZE		(32 + 8*1024)
#define MAX_SYSLOG_MEMSIZE		(256 * 1024)
#define MAX_SYSLOG_FILENUM		5
#define MAX_SYSLOG_FILESIZE		(48*1024)
#endif

/**********************************************
form <syslog.h>
priority:
	LOG_EMERG 	0		//emerg
　　LOG_ALERT 	1		//alert
　　LOG_CRIT 	2		//crit
　　LOG_ERR 	3		//err
　　LOG_WARNING 4		//warning
　　LOG_NOTICE 	5		//notice
　　LOG_INFO 	6		//info
　　LOG_DEBUG 	7		//debug
************************************************/
#define SUB_MODULE_NAME		""
#define SS_SYSLOG(priority, fmt, args...) do { \
	 SS_SYSLOG_FUNC(priority, "%s [%s:%5d] " fmt, (const char *)SUB_MODULE_NAME, (const char *)__FILE__,__LINE__,## args);	 \
} while(0)

int  SS_SYSLOG_INIT();
int  SS_SYSLOG_UNINIT() ;
void SS_SYSLOG_FUNC(int priority, const char *fmt,...);
#if HTTP_SEND_GAT1400
void SS_GAT1400_SYSLOG(int priority, const char *fmt,...);
#endif
void SS_SYSLOG_WRITEILE(const char *buf, int priority, int flag);
int SS_SYSLOG_OPENFILE(const char *device, int mode);


#ifdef __cplusplus
}
#endif


#endif


