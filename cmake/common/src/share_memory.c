/*
 * @Author: Pokkis 1004267454@qq.com
 * @Date: 2022-05-13 22:22:49
 * @LastEditors: Pokkis 1004267454@qq.com
 * @LastEditTime: 2022-05-17 22:58:53
 * @FilePath: /shared_memory/client/share_memory.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%A
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//#include <sys/shm.h>  // shared memory
//#include <sys/sem.h>  // semaphore
#include <string.h>   // memcpy
#include "share_memory.h"

#define MAGIC_NUM  0x12348765
//最大只能2M
#define DATA_MAX   1024*1024

int share_memory_init(my_share_buf *p_share, enum share_type rw)
{
    assert(NULL != p_share);
#if 0
    key_t key = MAGIC_NUM;
    int shmid;
    
    // 获取key值
	// if((key = ftok(".", 'z')) < 0)
	// {
	// 	perror("ftok error");
	// 	exit(1);
	// }

	// 创建共享内存 同步字节头+环形数据区大小
    p_share->buf_size = DATA_MAX;//环形数据区大小
    int share_buf_size = p_share->buf_size + sizeof(sys_info);

    if((shmid = shmget(key, share_buf_size, IPC_CREAT|0666)) == -1)
	{
		perror("Create Shared Memory Error ");
		exit(1);
	}

	// 连接共享内存
	p_share->buf = (char*)shmat(shmid, 0, 0);
	if(p_share->buf == NULL)
	{
		perror("Attach Shared Memory Error");
		exit(1);
	}

    p_share->head = (sys_info *)p_share->buf;
    p_share->buf = p_share->buf + sizeof(sys_info);
    if(rw == SHARE_RWITE)
    { 
        sys_info *p_sys_info = p_share->head;
        p_sys_info->curwriterpos = p_sys_info->prewriterpos = 0;
        p_sys_info->writercount = 0;
    }
    p_share->read_info.curwriterpos = -1;
    p_share->read_info.prewriterpos = -1;
    p_share->read_info.writercount = 0;
#endif   
    //sys_info *p_sys_info = p_share->head;
    //DBG("prewriterpos:%d curwriterpos:%d writercount:%d\n", p_sys_info->prewriterpos, p_sys_info->curwriterpos, p_sys_info->writercount);
    
    return 0;
}

static int share_memory_data_add(my_share_buf *p_share, const char *data, const int data_size)
{
    assert(NULL != p_share);
    assert(NULL != data);
   
    sys_info *p_sys_info = p_share->head;

    //填充数据
    if(p_sys_info->curwriterpos + data_size > p_share->buf_size)
    {
        //拷贝数据前半部分
        memcpy(p_share->buf + p_sys_info->curwriterpos, data, p_share->buf_size - p_sys_info->curwriterpos);
        //拷贝数据后半部分
        memcpy(p_share->buf , data + p_share->buf_size - p_sys_info->curwriterpos, data_size - (p_share->buf_size - p_sys_info->curwriterpos));

        p_sys_info->curwriterpos = (p_sys_info->curwriterpos + data_size) % p_share->buf_size;
    }
    else
    {
        //拷贝数据
        memcpy(p_share->buf + p_sys_info->curwriterpos, data, data_size);
        p_sys_info->curwriterpos = p_sys_info->curwriterpos + data_size;
    }

    return 0;
}

int share_memory_write(my_share_buf *p_share, const char *data, const int data_size)
{
    assert(NULL != p_share);
    assert(NULL != data);

    if(data_size > p_share->buf_size)
        return -1;

    sys_info *p_sys_info = p_share->head;
    p_sys_info->writercount++;
    p_sys_info->prewriterpos = p_sys_info->curwriterpos;

    sys_head data_head = { 0 };
    data_head.magic = MAGIC_NUM;
    data_head.size = data_size;
    int data_head_size = sizeof(sys_head);

    //填充数据同步头
    share_memory_data_add(p_share, (char *)&data_head, data_head_size);

    //在同步头后填充数据
    share_memory_data_add(p_share, data, data_size);

//debug with write 
#if 0
    DBG("prewriterpos:%d curwriterpos:%d writercount:%d\n", p_sys_info->prewriterpos, p_sys_info->curwriterpos, p_sys_info->writercount);

    sys_head *p_sys_head = p_share->buf + p_sys_info->prewriterpos;
    DBG("magic:%x\n", p_sys_head->magic);
    DBG("size:%d\n", p_sys_head->size);
#endif

    return 0;
}

static int share_memory_data_read(my_share_buf *p_share, int start_pos, char *data, int data_size)
{
    assert(NULL != p_share);
    assert(NULL != data);
    
    //填充数据
    if(start_pos + data_size > p_share->buf_size)
    {
        //拷贝数据前半部分
        memcpy(data, p_share->buf + start_pos, p_share->buf_size - start_pos);
        //拷贝数据后半部分
        memcpy(data, p_share->buf, data_size - (p_share->buf_size - start_pos));
    }
    else
    {
        //拷贝数据
        memcpy(data, p_share->buf + start_pos, data_size);
    }


    return 0;
}

int share_memory_read(my_share_buf *p_share, char *data, const int data_size)
{
   // assert(NULL != p_share);
  //  assert(NULL != data);

   /* if(data_size > p_share->buf_size)
    {
        DBG("read data to large!\n");
        return -1;
    }*/
        

    sys_info *p_sys_info = p_share->head;
    //DBG("prewriterpos:%d curwriterpos:%d writercount:%d\n", p_sys_info->prewriterpos, p_sys_info->curwriterpos, p_sys_info->writercount);
    if(p_share->read_info.curwriterpos == -1)
    {
        p_share->read_info.curwriterpos = p_sys_info->curwriterpos;
    }

    //读取每一帧数据同步头

    sys_head head ;

    share_memory_data_read(p_share, p_share->read_info.curwriterpos, (char *)&head, (int)sizeof(head));
    if(head.magic != MAGIC_NUM)
    {
       DBG("no data to read!\n");
        return -2;
    }


    DBG("magic:%x\n", head.magic);
    DBG("size:%d\n", head.size);

    int read_data = data_size > head.size ? head.size : data_size;

    //读取每一帧数据
    share_memory_data_read(p_share, p_share->read_info.curwriterpos + sizeof(sys_head), data, read_data);

    p_share->read_info.prewriterpos = p_share->read_info.curwriterpos;
    p_share->read_info.curwriterpos = (p_share->read_info.curwriterpos + sizeof(sys_head) + head.size)%p_share->buf_size;
    p_share->read_info.writercount++;

    //DBG("prewriterpos:%d curwriterpos:%d writercount:%d\n", p_share->read_info.prewriterpos, p_share->read_info.curwriterpos, p_share->read_info.writercount);
    
    return head.size;
}