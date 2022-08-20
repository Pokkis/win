#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h> //�ļ����ƶ���  
#include <termios.h>//�ն˿��ƶ���  
#include <errno.h>   
#define DEVICE "/dev/ttyAMA2"  
 
int serial_fd = 0;  
  
//�򿪴��ڲ���ʼ������  
int init_serial(void)  
{  
    serial_fd = open(DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);  
    if (serial_fd < 0) {  
        perror("open /dev/ttyAMA0 error!\n");  
        return -1;  
    }  
     
    //������Ҫ���ýṹ��termios <termios.h>  
   struct termios options;  
      
    /**1. tcgetattr�������ڻ�ȡ���ն���صĲ����� 
   *����fdΪ�ն˵��ļ������������صĽ��������termios�ṹ���� 
    */  
   tcgetattr(serial_fd, &options);  
    /**2. �޸�����õĲ���*/  
    options.c_cflag |= (CLOCAL | CREAD);//���ÿ���ģʽ״̬���������ӣ�����ʹ��  
    options.c_cflag &= ~CSIZE;//�ַ����ȣ���������λ֮ǰһ��Ҫ�������λ  
    options.c_cflag &= ~CRTSCTS;//��Ӳ������  
    options.c_cflag |= CS8;//8λ���ݳ���  
    options.c_cflag &= ~CSTOPB;//1λֹͣλ  
    options.c_iflag |= IGNPAR;//����ż����λ  
    options.c_oflag = 0; //���ģʽ  
    options.c_lflag = 0; //�������ն�ģʽ  
    cfsetospeed(&options, B115200);//���ò�����  
      
    /**3. ���������ԣ�TCSANOW�����иı�������Ч*/  
    tcflush(serial_fd, TCIFLUSH);//������ݿ��Խ��գ�������  
    tcsetattr(serial_fd, TCSANOW, &options);  
     
    return 0;  
}  
/** 
63.*���ڷ������� 
64.*@fd:���������� 
65.*@data:���������� 
66.*@datalen:���ݳ��� 
67.*/  
int uart_send(int fd, char *data, int datalen) 
{  
    int len = 0;  
    len = write(fd, data, datalen);//ʵ��д��ĳ���  
    if(len == datalen) {  
        return len;  
    } else {  
        tcflush(fd, TCOFLUSH);//TCOFLUSHˢ��д������ݵ�������  
        return -1;  
    }  
      
    return 0;  
}  
  
/** 
83.*���ڽ������� 
84.*Ҫ����������pc�˷���ascii�ļ� 
85.*/  
int uart_recv(int fd, char *data, int datalen)  
{  
    int len=0, ret = 0;  
    fd_set fs_read;  
    struct timeval tv_timeout;  
      
    FD_ZERO(&fs_read);  
    FD_SET(fd, &fs_read);  
    tv_timeout.tv_sec  = (100*20/115200+2);  
    tv_timeout.tv_usec = 0;  
      
    ret = select(fd+1, &fs_read, NULL, NULL, &tv_timeout);
	if(ret < 0)
	{
		printf("select ret < 0\n");
		return -1;
	}
	else if(ret == 0)
	{
		printf("select ret = 0\n");
		return -1;
	}
	else
	{
	    printf("ret = %d\n", ret);  
	    //�������0��������������״̬�ı�ǰ�ѳ���timeoutʱ��,���󷵻�-1  
	     
	    if (FD_ISSET(fd, &fs_read)) {  
	        len = read(fd, data, datalen);  
	        	printf("len = %d\n", len);  
	        return len;  
	    } else {  
	        perror("select");  
	        return -1;  
	    }  
	}      
    return 0;  
}  
  
int main(int argc, char **argv)  
{  
    init_serial();//��ʼ������  
 
    unsigned char buf[10]={0xfb,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x08};  
    char buf1[8];
	int i = 0;
	//for(i = 0;i < 10;i++)  
    		//uart_send(serial_fd, buf, 10);  
    //printf("\n");
	  while(1)
	  {
		printf("1111\n");
		//uart_send(serial_fd, buf, 10); 		 
	    int ret = uart_recv(serial_fd, buf1, 8);
		
		if(ret > 0)
		{
			//printf("------>buf1 = %s\n",buf1);
			printf("2222\n");
			for(i = 0;i < 8;i++)
			{
				printf("%x ",buf1[i]);
			}
			printf("\n");
			//uart_send(serial_fd, buf, 10); 
		}
		memset(buf1,0,8);
		printf("3333\n");	
		usleep(2000);
	  }
      
      
    //printf("uart receive %s\n", buf1);  
    close(serial_fd);  
    return 0;  
}  

