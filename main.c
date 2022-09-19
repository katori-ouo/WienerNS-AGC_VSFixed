#include <stdio.h>
#include <stdlib.h>
#include "audio_config.h"
#include "nsx.h"
#include "agc.h"

#define FRAME_LEN (128)

void InnoTalkNS16KSampleX(char *szFileIn, char *szFileOut, int nSample, int nMode)
{
	void *pNS_inst = NULL;
	void *AGC_inst = NULL;
	short shInL[FRAME_LEN] = { 0 };
	short shTmpL[FRAME_LEN] = { 0 };
	short shOutL[FRAME_LEN] = { 0 };
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
		agcConfig.AgcVadLowThr = 0;                //JT:AGC中VAD判定低门限,Q10，推荐值为0
		agcConfig.AgcVadUppThr = 1024;             //JT:AGC中VAD判定高门限,Q10，推荐值为1024
		agcConfig.GateLowThr = 0;                  //JT:Gate判定低门限，推荐值为0
		agcConfig.GateUppThr = 2500;               //JT:Gate判定高门限，推荐值为2500
		agcConfig.GateVadSensitivity = 6;             //JT:Gate计算对语音活性的敏感度，推荐值为6
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
			if (FRAME_LEN == fread(shInL, sizeof(short), FRAME_LEN, fpIn))
			{
				InnoTalkNsx_ProcessCore(pNS_inst, shInL, shTmpL);
				InnoTalkAgc_Process(AGC_inst, shTmpL, FRAME_LEN, shOutL, agcConfig);
				fwrite(shOutL, sizeof(short), FRAME_LEN, fpOut);
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
