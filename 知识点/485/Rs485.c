void *SS_RADARINFO_READ_Task(void *arg)
{
	int nCommNo = 0;
	char szRecvBuffer[256] = {0};
	char szCmdBuffer[128] = {0};
	int nRecvLen = 0;
	int nCmdLen = 0;
	int ret = 0;
	AlarmSensorParamS alarmSensorParam;	
	while(!stSysStatus.Restarting)
	{
		if( gCurComSet[nCommNo].stSerialProtocol.baudrate != 115200   ||		
			gCurComSet[nCommNo].stSerialProtocol.dataBits  != 8      ||
			gCurComSet[nCommNo].stSerialProtocol.stopBits  != 1      ||
			gCurComSet[nCommNo].stSerialProtocol.parity!= 0      ||
			gCurComSet[nCommNo].stSerialProtocol.flowControl != 0  ||
			(SS_COMM_GetFd(nCommNo) <= 0)
			)
		{
			gCurComSet[nCommNo].stSerialProtocol.baudrate  = 115200;		
			gCurComSet[nCommNo].stSerialProtocol.dataBits   = 8;		
			gCurComSet[nCommNo].stSerialProtocol.stopBits   = 1;		
			gCurComSet[nCommNo].stSerialProtocol.parity = 0;		
			gCurComSet[nCommNo].stSerialProtocol.flowControl  = 0;		
			SS_COMM_Init(nCommNo, &gCurComSet[nCommNo]);	
		}
		
		if(getRs485ReadFlag() == 0)
		{
			usleep(40 * 1000);
			continue;
		}
		
		if(nCommNo == 0)
			SS_RS232_SetRS485_RTX(0);

		memset(szRecvBuffer, 0, sizeof(szRecvBuffer));
		memset(szCmdBuffer, 0, sizeof(szCmdBuffer));
		
		ret = SS_Recv_RADAR_Data(nCommNo, (char *)szRecvBuffer, sizeof(szRecvBuffer), &nRecvLen);
		
		if(ret < 0)
		{
			usleep(40 * 1000);
		}
		
		if(nRecvLen > 0)
		{
			
			#if 1
			#if YINGFANG_DEBUG	
			printf("================================= \n");
			printf("recv cmd buffer[%d]: \n", nRecvLen);
			for(int i=0; i < nRecvLen; i++)
				printf("%02x ", szRecvBuffer[i]);
			printf("\n");
			#endif
			#if 1
			//只取一条数据发送
			nCmdLen	= SS_Get_Recv_ONE_RADAR_DATA(szCmdBuffer, sizeof(szCmdBuffer), szRecvBuffer, sizeof(szRecvBuffer));
			if(nCmdLen > 0)
			{
				memset(&alarmSensorParam, 0, sizeof(AlarmSensorParamS));
				SS_PARAM_Get_AlarmSensorParam(&alarmSensorParam);
				ret = SS_RADARINFO_SOCKET_SEND((char *)szRecvBuffer, nCmdLen, alarmSensorParam.radarParam.ip, alarmSensorParam.radarParam.port);
				if(ret == 1)
				{
					printf("SS_RADARINFO_SOCKET_SEND succedd:ret:%d\n", ret);
				}
				else
				{
					printf("SS_RADARINFO_SOCKET_SEND faild:ret:%d\n", ret);
				}
			}
			#else
			while(!stSysStatus.Restarting)
			{
				nCmdLen = SS_Get_Recv_RADAR_DATA(szCmdBuffer, sizeof(szCmdBuffer), szRecvBuffer, sizeof(szRecvBuffer), &nRecvLen);
				if(nCmdLen > 0)
				{
					memset(&alarmSensorParam, 0, sizeof(AlarmSensorParamS));
					SS_PARAM_Get_AlarmSensorParam(&alarmSensorParam);
					ret = SS_RADARINFO_SOCKET_SEND((char *)szRecvBuffer, nCmdLen, alarmSensorParam.radarParam.ip, alarmSensorParam.radarParam.port);
					if(ret == 1)
					{
						printf("SS_RADARINFO_SOCKET_SEND succedd:ret:%d\n", ret);
					}
					else
					{
						printf("SS_RADARINFO_SOCKET_SEND faild:ret:%d\n", ret);
					}
				}
				else
				{
					break;
				}
				usleep(20 * 1000);
			}
			#endif
			#if YINGFANG_DEBUG	
			printf("================================= \n");
			#endif
		#endif
		}
		else
		{
			usleep(40 * 1000);
		}

		tcflush((SS_COMM_GetFd(0)),TCIFLUSH);
		
	}
}