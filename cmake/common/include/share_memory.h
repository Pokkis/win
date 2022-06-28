/*
 * @Author: Pokkis 1004267454@qq.com
 * @Date: 2022-05-13 22:23:18
 * @LastEditors: Pokkis 1004267454@qq.com
 * @LastEditTime: 2022-05-16 23:38:22
 * @FilePath: /shared_memory/client/share_memory.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef SHARE_MEMOYY
#define SHARE_MEMOYY

#define DEBUG             0

#ifndef DBG
#if  DEBUG
#define DBG(fmt, args..) do { \
                 fprintf(stdout, "[%s:%5d] " fmt, (char *)__FILE__,__LINE__,## args);     \
             } while(0)
#else
#define DBG(fmt) do { \
             } while(0)
#endif
#endif

#ifdef __cplusplus
extern "C"{ 
#endif

enum share_type
{
    SHARE_READ,
    SHARE_RWITE,
};

typedef struct sys_info_
{
    int  curwriterpos;
    int  prewriterpos;
    int  writercount;
}sys_info;

typedef struct sys_head_
{
    int  magic;
    int  size;
}sys_head;

typedef struct my_share_buf_
{
    sys_info read_info;
    int buf_size;
    sys_info *head;
    char *buf;
}my_share_buf;

int share_memory_init(my_share_buf *p_share, enum share_type rw);

int share_memory_write(my_share_buf *p_share, const char *data, const int data_size);

int share_memory_read(my_share_buf *p_share, char *data, int data_size);

#ifdef __cplusplus
}
#endif
#endif