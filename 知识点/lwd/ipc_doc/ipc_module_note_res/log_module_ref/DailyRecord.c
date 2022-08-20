
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include "Declare.h"
#include "DailyRecord.h"
#include "PublicInterface.h"

static char __szSyslogBuff[MAX_SYSLOG_BUFSIZE] = {0};
static pthread_mutex_t __szSyslogBuffMutex;

int	    _SysLogInitOk  = 0;		//是否已经初始化成功
int	    _SysLogFileNo  = 0;
#if HTTP_SEND_GAT1400
int     _SysLogFileNo_GAT1400 = 0;  //1400协议日志
#endif
int     _SysLogMemSize = 0;
int     _SysMaxLogFileNum = 0;
int		_SysLogMode	   = 0;		//0: 写向日志文件, 1: 写向串口
int     _SysLogPri     = 0;		//日志级别
								
int SS_SYSLOG_READ_CFG()
{
	FILE   *fp= NULL;
	char   szCfg[128] = {0};
	
	if ((fp = fopen("/etc/syslog.cfg", "r")) == NULL)
	{
	#if defined(HI3518E_ARCH) || defined(HI3518EV200_ARCH)
		_SysLogMemSize  = 128*1024;
		_SysMaxLogFileNum = (_SysLogMemSize)/(24*1024);
		_SysLogMode     = 0;
		_SysLogPri      = 3;
	#else
#if HTTP_SEND_GAT1400
		_SysLogMemSize  = 256*1024*2;
		_SysMaxLogFileNum = (_SysLogMemSize)/(48*1024)/2;
#else
		_SysLogMemSize  = 256*1024;
		_SysMaxLogFileNum = (_SysLogMemSize)/(48*1024);
#endif
		_SysLogMode     = 0;
		_SysLogPri      = 7;
	#endif
		printf("[%s %d] not exits log file\n", __FILE__, __LINE__);
	} 
	else
	{
		while (fgets(szCfg, sizeof(szCfg), fp) != NULL)
		{
			if (strstr(szCfg, "memsize=") != NULL)
			{
				sscanf(szCfg, "memsize=%d", &_SysLogMemSize);
				_SysLogFileNo = (_SysLogMemSize)/(48*1024);
			}
			else if (strstr(szCfg, "logmode=") != NULL)
			{
				sscanf(szCfg, "logmode=%d", &_SysLogMode);
			}
			else if (strstr(szCfg, "logpri=") != NULL)
			{
				sscanf(szCfg, "logpri=%d", &_SysLogPri);
			}
		}

		_SysMaxLogFileNum = (_SysLogMemSize)/(48*1024);
		SS_SYSLOG(LOG_DEBUG,"memsize %d logMode %d logPri %d FileNum %d\n",
			_SysLogMemSize, _SysLogMode, _SysLogPri, _SysLogFileNo);		
		fclose(fp);
		fp = NULL;
	}

	if (_SysMaxLogFileNum <= 0)
		_SysMaxLogFileNum = 1;
	return 0;
}
//===============================================================
//初始化
//===============================================================
int  SS_SYSLOG_INIT() 
{
	char szCmd[128] = {0};
	struct statfs statfsbuf;
	unsigned int total_size = 0;
	int   i  = 0;

	pthread_mutex_init(&__szSyslogBuffMutex, NULL);

	if(access("/etc/rebootFile", F_OK) != -1)
	{
		memset(szCmd, 0, sizeof(szCmd));
		sprintf(szCmd, "rm /etc/rebootFile");
		system(szCmd);
		usleep(100);
		memset(szCmd, 0, sizeof(szCmd));
	}

	memset(szCmd, 0, sizeof(szCmd));
	sprintf(szCmd, "touch /etc/rebootFile");
	system(szCmd);
	memset(szCmd, 0, sizeof(szCmd));

	SS_PUB_SetReboot(0);

	SS_SYSLOG_READ_CFG();

#if WEB_CHANGE
	system("rm -f /fx/web/en/debug.asp");
	system("ln -s /param/syslog /fx/web/en/debug.asp"); 
#if 1//MAC_ADDR_CHECK
	system("rm -f /fx/web/en/MacChange.asp");
	system("ln -s /param/MacChangelog /fx/web/en/MacChange.asp"); 
#endif
#else
	system("rm -f /web/en/debug.asp");
	system("ln -s /param/syslog /web/en/debug.asp"); 
#endif
	if (_SysLogMode == 1)
		return -1;
	
	system("umount /syslog/");	
	mkdir("/syslog", 0777);
	system("rm -f /syslog/*");
	sprintf(szCmd, "mount none /syslog -t tmpfs -o size=%d", _SysLogMemSize);	
	system(szCmd);
	//检测内存文件系统是否加载成功
	if (access("/syslog", F_OK) == -1)
		return -1;
	if (statfs("/syslog", &statfsbuf) != 0)
		return -1;

	total_size  = statfsbuf.f_blocks * statfsbuf.f_bsize;
	if(total_size == _SysLogMemSize)
	{
		_SysLogInitOk = 1;
		for (i = 0; i < _SysMaxLogFileNum; ++i)
		{
			memset(szCmd, 0, sizeof(szCmd));
			sprintf(szCmd, "touch /syslog/baselog%d", i);
			system(szCmd);
			memset(szCmd, 0, sizeof(szCmd));		
#if WEB_CHANGE
			sprintf(szCmd, "ln -s /syslog/baselog%d /fx/web/en/log%d.asp", i, i);
#else
			sprintf(szCmd, "ln -s /syslog/baselog%d /web/en/log%d.asp", i, i);
#endif
			system(szCmd);

#if HTTP_SEND_GAT1400
			memset(szCmd, 0, sizeof(szCmd));
			sprintf(szCmd, "touch /syslog/gatlog%d", i);
			system(szCmd);
			memset(szCmd, 0, sizeof(szCmd));		
#if WEB_CHANGE
			sprintf(szCmd, "ln -s /syslog/gatlog%d /fx/web/en/gatlog%d.asp", i, i);
#else
			sprintf(szCmd, "ln -s /syslog/gatlog%d /web/en/gatlog%d.asp", i, i);
#endif
			system(szCmd);
#endif
		}		
		SS_SYSLOG(LOG_WARNING, "Syslog mem file system is OK!\n");
	}
	else
	{
		SS_SYSLOG(LOG_ERR, "Syslog mem file system is err(%x %x)!\n", total_size, MAX_SYSLOG_MEMSIZE);
	}

	return 0;
}

int  SS_SYSLOG_UNINIT() 
{
	pthread_mutex_destroy(&__szSyslogBuffMutex);
}


//===============================================================
//打开文件
//===============================================================
int SS_SYSLOG_OPENFILE(const char *device, int mode)
{
	int m, f, fd = -1;

	m = mode | O_NONBLOCK;

	/* Retry up to 5 times */
	for (f = 0; f < 5; f++)
		if ((fd = open(device, m, 0600)) >= 0)
			break;

	if (fd < 0)
		return fd;

	/* Reset original flags. */
	if (m != mode)
		fcntl(fd, F_SETFL, mode);

	return fd;
}

//===============================================================
//写日志
//===============================================================
void SS_SYSLOG_WRITEILE(const char *buf, int priority, int flag)
{
	int      fd = -1;
	struct   flock fl;
	unsigned int nLen    = 0;
	unsigned int nStrLen = 0;
	char         szFileName[128] = {0};

	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 1;
	
	if(buf == NULL)
		return;

	if(flag == 0)
	{
		_SysLogFileNo = _SysLogFileNo % _SysMaxLogFileNum;
	}
	else
	{
#if HTTP_SEND_GAT1400
		if(flag == 1)
		{
			_SysLogFileNo_GAT1400 = _SysLogFileNo_GAT1400 % _SysMaxLogFileNum;
		}
#endif
	}
	nStrLen    = strlen(buf);

	if(flag == 0)
	{
		if (priority == LOG_EMERG)
		{
			sprintf(szFileName, "/param/syslog");
		}
#if 1//MAC_ADDR_CHECK
		else if(priority == LOG_ALERT)
		{
			sprintf(szFileName, "/param/MacChangelog");
		}
#endif
		else
		{
			sprintf(szFileName, "/syslog/baselog%d", _SysLogFileNo);
		}
	}
	else
	{
#if HTTP_SEND_GAT1400
		if(flag == 1)
		{
			if (priority == LOG_EMERG)
			{
				sprintf(szFileName, "/param/gatlog");
			}
			else
			{
				sprintf(szFileName, "/syslog/gatlog%d", _SysLogFileNo_GAT1400);
			}
		}
#endif
	}
	
	if ((fd = SS_SYSLOG_OPENFILE(szFileName, O_WRONLY | O_CREAT | O_NOCTTY | O_APPEND | O_NONBLOCK)) >= 0)
	{
		//文件加锁
		fl.l_type = F_WRLCK;
		fcntl(fd, F_SETLKW, &fl);

		struct stat statf;
		int r = fstat(fd, &statf);
		nLen  = lseek(fd, 0, SEEK_END) + nStrLen;
		
		if( !r && (statf.st_mode & S_IFREG) )		//成功
		{
		}
		else										//失败
		{
			//文件解锁
			fl.l_type = F_UNLCK;
			fcntl(fd, F_SETLKW, &fl);
			close(fd);
			return;
		}

		if( nLen > MAX_SYSLOG_FILESIZE ) 
		{
			if(flag == 0)
			{
				_SysLogFileNo = (_SysLogFileNo + 1) % _SysMaxLogFileNum;
			}
			else
			{
#if HTTP_SEND_GAT1400
				if(flag == 1)
				{
					_SysLogFileNo_GAT1400 = (_SysLogFileNo_GAT1400 + 1) % _SysMaxLogFileNum;
				}
#endif	
			}

			//文件解锁
			fl.l_type = F_UNLCK;
			fcntl (fd, F_SETLKW, &fl);

			//关闭文件
			close(fd);
			fd = -1;

			if(flag == 0)
			{
				if (priority == LOG_EMERG)
				{
					sprintf(szFileName, "/param/syslog");
				}
#if 1//MAC_ADDR_CHECK
				else if(priority == LOG_ALERT)
				{
					sprintf(szFileName, "/param/MacChangelog");
				}
#endif		
				else
				{
					sprintf(szFileName, "/syslog/baselog%d", _SysLogFileNo);
				}
			}
			else
			{
#if HTTP_SEND_GAT1400
				if(flag == 1)
				{
					if (priority == LOG_EMERG)
					{
						sprintf(szFileName, "/param/gatlog");
					}
					else
					{
						sprintf(szFileName, "/syslog/gatlog%d", _SysLogFileNo_GAT1400);
					}
				}
#endif
			}

			fd = SS_SYSLOG_OPENFILE (szFileName,	O_WRONLY | O_CREAT | O_NOCTTY | O_TRUNC | O_NONBLOCK);
			if(fd >= 0)
			{
				//文件加锁
				fl.l_type = F_WRLCK;
				fcntl (fd, F_SETLKW, &fl);
			}
		}
		
		if(fd >= 0)
		{
			//写数据
			write(fd,  buf, nStrLen);
			
			//文件解锁
			fl.l_type = F_UNLCK;
			fcntl(fd, F_SETLKW, &fl);

			//关闭文件
			close(fd);
			fd = -1;
		}
	}
}

//===============================================================
//应用程序打印日志
//===============================================================
void SS_SYSLOG_FUNC(int priority, const char *fmt,...)
{
	pthread_mutex_lock(&__szSyslogBuffMutex);
	
	time_t  secs_last;
	struct  tm *tm1 = NULL;  
	va_list args;

	//根据权限做处理
	//数据
	va_start(args, fmt);
	vsnprintf(&__szSyslogBuff[28], MAX_SYSLOG_BUFSIZE, fmt, args);
	va_end(args);

#if 1
	if( strstr(&__szSyslogBuff[28], "SS_Sock_Bind_Interface") != NULL ||
		strstr(&__szSyslogBuff[28], "SS_Set_Sock_Multi_MemberShip") != NULL ||
		strstr(&__szSyslogBuff[28], "SS_Set_Sock_Multi_MemberShip") != NULL ||
		strstr(&__szSyslogBuff[28], "nCurrAgc =") != NULL)
	{
		return;
	}
#endif

#if HONEYWELL_VERSION
	if(strstr(&__szSyslogBuff[28], "RECV REMOTE CONNECT"))
	{
		pthread_mutex_unlock(&__szSyslogBuffMutex);
		return;
	}
#endif

	if((_SysLogInitOk && _SysLogMode == 0) || priority == LOG_EMERG)//()			
	{
		//将时间写到缓存(时间 0 ~ 19), (模式20 ~ 27) (数据 28 ~ )
		secs_last = time(0);
		tm1  = localtime(&secs_last); 
		if(tm1 != NULL)
		{
			sprintf(__szSyslogBuff, "%04d-%02d-%02d %02d:%02d:%02d",
						tm1->tm_year+1900, tm1->tm_mon+1, tm1->tm_mday,
						tm1->tm_hour, tm1->tm_min, tm1->tm_sec);  
			__szSyslogBuff[19] = ' ';
		}
		else
		{
			memset(__szSyslogBuff, ' ', 20);
		}

		//printf("***************LOG_WARNING = %d, %d\n", LOG_WARNING, LOG_DEBUG);

		//权限(模式)
		if(priority == LOG_DEBUG)
		{
			memcpy(&__szSyslogBuff[20], "debug   ", 8);
		}
		else if (priority == LOG_INFO)
		{
			memcpy(&__szSyslogBuff[20], "info    ", 8);
		}
		else if (priority == LOG_WARNING)
		{
			memcpy(&__szSyslogBuff[20], "warn    ", 8);
		}
		else if (priority == LOG_ERR)
		{
			memcpy(&__szSyslogBuff[20], "error   ", 8);
		}
		else if (priority == LOG_EMERG)
		{
			memcpy(&__szSyslogBuff[20], "emerg   ", 8);
		}	
		//网页显示结束符
		strcat(__szSyslogBuff, "</br>");

		if (priority <= _SysLogPri)
			SS_SYSLOG_WRITEILE(__szSyslogBuff, priority, 0);
	}
	else
	{
		printf("%s", &__szSyslogBuff[28]);
	}

	pthread_mutex_unlock(&__szSyslogBuffMutex);
	
}

#if HTTP_SEND_GAT1400
void SS_GAT1400_SYSLOG(int priority, const char *fmt,...)
{
	time_t  secs_last;
	struct  tm *tm1 = NULL;  
	va_list args;

	//根据权限做处理
	//数据
	va_start(args, fmt);
	vsnprintf(&__szSyslogBuff[28], MAX_SYSLOG_BUFSIZE, fmt, args);
	va_end(args);

#if 1
	if( strstr(&__szSyslogBuff[28], "SS_Sock_Bind_Interface") != NULL ||
		strstr(&__szSyslogBuff[28], "SS_Set_Sock_Multi_MemberShip") != NULL ||
		strstr(&__szSyslogBuff[28], "SS_Set_Sock_Multi_MemberShip") != NULL ||
		strstr(&__szSyslogBuff[28], "nCurrAgc =") != NULL)
	{
		return;
	}
#endif

#if HONEYWELL_VERSION
	if(strstr(&__szSyslogBuff[28], "RECV REMOTE CONNECT"))
	{
		return;
	}
#endif

	if((_SysLogInitOk && _SysLogMode == 0) || priority == LOG_EMERG)//()			
	{
		//将时间写到缓存(时间 0 ~ 19), (模式20 ~ 27) (数据 28 ~ )
		secs_last = time(0);
		tm1  = localtime(&secs_last); 
		if(tm1 != NULL)
		{
			sprintf(__szSyslogBuff, "%04d-%02d-%02d %02d:%02d:%02d",
						tm1->tm_year+1900, tm1->tm_mon+1, tm1->tm_mday,
						tm1->tm_hour, tm1->tm_min, tm1->tm_sec);  
			__szSyslogBuff[19] = ' ';
		}
		else
		{
			memset(__szSyslogBuff, ' ', 20);
		}

		//printf("***************LOG_WARNING = %d, %d\n", LOG_WARNING, LOG_DEBUG);

		//权限(模式)
		if(priority == LOG_DEBUG)
		{
			memcpy(&__szSyslogBuff[20], "debug   ", 8);
		}
		else if (priority == LOG_WARNING)
		{
			memcpy(&__szSyslogBuff[20], "warn    ", 8);
		}
		else if (priority == LOG_ERR)
		{
			memcpy(&__szSyslogBuff[20], "error   ", 8);
		}
		else if (priority == LOG_EMERG)
		{
			memcpy(&__szSyslogBuff[20], "emerg   ", 8);
		}	
		//网页显示结束符
		strcat(__szSyslogBuff, "<br/>");
		
		//if (priority <= _SysLogPri)
			SS_SYSLOG_WRITEILE(__szSyslogBuff, priority, 1);
	}
	else
	{
		printf("%s", &__szSyslogBuff[28]);
	}
	
}
#endif
