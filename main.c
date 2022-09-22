#include <stdio.h>
#include <stdlib.h>
#include "nsx.h"
#include "agc.h"

void InnoTalkNS16KSampleX(char *szFileIn, char *szFileOut, uint32_t nSample)
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
		int32_t minLevel = 0;
		int32_t maxLevel = 255;
		// int agcMode = kAgcModeAdaptiveDigital;
		InnoTalkAgc_Init(AGC_inst, minLevel, maxLevel, nSample);

		InnoTalkAgc_config_t agcConfig;
		agcConfig.compressionGaindB = (int16_t)15;
		agcConfig.limiterEnable = (uint8_t)1;
		agcConfig.targetLevelDbfs = (int16_t)6;
		agcConfig.SilenceGainFall = (int16_t)110;
		agcConfig.AgcVadLowThr =  (int16_t)0;                //JT:AGC中VAD判定低门限,Q10，推荐值为0
		agcConfig.AgcVadUppThr =  (int16_t)1024;             //JT:AGC中VAD判定高门限,Q10，推荐值为1024
		agcConfig.GateLowThr =  (int16_t)0;                  //JT:Gate判定低门限，推荐值为0
		agcConfig.GateUppThr =  (int16_t)2500;               //JT:Gate判定高门限，推荐值为2500
		agcConfig.GateVadSensitivity =  (int16_t)6;             //JT:Gate计算对语音活性的敏感度，推荐值为6
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
				InnoTalkAgc_Process(AGC_inst, shTmpL, (int16_t)FRAME_LEN, shOutL, agcConfig);
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
	InnoTalkNS16KSampleX("TelinkTest.pcm", "TelinkTesto_128.pcm", (uint32_t)16000);
	printf("end!\n");

	getchar();
	return 0;
}
