/*
 * @Author: Pokkis 1004267454@qq.com
 * @Date: 2022-05-07 20:47:59
 * @LastEditors: Pokkis 1004267454@qq.com
 * @LastEditTime: 2022-05-15 14:44:21
 * @FilePath: /shared_memory/client/file_parse_h264.h
 * @Description:
 */
#ifndef _FILE_PARSE_H264_H_
#define _FILE_PARSE_H264_H_

/**
 * @description: 
 * @param {unsigned char} *p_data 源数据
 * @param {int} *p_pre_len nal type前的长度
 * @return {*}
 */

#ifdef __cplusplus
extern "C"{ 
#endif

int find_nal_position(const unsigned char *p_data, const int data_len, int *p_pre_len);

#ifdef __cplusplus
}
#endif

#endif