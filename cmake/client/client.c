/*
 * @Author: Pokkis 1004267454@qq.com
 * @Date: 2022-05-14 23:56:25
 * @LastEditors: Pokkis 1004267454@qq.com
 * @LastEditTime: 2022-05-17 22:47:55
 * @FilePath: /shared_memory/server/service.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include "../common/include/share_memory.h"

#define TEST_FILE "./read01.h264"

int main()
{
    my_share_buf share_buf = { 0 };
    int n_ret = share_memory_init(&share_buf, SHARE_READ);
    DBG("n_ret:%d\n", n_ret);

    char data[100*1024];

    FILE *fp = fopen(TEST_FILE, "wb+");
    if(NULL == fp)
    {
        DBG("file open faild!\n");
        exit(0);
    }

    while(1)
    {
        n_ret = share_memory_read(&share_buf, data, sizeof(data));
        DBG("n_ret:%d\n", n_ret);
        if(n_ret >0)
            fwrite(data, 1, n_ret,fp);
        
        //usleep(50*1000);
    }

    fclose(fp);

    return 0;
}