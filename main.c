#include <stdio.h>
#include <stdlib.h>
#include "nsx.h"
#include "agc.h"

#define LEN128_PACKET (128)

void InnoTalkNS16KSampleX(char *szFileIn, char *szFileOut, int nSample, int nMode)
{
	void *pNS_inst = NULL;
	void *AGC_inst = NULL;
	short shInL[LEN128_PACKET] = { 0 };
	short shTmpL[LEN128_PACKET] = { 0 };
	short shOutL[LEN128_PACKET] = { 0 };
	FILE *fpIn = NULL;
	FILE *fpOut = NULL;

	do
	{
		InnoTalkNsx_Create(&pNS_inst);
		InnoTalkNsx_InitCore(pNS_inst, nSample);

		InnoTalkAgc_Create(&AGC_inst);
		int minLevel = 0;
		int maxLevel = 255;
		int agcMode = kAgcModeAdaptiveDigital;
		InnoTalkAgc_Init(AGC_inst, minLevel, maxLevel, nSample);

		InnoTalkAgc_config_t agcConfig;
		agcConfig.compressionGaindB = 15;
		agcConfig.limiterEnable = 1;
		agcConfig.targetLevelDbfs = 6;
		agcConfig.SilenceGainFall = 110;
		agcConfig.AgcVadLowThr = 0;                //JT:AGC��VAD�ж�������,Q10���Ƽ�ֵΪ0
		agcConfig.AgcVadUppThr = 1024;             //JT:AGC��VAD�ж�������,Q10���Ƽ�ֵΪ1024
		agcConfig.GateLowThr = 0;                  //JT:Gate�ж������ޣ��Ƽ�ֵΪ0
		agcConfig.GateUppThr = 2500;               //JT:Gate�ж������ޣ��Ƽ�ֵΪ2500
		agcConfig.GateVadSensitivity = 6;             //JT:Gate������������Ե����жȣ��Ƽ�ֵΪ6
		InnoTalkAgc_set_config(AGC_inst, agcConfig);

		fpIn = fopen(szFileIn, "rb");
		fpOut = fopen(szFileOut, "wb");
		if (NULL == fpIn || NULL == fpOut)
		{
			printf("open file err \n");
			break;
		}

		while (1)
		{
			if (LEN128_PACKET == fread(shInL, sizeof(short), LEN128_PACKET, fpIn))
			{
				InnoTalkNsx_ProcessCore(pNS_inst, shInL, shTmpL);
				InnoTalkAgc_Process(AGC_inst, shTmpL, LEN128_PACKET, shOutL, agcConfig);
				fwrite(shOutL, sizeof(short), LEN128_PACKET, fpOut);
			}
			else
			{
				break;
			}
		}
	} while (0);

	fclose(fpIn);
	fclose(fpOut);
}

int main()
{
	printf("processing...\n");
	InnoTalkNS16KSampleX("TelinkTest.pcm", "TelinkTest0919.pcm", 16000, 3);
	printf("end!\n");

	getchar();
	return 0;
}
