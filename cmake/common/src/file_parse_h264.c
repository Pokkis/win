/*
 * @Author: Pokkis 1004267454@qq.com
 * @Date: 2022-05-07 20:47:42
 * @LastEditors: Pokkis 1004267454@qq.com
 * @LastEditTime: 2022-05-15 15:38:59
 * @FilePath: /shared_memory/client/file_parse_h264.c
 * @Description: 
 */
#include <stdio.h>
#include "file_parse_h264.h"

int find_nal_position(const unsigned char *p_data, const int data_len, int *p_pre_len)
{
    if(NULL == p_data || 0 == data_len || NULL == p_pre_len)
    {
        return -1;
    }

    static int cur_pre_len = 2;
    static char init = 0;

    int pre_len = 0;
    for(int i = 0; i < data_len; i++)
    {
        if(pre_len >= cur_pre_len && p_data[i] == 1)
        {
            *p_pre_len = pre_len;
            if(init == 0)
            {
                cur_pre_len = pre_len;
                init = 1;
            }

            //printf("p_data:%x i:%d pre_len:%d\n", p_data, i, pre_len);
            return (i - pre_len);
        }

        if(p_data[i])
        {
            pre_len = 0;
        }
        else
        {
            pre_len++;
        }

    }

    return -2;
}