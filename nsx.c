/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "audio_config.h"
#include "nsx.h"

extern const int32_t CEVA_TL4_cos_sin_fft_32[512 * 2];
extern const int32_t twi_table_32_rfft_256[64 * 2];
int32_t fftftemp[ANAL_BLOCKL_MAX * 2];
int32_t FFTIOt[ANAL_BLOCKL_MAX];
int32_t bitrev[0x400 + 2];

// Q15
const int32_t delta32[129] = { 42598, 42598, 42598, 42598, 42598, 42598, 42598, 42598, 42598,
    42598, 42598, 42598, 42598, 42598, 42598, 42598, 42598, 42598, 42598, 42598, 42598, 42598,
    42598, 42598, 42598, 42598, 42598, 42598, 42598, 42598, 42598, 42598, 41871, 43765, 45660,
    47554, 49448, 51343, 53237, 55132, 57026, 58920, 60815, 62709, 64604, 66498, 68392, 70287,
    72181, 74076, 75970, 77864, 79759, 81653, 83548, 85442, 87336, 89231, 91125, 93020, 94914,
    96808, 98703, 100597, 102492, 104386, 106280, 108175, 110069, 111964, 113858, 115752, 117647,
    119541, 121436, 123330, 125224, 127119, 129013, 130908, 132802, 134696, 136591, 138485, 140380,
    142274, 144168, 146063, 147957, 149852, 151746, 153640, 155535, 157429, 159324, 161218, 163840,
    163840, 163840, 163840, 163840, 163840, 163840, 163840, 163840, 163840, 163840, 163840, 163840,
    163840, 163840, 163840, 163840, 163840, 163840, 163840, 163840, 163840, 163840, 163840, 163840,
    163840, 163840, 163840, 163840, 163840, 163840, 163840, 163840 };
// Q14
static int16_t kBlocks128w256x[256] = {
    0, 201, 402, 603, 804, 1005, 1205, 1406, 1606, 1806, 2006,
    2205, 2404, 2603, 2801, 2999, 3196, 3393, 3590, 3786, 3981, 4176,
    4370, 4563, 4756, 4948, 5139, 5330, 5520, 5708, 5897, 6084, 6270,
    6455, 6639, 6823, 7005, 7186, 7366, 7545, 7723, 7900, 8076, 8250,
    8423, 8595, 8765, 8935, 9102, 9269, 9434, 9598, 9760, 9921, 10080,
    10238, 10394, 10549, 10702, 10853, 11003, 11151, 11297, 11442, 11585, 11727,
    11866, 12004, 12140, 12274, 12406, 12537, 12665, 12792, 12916, 13039, 13160,
    13279, 13395, 13510, 13623, 13733, 13842, 13949, 14053, 14155, 14256, 14354,
    14449, 14543, 14635, 14724, 14811, 14896, 14978, 15059, 15137, 15213, 15286,
    15357, 15426, 15493, 15557, 15619, 15679, 15736, 15791, 15843, 15893, 15941,
    15986, 16029, 16069, 16107, 16143, 16176, 16207, 16235, 16261, 16284, 16305,
    16324, 16340, 16353, 16364, 16373, 16379, 16383, 16384, 16383, 16379, 16373,
    16364, 16353, 16340, 16324, 16305, 16284, 16261, 16235, 16207, 16176, 16143,
    16107, 16069, 16029, 15986, 15941, 15893, 15843, 15791, 15736, 15679, 15619,
    15557, 15493, 15426, 15357, 15286, 15213, 15137, 15059, 14978, 14896, 14811,
    14724, 14635, 14543, 14449, 14354, 14256, 14155, 14053, 13949, 13842, 13733,
    13623, 13510, 13395, 13279, 13160, 13039, 12916, 12792, 12665, 12537, 12406,
    12274, 12140, 12004, 11866, 11727, 11585, 11442, 11297, 11151, 11003, 10853,
    10702, 10549, 10394, 10238, 10080, 9921, 9760, 9598, 9434, 9269, 9102,
    8935, 8765, 8595, 8423, 8250, 8076, 7900, 7723, 7545, 7366, 7186,
    7005, 6823, 6639, 6455, 6270, 6084, 5897, 5708, 5520, 5330, 5139,
    4948, 4756, 4563, 4370, 4176, 3981, 3786, 3590, 3393, 3196, 2999,
    2801, 2603, 2404, 2205, 2006, 1806, 1606, 1406, 1205, 1005, 804,
    603, 402, 201 };

#if FSmooth
/**
 * @brief 创建一个滑动平均滤波器并初始化
 * 
 * @param size 滑动窗口长度
 * @return MovingAverage* 滑动平均滤波器obj
 */
MovingAverage* movingAverageCreate(int32_t size) {
    MovingAverage* obj = (MovingAverage*)malloc(sizeof(MovingAverage));
    obj->size = size;
    obj->sum = 0;
    obj->queue = (int32_t*)malloc(sizeof(int32_t) * (size + 1));
    obj->front = 0;
    obj->rear = 0;
    return obj;
}

/**
 * @brief 对输入的值做滑动平均
 * 
 * @param obj 滑动平均滤波器
 * @param val 输入的下一个值
 * @return uint32_t 滑动平均后的值
 */
int32_t movingAverageNext(MovingAverage* obj, int32_t val) {
    int size = (obj->rear - obj->front + obj->size + 1) % (obj->size + 1);
    if (size == obj->size) {
        obj->sum -= obj->queue[obj->front];
        obj->front = (obj->front + 1) % (obj->size + 1);
        size--;
    }
    obj->queue[obj->rear] = val;
    obj->rear = (obj->rear + 1) % (obj->size + 1);
    obj->sum += val;
    size++;
    return (int32_t)(obj->sum / size);
}

/**
 * @brief 释放滑动平均滤波器的内存空间
 * 
 * @param obj 滑动平均滤波器
 */
void movingAverageFree(MovingAverage* obj) {
    free(obj->queue);
    free(obj);
}
#endif

/**
 * @brief 为ns结构体分配内存空间
 * 
 * @param NS_inst &(void *)
 * @return int 分配失败返回-1, 否则返回0
 */
int InnoTalkNsx_Create(void** NS_inst)
{
    NsxInst_t* stt;
    if (NS_inst == NULL)
    {
        return -1;
    }
    stt = (NsxInst_t*)malloc(sizeof(NsxInst_t));

    *NS_inst = stt;
    if (stt == NULL)
    {
        return -1;
    }
    return 0;
}

/**
 * @brief 初始化ns结构体
 * 
 * @param inst1 指向ns结构体的指针
 * @param fs 采样频率
 * @return int32_t 初始化失败返回-1, 否则返回0
 */
int32_t InnoTalkNsx_InitCore(void* inst1, uint32_t fs) {
    int i;
    NsxInst_t* inst = (NsxInst_t*)inst1;
    if (inst == NULL) {
        return -1;
    }
    // Initialization of struct
    inst->fs = fs;
    inst->blockLen = 128;
    inst->stages = 8;
    inst->blockIndex = -1; //frame counter
    inst->scaleEnergyIn = 0;
    inst->energyIn = 0;
    inst->zeroInputSignal = 0;
    inst->norm = 0;
    inst->normPrev = 15;

    memset(inst->analysisBuffer, 0, ANAL_BLOCKL_MAX * sizeof(int16_t));
    memset(inst->synthesisBuffer, 0, ANAL_BLOCKL_MAX * sizeof(int16_t));
    memset(inst->fftdata, 0, ANAL_BLOCKL_MAX * 2 * sizeof(int16_t));

    inst->overdrive16 = 42598;   // Q15(1.3)
    inst->denoiseBound16 = 2294; // Q15(0.07)

    memset(inst->smooth32, (int32_t)Q15MOD, sizeof(int32_t) * HALF_ANAL_BLOCKL);
    memset(inst->pmagnPrev32, 0, sizeof(int32_t) * HALF_ANAL_BLOCKL);
    memset(inst->noisePrev32, 0, sizeof(int32_t) * HALF_ANAL_BLOCKL);
    memset(inst->probPrev32, 0, sizeof(int32_t) * HALF_ANAL_BLOCKL);
    memset(inst->signalPrev32, 0, sizeof(int32_t) * HALF_ANAL_BLOCKL);
    memset(inst->minMagn32, 0, sizeof(int32_t) * HALF_ANAL_BLOCKL);

    inst->initFlag = 1;
    return 0;
}

/**
 * @brief 计算信号能量
 * 
 * @param vector 输入int16的信号
 * @param vector_length 信号长度
 * @param scale_factor 信号归一化尺度
 * @return uint64_t 信号能量
 */
uint64_t InnoTalk_Energy(int16_t* vector, int vector_length, int* scale_factor)
{
    uint64_t en = 0;
    int scaling = 0;
    int i;
    int looptimes = vector_length;
    int16_t* vectorptr = vector;

    for (i = 0; i < looptimes; i++)
    {
        en += ((uint64_t)((uint32_t)vectorptr[i] * (uint32_t)vectorptr[i])) >> scaling;
    }
    *scale_factor = scaling;
    return en;
}

/**
 * @brief 对输入信号进行加窗, 缓存, 计算能量, 移位以及FFT
 * 
 * @param inst1 ns结构体的指针
 * @param speechFrame 输入的一帧信号
 */
void InnoTalkNsx_DataAnalysis(void* inst1, short* speechFrame) {
    NsxInst_t* inst = (NsxInst_t*)inst1;
    int i;
    int16_t maxWinData;
    int16_t winData[ANAL_BLOCKL_MAX] = { 0 }, realImag[ANAL_BLOCKL_MAX] = { 0 };

    memcpy(inst->analysisBuffer, inst->analysisBuffer + inst->blockLen, (ANAL_BLOCKL_MAX - inst->blockLen) * sizeof(int16_t));
    memcpy(inst->analysisBuffer + ANAL_BLOCKL_MAX - inst->blockLen, speechFrame, inst->blockLen * sizeof(int16_t));

    for (i = 0; i < ANAL_BLOCKL_MAX; i++)
    {
        // 这里的winData是浮点版本的四舍五入的结果.
        winData[i] = ((int32_t)((int16_t)kBlocks128w256x[i] * (int16_t)inst->analysisBuffer[i]) + (int32_t)((int32_t)1 << (14 - 1))) >> 14;
        // winData[i] = ((int32_t)((int16_t)kBlocks128w256x[i] * (int16_t)inst->analysisBuffer[i])) >> 14;
    }

    // Get input energy
    inst->energyIn = InnoTalk_Energy(winData, ANAL_BLOCKL_MAX, &(inst->scaleEnergyIn));
    // Reset zero input flag
    inst->zeroInputSignal = 0;
    // Acquire norm for winData
    maxWinData = CEVA_TL4_vec_max_abs16_asm(winData, ANAL_BLOCKL_MAX); //找到fftdata的最大值
    inst->norm = InnoTalkSpl_NormW16(maxWinData);		//前导零的个数（最多是8+4+2+1）
    if (maxWinData == 0) {
        // Treat zero input separately.
        inst->zeroInputSignal = 1;
        return;
    }

    // Determine the net normalization in the frequency domain
    CEVA_TL4_vec_shf16_asm(winData, inst->norm, ANAL_BLOCKL_MAX, realImag);	//左移, Q(norm)

    for (i = 0; i < ANAL_BLOCKL_MAX; i++)
    {
        FFTIOt[i] = (int32_t)realImag[i];
        // FFTIOt[i] = (int32_t)winData[i];		//不用移位的数据
    }

    CEVA_TL4_fft_real_32_asm(inst->stages, FFTIOt, fftftemp, CEVA_TL4_cos_sin_fft_32, twi_table_32_rfft_256, bitrev, 1);
    for (i = 0; i < ANAL_BLOCKL_MAX * 2; i++)
    {
        inst->fftdata[i] = (int16_t)(fftftemp[i] >> inst->stages);
        // inst->fftdata[i] = (int16_t)(fftftemp[i]);
    }
}

/**
 * @brief 降噪算法的核心函数
 * 
 * @param inst1 ns结构体
 * @param speechFrame 输入的short型一帧信号
 * @param outFrame 输出的short型一帧降噪后信号
 * @return int 降噪失败返回-1, 否则返回0
 */
int InnoTalkNsx_ProcessCore(void* inst1, short* speechFrame, short* outFrame) {
    // main routine for noise suppression
    NsxInst_t* inst = (NsxInst_t*)inst1;
    if (inst->initFlag != 1) {
        return -1;
    }

    int i;
    int16_t  dTmp, Qdiff;
    int32_t  fTmp32;
    int64_t  tempdata64;

    int32_t  winDataI[HALF_ANAL_BLOCKL * 2];
    int32_t  winDataO[ANAL_BLOCKL_MAX];
    int16_t  real16[HALF_ANAL_BLOCKL], imag16[HALF_ANAL_BLOCKL];
    int32_t  magn32[HALF_ANAL_BLOCKL], noise32[HALF_ANAL_BLOCKL], pmagn32[HALF_ANAL_BLOCKL];
    int64_t  snrLocPost32[HALF_ANAL_BLOCKL] = { 0 };
    int64_t  snrLocPrior32[HALF_ANAL_BLOCKL] = { 0 };
    int32_t  probSpeechFinal32[HALF_ANAL_BLOCKL] = { 0 }; /* Q15 */
    int32_t  ii32[HALF_ANAL_BLOCKL] = { 0 }; /* Q15 */
    int32_t  theFilter32[HALF_ANAL_BLOCKL];
    int32_t  pr32[HALF_ANAL_BLOCKL] = { 0 }, afa32[HALF_ANAL_BLOCKL] = { 0 }; /* Q15 */
    int32_t  factor32, gain32;
    uint64_t energyOut;
#if FSmooth
    uint64_t fenergy1, fenergy2;
    int32_t  zeta;
    int32_t  winLen;
    MovingAverage* smooth;
#endif

    // Store speechFrame and transform to frequency domain
    InnoTalkNsx_DataAnalysis(inst, speechFrame);
    Qdiff = 2 * (inst->norm - inst->normPrev);

    if (inst->zeroInputSignal) {
        // synthesize the special case of zero input
        memcpy(outFrame, inst->synthesisBuffer, inst->blockLen * sizeof(int16_t));
        // update synthesis buffer
        INNOTALK_SPL_MEMCPY_W16(inst->synthesisBuffer,
            inst->synthesisBuffer + inst->blockLen,
            ANAL_BLOCKL_MAX - inst->blockLen);
        InnoTalkSpl_ZerosArrayW16(inst->synthesisBuffer + ANAL_BLOCKL_MAX - inst->blockLen,
            inst->blockLen);

        return 0;
    }
    inst->blockIndex++; // Update the block index only when we process a block.

    // Q(norm)
    imag16[0] = 0;
    real16[0] = inst->fftdata[0];
    imag16[HALF_ANAL_BLOCKL - 1] = 0;
    real16[HALF_ANAL_BLOCKL - 1] = inst->fftdata[ANAL_BLOCKL_MAX];
    // Q(2 * norm)
    magn32[0] = (int32_t)(real16[0] * real16[0]);
    magn32[HALF_ANAL_BLOCKL - 1] = (int32_t)(real16[HALF_ANAL_BLOCKL - 1] * real16[HALF_ANAL_BLOCKL - 1]);
#if FSmooth
    // 计算输入的频域能量, Q(2 * norm)
    fenergy1 = (uint64_t)magn32[0];
    fenergy1 += (uint64_t)magn32[HALF_ANAL_BLOCKL - 1];
#endif

    for (i = 1; i < HALF_ANAL_BLOCKL - 1; i++)
    {
        // Q(norm)
        real16[i] = inst->fftdata[2 * i];
        imag16[i] = inst->fftdata[2 * i + 1];
        // Q(2 * norm)
        fTmp32 =(int32_t)(real16[i] * real16[i]);
        fTmp32 += (int32_t)(imag16[i] * imag16[i]);
        magn32[i] = fTmp32;
#if FSmooth
        fenergy1 += (uint64_t)magn32[i];
#endif
    }

    // 如果是第一帧
    if (inst->blockIndex == 0)
    {
        memset(probSpeechFinal32, 0, sizeof(int32_t) * HALF_ANAL_BLOCKL);
        memcpy(inst->minMagn32, magn32, sizeof(int32_t) * HALF_ANAL_BLOCKL);
        memcpy(pmagn32, magn32, sizeof(int32_t) * HALF_ANAL_BLOCKL);
        memcpy(noise32, magn32, sizeof(int32_t) * HALF_ANAL_BLOCKL);
    }
    else
    // 非首帧
    {
        // norm < normPrev, 当前帧的magn右移位数更多, 则把上一帧数据再右移
        if (Qdiff < 0)
        {
            CEVA_TL4_vec_shf32_asm(inst->pmagnPrev32, Qdiff, HALF_ANAL_BLOCKL, inst->pmagnPrev32); //Q(2*norm)
            CEVA_TL4_vec_shf32_asm(inst->minMagn32, Qdiff, HALF_ANAL_BLOCKL, inst->minMagn32); //Q(2*norm)
            CEVA_TL4_vec_shf32_asm(inst->noisePrev32, Qdiff, HALF_ANAL_BLOCKL, inst->noisePrev32); //Q(2*norm)
            CEVA_TL4_vec_shf32_asm(inst->signalPrev32, Qdiff, HALF_ANAL_BLOCKL, inst->signalPrev32); //Q(2*norm)
        }
        // norm > normPrev, 上一帧帧的magn右移位数更多, 则把当前帧数据再右移
        else if (Qdiff > 0) // norm > normPrev, 转到normPrev
        {
            CEVA_TL4_vec_shf32_asm(magn32, -Qdiff, HALF_ANAL_BLOCKL, magn32);
        }

        for (i = 0; i < HALF_ANAL_BLOCKL; i++)
        {
            // 1.平滑功率谱 对应原理中最小值统计估噪的第(1)步即公式(1)
            tempdata64 = (int64_t)(SMOOTH_APY16 * (int64_t)inst->pmagnPrev32[i]) + (int64_t)((SMOOTH_APY16_S) * (int64_t)magn32[i]); //Q(min + 15)
            pmagn32[i] = (int32_t)(tempdata64 >> 15); //Q(min)

            // 2.搜索频带最小值 对应原理中最小值统计估噪的第(2)步
            if (inst->minMagn32[i] < pmagn32[i])
            {
                tempdata64 = (int64_t)(SMOOTH_R16 * (int64_t)inst->minMagn32[i]) + (int64_t)(1638 * ((int64_t)pmagn32[i] - (((int64_t)(SMOOTH_BETA16 * (int64_t)inst->pmagnPrev32[i])) >> 15))); //Q(min + 15)
                inst->minMagn32[i] = (int32_t)(tempdata64 >> 15); //Q(min)
            }
            else
            {
                inst->minMagn32[i] = pmagn32[i]; //Q(min)
            }

            // 3.判断是否存在语音 对应原理中最小值统计估噪的第(3)步
            if (inst->minMagn32[i] <= 0)
            {
                pr32[i] = delta32[i] + Q15MOD;
            }
            else
            {
                pr32[i] = (((int64_t)pmagn32[i]) << 15) / inst->minMagn32[i];
            }
            if (pr32[i] > delta32[i])
            {
                ii32[i] = (int32_t)Q15MOD;
            }
            else
            {
                ii32[i] = 0;
            }

            // 4.计算语音出现的概率 对应原理中最小值统计估噪的第(4)步
            tempdata64 = (int64_t)AP16 * inst->probPrev32[i] + (int32_t)(AP16_S) * ii32[i]; // Q15 * Q15
            probSpeechFinal32[i] = (int32_t)(tempdata64 >> 15); // Q15
        }

        // 5.噪声谱估计,对应原理中最小值统计估噪的第(5)步:
        if (inst->blockIndex < 7)
        {
            memcpy(noise32, pmagn32, HALF_ANAL_BLOCKL * sizeof(int32_t));
        }
        else
        {
            for (i = 0; i < HALF_ANAL_BLOCKL; i++)
            {
                tempdata64 = (int64_t)G16 * probSpeechFinal32[i] + (int64_t)966367641;//(0.9) * Q30MOD
                afa32[i] = (int32_t)(tempdata64 >> 15); //Q15
                tempdata64 = (int64_t)(afa32[i] * (int64_t)inst->noisePrev32[i]) + (int64_t)(Q15MOD - afa32[i]) * (int64_t)magn32[i]; // Q(min + 15)
                noise32[i] = (int32_t)(tempdata64 >> 15);  //Q(min)
            }
        }
    }

    // 将当前帧的值赋给前一帧
    memcpy(inst->probPrev32, probSpeechFinal32, HALF_ANAL_BLOCKL * sizeof(int32_t));
    if (Qdiff < 0)
    {
        memcpy(inst->pmagnPrev32, pmagn32, HALF_ANAL_BLOCKL * sizeof(int32_t));
        memcpy(inst->noisePrev32, noise32, HALF_ANAL_BLOCKL * sizeof(int32_t));
    }
    else
    {
        CEVA_TL4_vec_shf32_asm(pmagn32, Qdiff, HALF_ANAL_BLOCKL, inst->pmagnPrev32);
        CEVA_TL4_vec_shf32_asm(noise32, Qdiff, HALF_ANAL_BLOCKL, inst->noisePrev32);
        CEVA_TL4_vec_shf32_asm(inst->minMagn32, Qdiff, HALF_ANAL_BLOCKL, inst->minMagn32);
    }
    inst->normPrev = inst->norm;

    // 6. 计算先验/后验信噪比和维纳滤波器
    for (i = 0; i < HALF_ANAL_BLOCKL; i++)
    {
        if (inst->blockIndex == 0)
        {
            snrLocPrior32[i] = 31457; //0.96 * Q15MOD
            theFilter32[i] = 13919; //snrLocPrior[i] / (inst->overdrive + snrLocPrior[i]) in Q15MOD
        }
        else
        {
            if (noise32[i] == 0)
            {
                snrLocPrior32[i] = 30828; // 0.96*0.98 in Q15
                theFilter32[i] = 13757; //snrLocPrior[i] / (inst->overdrive + snrLocPrior[i]) in Q15MOD
            }
            else
            {
                snrLocPost32[i] = magn32[i]; //Q(min)
                tempdata64 = (int64_t)(AF16_S)*max((int32_t)snrLocPost32[i] - (int32_t)noise32[i], 0); //Q(min + 15)
                snrLocPrior32[i] = (int64_t)(AF16 * (int64_t)inst->signalPrev32[i]) + tempdata64; //Q(min + 15)
                tempdata64 = (int64_t)inst->overdrive16 * noise32[i]; //Q(min + 15)
                theFilter32[i] = ((int64_t)snrLocPrior32[i] * Q15MOD) / (tempdata64 + snrLocPrior32[i]); //Q((min + 30) - (min + 15)) = Q15
            }
        }

        if (theFilter32[i] > Q15MOD) // (int)(1.0* Q15MOD))
        {
            theFilter32[i] = Q15MOD; // (int)(1.0 * Q15MOD);
        }
        if (theFilter32[i] < inst->denoiseBound16) //(int)(0.07 * Q15MOD))		//denoise 系数
        {
            theFilter32[i] = inst->denoiseBound16;// (int)(0.07 * Q15MOD);
        }

        tempdata64 = ((int64_t)(((int64_t)magn32[i] * (int64_t)theFilter32[i]) >> 15)); // Q(min + 15 - 15)
        inst->signalPrev32[i] = (int32_t)(((int64_t)theFilter32[i] * tempdata64) >> 15);  // Q(min + 15 - 15)
    }
    if (Qdiff > 0)
    {
        CEVA_TL4_vec_shf32_asm(inst->signalPrev32, Qdiff, HALF_ANAL_BLOCKL, inst->signalPrev32);
    }


    // 7. 抑制水声
    for ( i = 0; i < HALF_ANAL_BLOCKL; i++)
    {
        tempdata64 = (int64_t)(FILTER_SMOOTH * (int64_t)inst->smooth32[i]) + (int64_t)(FILTER_SMOOTH_S * (int64_t)theFilter32[i]);
        inst->smooth32[i] = (int32_t)(tempdata64 >> 15);
        // inst->smooth32[i] = theFilter32[i];
    }
#if FSmooth
    fenergy2 = 0;
    for (i = 0; i < HALF_ANAL_BLOCKL; i++)
    {
        fenergy2 += (uint64_t)inst->signalPrev32[i];
    }
    zeta = (fenergy2 << 15) / fenergy1;
    if (zeta > ZETA_THR)
    {
        zeta = (int32_t)Q15MOD;
    }
    if (zeta == (int32_t)Q15MOD)
    {
        winLen = 1;
    }
    else
    {
        winLen = 2 * ((((Q15MOD - (zeta << 15) / ZETA_THR) * PSI) + Q15MOD) >> 15) + 1;
        smooth = movingAverageCreate(winLen);
        for (i = 0; i < HALF_ANAL_BLOCKL; i++)
        {
            theFilter32[i] = movingAverageNext(smooth, theFilter32[i]);
        }
        movingAverageFree(smooth);
    }
    memcpy(inst->smooth32, theFilter32, HALF_ANAL_BLOCKL * sizeof(int32_t));
#endif
    for (i = 0; i < HALF_ANAL_BLOCKL; i++)
    {
        // 8. 增强语音谱
        real16[i] = ((int32_t)(real16[i] * inst->smooth32[i])) >> 15;
        imag16[i] = ((int32_t)(imag16[i] * inst->smooth32[i])) >> 15;
    }

    // 转时域
    winDataI[0] = (int32_t)(real16[0]);
    winDataI[1] = 0;
    winDataI[ANAL_BLOCKL_MAX] = (int32_t)(real16[HALF_ANAL_BLOCKL - 1]);
    winDataI[ANAL_BLOCKL_MAX + 1] = 0;
    for (i = 1; i < HALF_ANAL_BLOCKL - 1; i++) {
        winDataI[2 * i] = (int32_t)(real16[i]);
        winDataI[2 * i + 1] = (int32_t)(imag16[i]);
    }
    CEVA_TL4_fft_real_inv_32_asm(inst->stages, winDataI, winDataO, CEVA_TL4_cos_sin_fft_32, twi_table_32_rfft_256, bitrev, 1);
    CEVA_TL4_vec_shf32_asm(winDataO, -inst->norm, ANAL_BLOCKL_MAX, winDataO);

    factor32 = Q15MOD;
    if (inst->blockIndex > 0) {
        energyOut = 0;
        for (i = 0; i < ANAL_BLOCKL_MAX; i++) {
            energyOut += (uint64_t)(winDataO[i] * winDataO[i]);
        }
        gain32 = CEVA_TL4_sqrt_int32_asm((energyOut << 30) / (inst->energyIn + 32768), 1);

        if (gain32 > B_LIM16) {
            factor32 = (int32_t)Q15MOD + ((42598 * (gain32 - B_LIM16)) >> 15);
        }
        else {
            factor32 = (int32_t)Q15MOD + 2 * (gain32 - B_LIM16);
        }
        if (gain32 * factor32 > (int64_t)Q30MOD) {
            factor32 = (int64_t)Q30MOD / gain32;
        }
    }

    for (i = 0; i < ANAL_BLOCKL_MAX; i++) {
        inst->synthesisBuffer[i] += (short)(((int64_t)factor32 * (int32_t)kBlocks128w256x[i] * (int32_t)winDataO[i]) >> 29);
    }

    for (i = 0; i < inst->blockLen; i++) {
        dTmp = (short)inst->synthesisBuffer[i];
        outFrame[i] = dTmp;
    }
    // update synthesis buffer
    memcpy(inst->synthesisBuffer, inst->synthesisBuffer + inst->blockLen, (ANAL_BLOCKL_MAX - inst->blockLen) * sizeof(int16_t));
    memset(inst->synthesisBuffer + ANAL_BLOCKL_MAX - inst->blockLen, 0, (inst->blockLen) * sizeof(int16_t));

    return 0;
}

