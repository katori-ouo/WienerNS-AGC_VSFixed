/*
 * agc.c
 *
 *  Created on: 2021年4月26日
 *      Author: w
 */
#include <stdlib.h>
#include <stddef.h>
#include "agc.h"
//#include "config.h"


//__attribute__((section(".DSECT inno_test_a_DATA")))   // 380 B
Agc_t agcFixed;


//__attribute__((section(".DSECT inno_test_a_DATA")))  //Total = 256 B
int32_t kTargetLevelTable[64] = {134209536, 106606424, 84680493, 67264106,
        53429779, 42440782, 33711911, 26778323, 21270778, 16895980, 13420954, 10660642,
        8468049, 6726411, 5342978, 4244078, 3371191, 2677832, 2127078, 1689598, 1342095,
        1066064, 846805, 672641, 534298, 424408, 337119, 267783, 212708, 168960, 134210,
        106606, 84680, 67264, 53430, 42441, 33712, 26778, 21271, 16896, 13421, 10661, 8468,
        6726, 5343, 4244, 3371, 2678, 2127, 1690, 1342, 1066, 847, 673, 534, 424, 337, 268,
        213, 169, 134, 107, 85, 67};

//__attribute__((section(".DSECT inno_test_a_DATA")))  // Total = 256 bytes
uint16_t kGenFuncTable[kGenFuncTableSize] = {
          256,   485,   786,  1126,  1484,  1849,  2217,  2586,
         2955,  3324,  3693,  4063,  4432,  4801,  5171,  5540,
         5909,  6279,  6648,  7017,  7387,  7756,  8125,  8495,
         8864,  9233,  9603,  9972, 10341, 10711, 11080, 11449,
        11819, 12188, 12557, 12927, 13296, 13665, 14035, 14404,
        14773, 15143, 15512, 15881, 16251, 16620, 16989, 17359,
        17728, 18097, 18466, 18836, 19205, 19574, 19944, 20313,
        20682, 21052, 21421, 21790, 22160, 22529, 22898, 23268,
        23637, 24006, 24376, 24745, 25114, 25484, 25853, 26222,
        26592, 26961, 27330, 27700, 28069, 28438, 28808, 29177,
        29546, 29916, 30285, 30654, 31024, 31393, 31762, 32132,
        32501, 32870, 33240, 33609, 33978, 34348, 34717, 35086,
        35456, 35825, 36194, 36564, 36933, 37302, 37672, 38041,
        38410, 38780, 39149, 39518, 39888, 40257, 40626, 40996,
        41365, 41734, 42104, 42473, 42842, 43212, 43581, 43950,
        44320, 44689, 45058, 45428, 45797, 46166, 46536, 46905
};

//__attribute__((section(".DSECT inno_test_a_DATA")))  // 6 bytes
uint16_t kResampleAllpass1[3] = {3284, 24441, 49528};

//__attribute__((section(".DSECT inno_test_a_DATA")))  // 6 bytes
uint16_t kResampleAllpass2[3] = {12199, 37471, 60255};


int InnoTalkAgc_Sat(short * x, short * y, short framesz, short a)
{
	int i, temp;
	if (a == 10)
	{
		for (i=0; i<framesz; i++)	{
			if (x[i] > 3000)	    {
				temp = (int)x[i]*2260;
				temp = temp >> 15;
				y[i] = (short)(temp + 29793);
			}
			else if (x[i] < -3000)	    {
				temp = (int)x[i]*2260;
				temp = temp >> 15;
				y[i] = (short)(temp - 29793);
			}
			else
			{
				y[i] = x[i]*10;
			}
		}
	}
	else if (a == 6)
	{
		for (i=0; i<framesz; i++)	{
			if (x[i] > 4000)	    {
				temp = (int)x[i]*9362;
				temp = temp >> 15;
				y[i] = (short)(temp + 22857);
			}
			else if (x[i] < -4000)	{
				temp = (int)x[i]*9362;
				temp = temp >> 15;
				y[i] = (short)(temp - 22857);
			}
			else
			{
				y[i] = x[i]*6;
			}
		}
	}
	else if (a == 5)
	{
		for (i=0; i<framesz; i++)	{
			if (x[i] > 5000)	    {
				temp = (int)x[i]*8495;
				temp = temp >> 15;
				y[i] = (short)(temp + 23703);
			}
			else if (x[i] < -5000)	    {
				temp = (int)x[i]*8495;
				temp = temp >> 15;
				y[i] = (short)(temp - 23703);
			}
			else
			{
				y[i] = x[i]*5;
			}
		}
	}
	else if (a == 4)
	{
		for (i=0; i<framesz; i++)	{
			if (x[i] > 6000)	    {
				temp = (int)x[i]*10082;
				temp = temp >> 15;
				y[i] = (short)(temp + 22154);
			}
			else if (x[i] < -6000)	    {
				temp = (int)x[i]*10082;
				temp = temp >> 15;
				y[i] = (short)(temp - 22154);
			}
			else
			{
				y[i] = x[i]*4;
			}
		}
	}
	else if (a == 3)
	{
		for (i=0; i<framesz; i++)	{
			if (x[i] > 8000)	    {
				temp = (int)x[i]*10923;
				temp = temp >> 15;
				y[i] = (short)(temp + 21333);
			}
			else if (x[i] < -8000)	    {
				temp = (int)x[i]*10923;
				temp = temp >> 15;
				y[i] = (short)(temp - 21333);
			}
			else
			{
				y[i] = x[i]*3;
			}
		}
	}
	else if (a == 2)
	{
		for (i=0; i<framesz; i++)	{
			if (x[i] > 12000)	    {
				temp = (int)x[i]*13834;
				temp = temp >> 15;
				y[i] = (short)(temp + 18934);
			}
			else if (x[i] < -8000)	    {
				temp = (int)x[i]*13834;
				temp = temp >> 15;
				y[i] = (short)(temp - 18934);
			}
			else
			{
				y[i] = x[i]*2;
			}
		}
	}
    return 0;

}

int32_t InnoTalkAgc_CalculateGainTable(int32_t *gainTable, // Q16
                                     int16_t digCompGaindB, // Q0
                                     int16_t targetLevelDbfs,// Q0
                                     uint8_t limiterEnable,
                                     int16_t analogTarget) // Q0
{

    // This function generates the compressor gain table used in the fixed digital part.
    uint32_t tmpU32no1, tmpU32no2, absInLevel, logApprox;
    int32_t inLevel, limiterLvl;
    int32_t tmp32, tmp32no1, tmp32no2, numFIX, den, y32;
    const uint16_t kLog10 = 54426; // log2(10)     in Q14
    const uint16_t kLog10_2 = 49321; // 10*log10(2)  in Q14
    const uint16_t kLogE_1 = 23637; // log2(e)      in Q14
    uint16_t constMaxGain;
    uint16_t tmpU16, intPart, fracPart;
    const int16_t kCompRatio = 3;
    const int16_t kSoftLimiterLeft = 1;
    int16_t limiterOffset = 0; // Limiter offset
    int16_t limiterIdx, limiterLvlX;
    int16_t constLinApprox, zeroGainLvl, maxGain, diffGain;
    int16_t i, tmp16, tmp16no1;
    int zeros, zerosScale;

    // Constants
//    kLogE_1 = 23637; // log2(e)      in Q14
//    kLog10 = 54426; // log2(10)     in Q14
//    kLog10_2 = 49321; // 10*log10(2)  in Q14

    // Calculate maximum digital gain and zero gain level
    tmp32no1 = INNOTALK_SPL_MUL_16_16(digCompGaindB - analogTarget, kCompRatio - 1);
    tmp16no1 = analogTarget - targetLevelDbfs;
    tmp16no1 += InnoTalkSpl_DivW32W16ResW16(tmp32no1 + (kCompRatio >> 1), kCompRatio);
    maxGain = INNOTALK_SPL_MAX(tmp16no1, (analogTarget - targetLevelDbfs));
    tmp32no1 = INNOTALK_SPL_MUL_16_16(maxGain, kCompRatio);
    zeroGainLvl = digCompGaindB;
    zeroGainLvl -= InnoTalkSpl_DivW32W16ResW16(tmp32no1 + ((kCompRatio - 1) >> 1),
                                             kCompRatio - 1);
    if ((digCompGaindB <= analogTarget) && (limiterEnable))
    {
        zeroGainLvl += (analogTarget - digCompGaindB + kSoftLimiterLeft);
        limiterOffset = 0;
    }

    // Calculate the difference between maximum gain and gain at 0dB0v:
    //  diffGain = maxGain + (compRatio-1)*zeroGainLvl/compRatio
    //           = (compRatio-1)*digCompGaindB/compRatio
    tmp32no1 = INNOTALK_SPL_MUL_16_16(digCompGaindB, kCompRatio - 1);
    diffGain = InnoTalkSpl_DivW32W16ResW16(tmp32no1 + (kCompRatio >> 1), kCompRatio);
    if (diffGain < 0 || diffGain >= kGenFuncTableSize)
    {
        return -1;
    }

    // Calculate the limiter level and index:
    //  limiterLvlX = analogTarget - limiterOffset
    //  limiterLvl  = targetLevelDbfs + limiterOffset/compRatio
    limiterLvlX = analogTarget - limiterOffset;
    limiterIdx = 2
            + InnoTalkSpl_DivW32W16ResW16(INNOTALK_SPL_LSHIFT_W32((int32_t)limiterLvlX, 13),
                                        INNOTALK_SPL_RSHIFT_U16(kLog10_2, 1));
    tmp16no1 = InnoTalkSpl_DivW32W16ResW16(limiterOffset + (kCompRatio >> 1), kCompRatio);
    limiterLvl = targetLevelDbfs + tmp16no1;

    // Calculate (through table lookup):
    //  constMaxGain = log2(1+2^(log2(e)*diffGain)); (in Q8)
    constMaxGain = kGenFuncTable[diffGain]; // in Q8

    // Calculate a parameter used to approximate the fractional part of 2^x with a
    // piecewise linear function in Q14:
    //  constLinApprox = round(3/2*(4*(3-2*sqrt(2))/(log(2)^2)-0.5)*2^14);
    constLinApprox = 22817; // in Q14

    // Calculate a denominator used in the exponential part to convert from dB to linear scale:
    //  den = 20*constMaxGain (in Q8)
    den = INNOTALK_SPL_MUL_16_U16(20, constMaxGain); // in Q8

    for (i = 0; i < 32; i++)
    {
        // Calculate scaled input level (compressor):
        //  inLevel = fix((-constLog10_2*(compRatio-1)*(1-i)+fix(compRatio/2))/compRatio)
        tmp16 = (int16_t)INNOTALK_SPL_MUL_16_16(kCompRatio - 1, i - 1); // Q0
        tmp32 = INNOTALK_SPL_MUL_16_U16(tmp16, kLog10_2) + 1; // Q14
        inLevel = InnoTalkSpl_DivW32W16(tmp32, kCompRatio); // Q14

        // Calculate diffGain-inLevel, to map using the genFuncTable
        inLevel = INNOTALK_SPL_LSHIFT_W32((int32_t)diffGain, 14) - inLevel; // Q14

        // Make calculations on abs(inLevel) and compensate for the sign afterwards.
        absInLevel = (uint32_t)INNOTALK_SPL_ABS_W32(inLevel); // Q14

        // LUT with interpolation
        intPart = (uint16_t)INNOTALK_SPL_RSHIFT_U32(absInLevel, 14);
        fracPart = (uint16_t)(absInLevel & 0x00003FFF); // extract the fractional part
        tmpU16 = kGenFuncTable[intPart + 1] - kGenFuncTable[intPart]; // Q8
        tmpU32no1 = INNOTALK_SPL_UMUL_16_16(tmpU16, fracPart); // Q22
        tmpU32no1 += INNOTALK_SPL_LSHIFT_U32((uint32_t)kGenFuncTable[intPart], 14); // Q22
        logApprox = INNOTALK_SPL_RSHIFT_U32(tmpU32no1, 8); // Q14
        // Compensate for negative exponent using the relation:
        //  log2(1 + 2^-x) = log2(1 + 2^x) - x
        if (inLevel < 0)
        {
            zeros = InnoTalkSpl_NormU32(absInLevel);
            zerosScale = 0;
            if (zeros < 15)
            {
                // Not enough space for multiplication
                tmpU32no2 = INNOTALK_SPL_RSHIFT_U32(absInLevel, 15 - zeros); // Q(zeros-1)
                tmpU32no2 = INNOTALK_SPL_UMUL_32_16(tmpU32no2, kLogE_1); // Q(zeros+13)
                if (zeros < 9)
                {
                    tmpU32no1 = INNOTALK_SPL_RSHIFT_U32(tmpU32no1, 9 - zeros); // Q(zeros+13)
                    zerosScale = 9 - zeros;
                } else
                {
                    tmpU32no2 = INNOTALK_SPL_RSHIFT_U32(tmpU32no2, zeros - 9); // Q22
                }
            } else
            {
                tmpU32no2 = INNOTALK_SPL_UMUL_32_16(absInLevel, kLogE_1); // Q28
                tmpU32no2 = INNOTALK_SPL_RSHIFT_U32(tmpU32no2, 6); // Q22
            }
            logApprox = 0;
            if (tmpU32no2 < tmpU32no1)
            {
                logApprox = INNOTALK_SPL_RSHIFT_U32(tmpU32no1 - tmpU32no2, 8 - zerosScale); //Q14
            }
        }
        numFIX = INNOTALK_SPL_LSHIFT_W32(INNOTALK_SPL_MUL_16_U16(maxGain, constMaxGain), 6); // Q14
        numFIX -= INNOTALK_SPL_MUL_32_16((int32_t)logApprox, diffGain); // Q14

        // Calculate ratio
        // Shift |numFIX| as much as possible.
        // Ensure we avoid wrap-around in |den| as well.
        if (numFIX > (den >> 8))  // |den| is Q8.
        {
            zeros = InnoTalkSpl_NormW32(numFIX);
        } else
        {
            zeros = InnoTalkSpl_NormW32(den) + 8;
        }
        numFIX = INNOTALK_SPL_LSHIFT_W32(numFIX, zeros); // Q(14+zeros)

        // Shift den so we end up in Qy1
        tmp32no1 = INNOTALK_SPL_SHIFT_W32(den, zeros - 8); // Q(zeros)
        if (numFIX < 0)
        {
            numFIX -= INNOTALK_SPL_RSHIFT_W32(tmp32no1, 1);
        } else
        {
            numFIX += INNOTALK_SPL_RSHIFT_W32(tmp32no1, 1);
        }
        y32 = INNOTALK_SPL_DIV(numFIX, tmp32no1); // in Q14
        if (limiterEnable && (i < limiterIdx))
        {
            tmp32 = INNOTALK_SPL_MUL_16_U16(i - 1, kLog10_2); // Q14
            tmp32 -= INNOTALK_SPL_LSHIFT_W32(limiterLvl, 14); // Q14
            y32 = InnoTalkSpl_DivW32W16(tmp32 + 10, 20);
        }
        if (y32 > 39000)
        {
            tmp32 = INNOTALK_SPL_MUL(y32 >> 1, kLog10) + 4096; // in Q27
            tmp32 = INNOTALK_SPL_RSHIFT_W32(tmp32, 13); // in Q14
        } else
        {
            tmp32 = INNOTALK_SPL_MUL(y32, kLog10) + 8192; // in Q28
            tmp32 = INNOTALK_SPL_RSHIFT_W32(tmp32, 14); // in Q14
        }
        tmp32 += INNOTALK_SPL_LSHIFT_W32(16, 14); // in Q14 (Make sure final output is in Q16)

        // Calculate power
        if (tmp32 > 0)
        {
            intPart = (int16_t)INNOTALK_SPL_RSHIFT_W32(tmp32, 14);
            fracPart = (uint16_t)(tmp32 & 0x00003FFF); // in Q14
            if (INNOTALK_SPL_RSHIFT_W32(fracPart, 13))
            {
                tmp16 = INNOTALK_SPL_LSHIFT_W16(2, 14) - constLinApprox;
                tmp32no2 = INNOTALK_SPL_LSHIFT_W32(1, 14) - fracPart;
                tmp32no2 = INNOTALK_SPL_MUL_32_16(tmp32no2, tmp16);
                tmp32no2 = INNOTALK_SPL_RSHIFT_W32(tmp32no2, 13);
                tmp32no2 = INNOTALK_SPL_LSHIFT_W32(1, 14) - tmp32no2;
            } else
            {
                tmp16 = constLinApprox - INNOTALK_SPL_LSHIFT_W16(1, 14);
                tmp32no2 = INNOTALK_SPL_MUL_32_16(fracPart, tmp16);
                tmp32no2 = INNOTALK_SPL_RSHIFT_W32(tmp32no2, 13);
            }
            fracPart = (uint16_t)tmp32no2;
            gainTable[i] = INNOTALK_SPL_LSHIFT_W32(1, intPart)
                    + INNOTALK_SPL_SHIFT_W32(fracPart, intPart - 14);
        } else
        {
            gainTable[i] = 0;
        }
    }

    return 0;
}

int InnoTalkAgc_set_config(void *agcInst, InnoTalkAgc_config_t agcConfig)
{

    Agc_t *stt;
    stt = (Agc_t *)agcInst;

    if (stt == NULL)
    {
        return -1;
    }

    stt->limiterEnable = agcConfig.limiterEnable;
    stt->compressionGaindB = agcConfig.compressionGaindB;
    stt->targetLevelDbfs = agcConfig.targetLevelDbfs;

    // Update threshold levels for analog adaptation
    InnoTalkAgc_UpdateAgcThresholds(stt);

    // Recalculate gain table
    if (InnoTalkAgc_CalculateGainTable(&(stt->digitalAgc.gainTable[0]), stt->compressionGaindB,
                           stt->targetLevelDbfs, stt->limiterEnable, stt->analogTarget) == -1)
    {
        return -1;
    }

    return 0;
}


void InnoTalkAgc_InitVad(AgcVad_t *state)
{

    int16_t k;

    state->HPstate = 0; // state of high pass filter
    state->logRatio = 0; // log( P(active) / P(inactive) )
    // average input level (Q10)
    state->meanLongTerm = INNOTALK_SPL_LSHIFT_W16(15, 10);

    // variance of input level (Q8)
    state->varianceLongTerm = INNOTALK_SPL_LSHIFT_W32(500, 8);

    state->stdLongTerm = 0; // standard deviation of input level in dB
    // short-term average input level (Q10)
    state->meanShortTerm = INNOTALK_SPL_LSHIFT_W16(15, 10);

    // short-term variance of input level (Q8)
    state->varianceShortTerm = INNOTALK_SPL_LSHIFT_W32(500, 8);

    state->stdShortTerm = 0; // short-term standard deviation of input level in dB
    state->counter = 3; // counts updates
    for (k = 0; k < 8; k++)
    {
        // downsampling filter
        state->downState[k] = 0;
    }

}

int InnoTalkAgc_Create(void **agcInst)
{

    Agc_t *stt = &agcFixed.fs;

	*agcInst = stt;

    stt->initFlag = 0;
    stt->lastError = 0;

    return 0;
}

int32_t InnoTalkAgc_InitDigital(DigitalAgc_t *stt, int16_t agcMode)
{

    // start out with 0 dB gain
	stt->capacitorSlow = 134217728; // (int32_t)(0.125f * 32768.0f * 32768.0f);

    stt->capacitorFast = 0;
    stt->gain = 65536;
    stt->gatePrevious = 0;
    stt->agcMode = agcMode;

    // initialize VADs
    InnoTalkAgc_InitVad(&stt->vadNearend);

    return 0;
}

int InnoTalkAgc_Init(void *agcInst, int32_t minLevel, int32_t maxLevel,
                    uint32_t fs)
{

    int32_t max_add, tmp32;

    int tmpNorm;
    Agc_t *stt;

    // typecast state pointer
    stt = (Agc_t *)agcInst;

    if (InnoTalkAgc_InitDigital(&stt->digitalAgc, kAgcModeAdaptiveDigital) != 0)
    {
        return -1;
    }

    // Analog AGC variables
    //stt->envSum = 0;

    // mode     = 0 - Only saturation protection
    //            1 - Analog Automatic Gain Control [-targetLevelDbfs (default -3 dBOv)]
    //            2 - Digital Automatic Gain Control [-targetLevelDbfs (default -3 dBOv)]
    //            3 - Fixed Digital Gain [compressionGaindB (default 8 dB)]

    stt->agcMode = kAgcModeAdaptiveDigital;
    stt->fs = fs;

    // initialize input VAD
    InnoTalkAgc_InitVad(&stt->vadMic);

    // If the volume range is smaller than 0-256 then
    // the levels are shifted up to Q8-domain
    tmpNorm = InnoTalkSpl_NormU32((uint32_t)maxLevel);
    stt->scale = tmpNorm - 23;
    if (stt->scale < 0)
    {
        stt->scale = 0;
    }
    // TODO(bjornv): Investigate if we really need to scale up a small range now when we have
    // a guard against zero-increments. For now, we do not support scale up (scale = 0).
    stt->scale = 0;
    maxLevel = INNOTALK_SPL_LSHIFT_W32(maxLevel, stt->scale);
    minLevel = INNOTALK_SPL_LSHIFT_W32(minLevel, stt->scale);

    // Make minLevel and maxLevel static in AdaptiveDigital
    if (stt->agcMode == kAgcModeAdaptiveDigital)
    {
        minLevel = 0;
        maxLevel = 255;
        stt->scale = 0;
    }
    // The maximum supplemental volume range is based on a vague idea
    // of how much lower the gain will be than the real analog gain.
    max_add = INNOTALK_SPL_RSHIFT_W32(maxLevel - minLevel, 2);

    // Minimum/maximum volume level that can be set
    stt->minLevel = minLevel;
    stt->maxAnalog = maxLevel;
    stt->maxLevel = maxLevel + max_add;
    stt->maxInit = stt->maxLevel;

    stt->zeroCtrlMax = stt->maxAnalog;

    // Initialize micVol parameter
    stt->micVol = stt->maxAnalog;
    if (stt->agcMode == kAgcModeAdaptiveDigital)
    {
        stt->micVol = 127; // Mid-point of mic level
    }
    stt->micRef = stt->micVol;
    //stt->micGainIdx = 127;
	stt->micGainIdx = 10;

    // Minimum output volume is 4% higher than the available lowest volume level
    tmp32 = INNOTALK_SPL_RSHIFT_W32((stt->maxLevel - stt->minLevel) * (int32_t)10, 8);
    stt->minOutput = (stt->minLevel + tmp32);

    InnoTalkSpl_MemSetW32(stt->filterState, 0, 8);

    stt->initFlag = kInitCheck;
    // Default config settings.
    //stt->defaultConfig.limiterEnable = kAgcTrue;
    //stt->defaultConfig.targetLevelDbfs = AGC_DEFAULT_TARGET_LEVEL;
    //stt->defaultConfig.compressionGaindB = AGC_DEFAULT_COMP_GAIN;
    //stt->Rxx160_LPw32 = stt->analogTargetLevel; // Initialize rms value
    stt->lowLevelSignal = 0;

    // Only positive values are allowed that are not too large
    if ((minLevel >= maxLevel) || (maxLevel & 0xFC000000))
    {
        return -1;
    } else
    {
        return 0;
    }

}

void InnoTalkAgc_UpdateAgcThresholds(Agc_t *stt)
{

    int16_t tmp16;
    // Set analog target level in envelope dBOv scale
    tmp16 = (DIFF_REF_TO_ANALOG * stt->compressionGaindB) + ANALOG_TARGET_LEVEL_2;
    tmp16 = InnoTalkSpl_DivW32W16ResW16((int32_t)tmp16, ANALOG_TARGET_LEVEL);
    stt->analogTarget = DIGITAL_REF_AT_0_COMP_GAIN + tmp16;
    if (stt->analogTarget < DIGITAL_REF_AT_0_COMP_GAIN)
    {
        stt->analogTarget = DIGITAL_REF_AT_0_COMP_GAIN;
    }

    // Since the offset between RMS and ENV is not constant, we should make this into a
    // table, but for now, we'll stick with a constant, tuned for the chosen analog
    // target level.

    stt->targetIdx = ANALOG_TARGET_LEVEL + OFFSET_ENV_TO_RMS;
    // Analog adaptation limits
    // analogTargetLevel = round((32767*10^(-targetIdx/20))^2*16/2^7)
    stt->analogTargetLevel = RXX_BUFFER_LEN * kTargetLevelTable[stt->targetIdx]; // ex. -20 dBov
    stt->startUpperLimit = RXX_BUFFER_LEN * kTargetLevelTable[stt->targetIdx - 1];// -19 dBov
    stt->startLowerLimit = RXX_BUFFER_LEN * kTargetLevelTable[stt->targetIdx + 1];// -21 dBov
    stt->upperPrimaryLimit = RXX_BUFFER_LEN * kTargetLevelTable[stt->targetIdx - 2];// -18 dBov
    stt->lowerPrimaryLimit = RXX_BUFFER_LEN * kTargetLevelTable[stt->targetIdx + 2];// -22 dBov
    stt->upperSecondaryLimit = RXX_BUFFER_LEN * kTargetLevelTable[stt->targetIdx - 5];// -15 dBov
    stt->lowerSecondaryLimit = RXX_BUFFER_LEN * kTargetLevelTable[stt->targetIdx + 5];// -25 dBov
    stt->upperLimit = stt->startUpperLimit;
    stt->lowerLimit = stt->startLowerLimit;

}

int InnoTalkAgc_Process(void *agcInst, const int16_t *in_near,
                      int16_t *out,
					  InnoTalkAgc_config_t agcConfig)
{

    Agc_t *stt;
    // int16_t i;

    int32_t out_tmp, tmp32;
    int32_t env[10];
    int32_t gains[9];
    int32_t nrg, max_nrg;
    int32_t cur_level;
    int32_t gain32, delta;
    int16_t logratio;//,Record;
    int16_t lower_thr, upper_thr;
    int16_t zeros, zeros_fast, frac;
    int16_t decay;
    int16_t gate, gain_adj;
    int16_t k, n;
    int16_t L, L2; // samples/subframe

    stt = (Agc_t *)agcInst;

    uint16_t tmpU16;
    int32_t tmp32b;
    int16_t subfr, tmp16;
    int16_t buf1[8];
    int16_t buf2[4];
    int16_t HPstate;
    int16_t dB;
    DigitalAgc_t * sttd;

    sttd = &stt->digitalAgc;

    AgcVad_t * state;

    //////////////WQY start
    // determine number of samples per ms  //JT:定义子帧长度
    L = 16;
    L2 = 4;

    // TODO(andrew): again, we don't need input and output pointers...
    if (in_near != out)
    {
        // Only needed if they don't already point to the same place.
        memcpy(out, in_near, 8 * L * sizeof(int16_t));
    }

    nrg = 0;
    int16_t * out1;
    out1 = out;
    state = &sttd->vadNearend;
    HPstate = state->HPstate;
    for (subfr = 0; subfr < 8; subfr++)//处理128点（一帧），分8个循环
    {
        for (k = 0; k < 8; k++)//
        {
            tmp32 = (int32_t)out1[2 * k] + (int32_t)out1[2 * k + 1];//jt：每两点求和
            tmp32 = INNOTALK_SPL_RSHIFT_W32(tmp32, 1);//jt：除以2，求平均
            buf1[k] = (int16_t)tmp32;//如此，16点降到的8点
        }
        out1 += 16;

        InnoTalkSpl_DownsampleBy2(buf1, 8, buf2, state->downState);//再降到4个点

        //上述代码输出一个4k的信号流（1ms对应4个点）
        // high pass filter and compute energy
        for (k = 0; k < 4; k++)//jt：实现一个二阶差分方程，频响为高通，截止频率为：//
        {
            out_tmp = buf2[k] + HPstate;
            tmp32 = INNOTALK_SPL_MUL(600, out_tmp);
            HPstate = (int16_t)(INNOTALK_SPL_RSHIFT_W32(tmp32, 10) - buf2[k]);
            tmp32 = INNOTALK_SPL_MUL(out_tmp, out_tmp);
            nrg += INNOTALK_SPL_RSHIFT_W32(tmp32, 6);
        }
    }
    state->HPstate = HPstate;

    // find number of leading zeros//jt：确定nrg32位的前面有多少个0
    if (!(0xFFFF0000 & nrg))
    {
        zeros = 16;
    } else
    {
       zeros = 0;
    }
    if (!(0xFF000000 & (nrg << zeros)))
    {
        zeros += 8;
    }
    if (!(0xF0000000 & (nrg << zeros)))
    {
        zeros += 4;
    }
    if (!(0xC0000000 & (nrg << zeros)))
    {
        zeros += 2;
    }
    if (!(0x80000000 & (nrg << zeros)))
    {
       zeros += 1;
    }
    // energy level (range {-32..30}) (Q10)
    dB = INNOTALK_SPL_LSHIFT_W16(15 - zeros, 11);

    // Update statistics
    if (state->counter < kAvgDecayTime)
    {
       // decay time = AvgDecTime * 10 ms
       state->counter++;
    }

    // update short-term estimate of mean energy level (Q10)
    tmp32 = (INNOTALK_SPL_MUL_16_16(state->meanShortTerm, 15) + (int32_t)dB);//jt：DB域上的平滑，平滑因子15/16，这里为什么不直接加当前帧的能量是因为转换对数时需要消耗计算量，因此可以通过统计数据位数来估算dB域的能量
    state->meanShortTerm = (int16_t)INNOTALK_SPL_RSHIFT_W32(tmp32, 4);

    // update short-term estimate of variance in energy level (Q8)
    tmp32 = INNOTALK_SPL_RSHIFT_W32(INNOTALK_SPL_MUL_16_16(dB, dB), 12);
    tmp32 += INNOTALK_SPL_MUL(state->varianceShortTerm, 15);
    state->varianceShortTerm = INNOTALK_SPL_RSHIFT_W32(tmp32, 4);

    // update short-term estimate of standard deviation in energy level (Q10)
    tmp32 = INNOTALK_SPL_MUL_16_16(state->meanShortTerm, state->meanShortTerm);
    tmp32 = INNOTALK_SPL_LSHIFT_W32(state->varianceShortTerm, 12) - tmp32;
    state->stdShortTerm = (int16_t)InnoTalkSpl_Sqrt(tmp32);

    // update long-term estimate of mean energy level (Q10)
    tmp32 = INNOTALK_SPL_MUL_16_16(state->meanLongTerm, state->counter) + (int32_t)dB;
    state->meanLongTerm = InnoTalkSpl_DivW32W16ResW16(tmp32, INNOTALK_SPL_ADD_SAT_W16(state->counter, 1));

    // update long-term estimate of variance in energy level (Q8)
    tmp32 = INNOTALK_SPL_RSHIFT_W32(INNOTALK_SPL_MUL_16_16(dB, dB), 12);
    tmp32 += INNOTALK_SPL_MUL(state->varianceLongTerm, state->counter);
    state->varianceLongTerm = InnoTalkSpl_DivW32W16(tmp32,   INNOTALK_SPL_ADD_SAT_W16(state->counter, 1));

    // update long-term estimate of standard deviation in energy level (Q10)
    tmp32 = INNOTALK_SPL_MUL_16_16(state->meanLongTerm, state->meanLongTerm);
    tmp32 = INNOTALK_SPL_LSHIFT_W32(state->varianceLongTerm, 12) - tmp32;
    state->stdLongTerm = (int16_t)InnoTalkSpl_Sqrt(tmp32);

    // update voice activity measure (Q10)
    tmp16 = INNOTALK_SPL_LSHIFT_W16(1, 12);
    tmp32 = INNOTALK_SPL_MUL_16_16(tmp16, (dB - state->meanLongTerm));
    tmp32 = InnoTalkSpl_DivW32W16(tmp32, state->stdLongTerm);
    tmpU16 = INNOTALK_SPL_LSHIFT_U16((uint16_t)15, 12);
    tmp32b = INNOTALK_SPL_MUL_16_U16(state->logRatio, tmpU16);
    tmp32 += INNOTALK_SPL_RSHIFT_W32(tmp32b, 10);

    state->logRatio = (int16_t)INNOTALK_SPL_RSHIFT_W32(tmp32, 6);

    // limit
    if (state->logRatio > 2048)
    {
        state->logRatio = 2048;
    }
    if (state->logRatio < -2048)
    {
        state->logRatio = -2048;
    }

    logratio = state->logRatio;


    upper_thr=agcConfig.AgcVadUppThr;

    lower_thr =agcConfig.AgcVadLowThr;

    if (logratio > upper_thr)  //JT：语音成分占主要，设置为最大的增益维持时间
    {
        // decay = -2^17 / DecayTime;  ->  -65
        //  decay = -65;
        decay = -65;
    } else if (logratio < lower_thr)  //无语音成分时，增益保持时间最小
    {
        decay = 0;
    } else
    {
        // decay = (int16_t)(((lower_thr - logratio)
        //       * (2^27/(DecayTime*(upper_thr-lower_thr)))) >> 10);
        // SUBSTITUTED: 2^27/(DecayTime*(upper_thr-lower_thr))  ->  65
        tmp32 = INNOTALK_SPL_MUL_16_16((lower_thr - logratio), 65);//JT:65为最大decay，根据LogRatio取65的加权
        decay = (int16_t)INNOTALK_SPL_RSHIFT_W32(tmp32, 10);
    }


    // adjust decay factor for long silence (detected as low standard deviation)
    // This is only done in the adaptive modes
    if (sttd->agcMode != kAgcModeFixedDigital)
    {
       if (sttd->vadNearend.stdLongTerm < 4000)
       {
           decay = 0;
       } else if (sttd->vadNearend.stdLongTerm < 8096)
       {
           // decay = (int16_t)(((stt->vadNearend.stdLongTerm - 4000) * decay) >> 12);
           tmp32 = INNOTALK_SPL_MUL_16_16((sttd->vadNearend.stdLongTerm - 4000), decay);
           decay = (int16_t)INNOTALK_SPL_RSHIFT_W32(tmp32, 12);
       }

       if (stt->lowLevelSignal != 0)
       {
           decay = 0;
       }
    }

    // Find max amplitude per sub frame
    // iterate over sub frames
    for (k = 0; k < 8; k++)//jt：对10个子帧的处理
    {
        // iterate over samples
        max_nrg = 0;
        for (n = 0; n < L; n++)//jt：每个子帧8点（8K）
        {
            nrg = INNOTALK_SPL_MUL_16_16(out[k * L + n], out[k * L + n]);
            if (nrg > max_nrg)
            {
                max_nrg = nrg;
            }
        }
        env[k] = max_nrg;//jt：获得每个子帧的最大值，存放在env中，作为包络线
    }

    // Calculate gain per sub frame
    gains[0] = sttd->gain;

    for (k = 0; k < 8; k++)//jt:依旧对子帧进行处理
    {
        //快慢包络的计算
        // Fast envelope follower  decay time = -131000 / -1000 = 131 (ms)，//jt:此处的131000对应的为2^17
        sttd->capacitorFast = AGC_SCALEDIFF32(-1000, sttd->capacitorFast, sttd->capacitorFast);
        if (env[k] > sttd->capacitorFast)
        {
            sttd->capacitorFast = env[k];
        }
        // Slow envelope follower
        if (env[k] > sttd->capacitorSlow)
        {
            // increase capacitorSlow
            sttd->capacitorSlow = AGC_SCALEDIFF32(500, (env[k] - sttd->capacitorSlow), sttd->capacitorSlow);
        } else
        {
            // decrease capacitorSlow
            sttd->capacitorSlow = AGC_SCALEDIFF32(decay, sttd->capacitorSlow, sttd->capacitorSlow);
        }

        // use maximum of both capacitors as current level，快慢包络取最大值作为当前判定信号
        if (sttd->capacitorFast > sttd->capacitorSlow)
        {
            cur_level = sttd->capacitorFast;
        } else
        {
            cur_level = sttd->capacitorSlow;
        }
        // Translate signal level into gain, using a piecewise linear approximation，将判定信号转换为增益，采用分段线性增益   find number of leading zeros
        zeros = InnoTalkSpl_NormU32((uint32_t)cur_level);
        if (cur_level == 0)
        {
            zeros = 31;
        }
        tmp32 = (INNOTALK_SPL_LSHIFT_W32(cur_level, zeros) & 0x7FFFFFFF);
        frac = (int16_t)INNOTALK_SPL_RSHIFT_W32(tmp32, 19); // Q12
        tmp32 = INNOTALK_SPL_MUL((sttd->gainTable[zeros-1] - sttd->gainTable[zeros]), frac);
        gains[k + 1] = sttd->gainTable[zeros] + INNOTALK_SPL_RSHIFT_W32(tmp32, 12);//JT:变系数的更新增益，gainTable[zeros],gainTable[zeros-1]

    }


    // Gate processing (lower gain during absence of speech)
    zeros = INNOTALK_SPL_LSHIFT_W16(zeros, 9) - INNOTALK_SPL_RSHIFT_W16(frac, 3);
    // find number of leading zeros
    zeros_fast = InnoTalkSpl_NormU32((uint32_t)sttd->capacitorFast);
    if (sttd->capacitorFast == 0)
    {
        zeros_fast = 31;
    }
    tmp32 = (INNOTALK_SPL_LSHIFT_W32(sttd->capacitorFast, zeros_fast) & 0x7FFFFFFF);
    zeros_fast = INNOTALK_SPL_LSHIFT_W16(zeros_fast, 9);
    zeros_fast -= (int16_t)INNOTALK_SPL_RSHIFT_W32(tmp32, 22);

    tmp32 =INNOTALK_SPL_MUL_16_16(logratio, agcConfig.GateVadSensitivity);//JT：放大logratio，使语音结束段时logratio的更新能体现。

    gate = 1000 + zeros_fast - zeros - sttd->vadNearend.stdShortTerm;

    if (gate < 0)
    {
        sttd->gatePrevious = 0;
    } else
    {
        tmp32 =INNOTALK_SPL_MUL_16_16(sttd->gatePrevious, 15)+INNOTALK_SPL_MUL_16_16(gate, 1);
        gate = (int16_t)INNOTALK_SPL_RSHIFT_W32( tmp32, 4);
        sttd->gatePrevious = gate;
    }

    if (gate > agcConfig.GateLowThr)
    {
        if (gate < agcConfig.GateUppThr)
        {
            gain_adj = INNOTALK_SPL_RSHIFT_W16(agcConfig.GateUppThr - gate, 5);
        } else
        {
            gain_adj = 0;
        }

        for (k = 0; k < 8; k++)
        {
            if ((gains[k + 1] - sttd->gainTable[0]) > 8388608)//JT:8388608代表100000000000000000000000
            {
                // To prevent wraparound
                tmp32 = INNOTALK_SPL_RSHIFT_W32((gains[k+1] - sttd->gainTable[0]), 8);
                // tmp32 = INNOTALK_SPL_MUL(tmp32, (100 + gain_adj));

        		tmp32 = INNOTALK_SPL_MUL(tmp32, ( agcConfig.SilenceGainFall + gain_adj));
            } else
            {
                tmp32 = INNOTALK_SPL_MUL((gains[k+1] - sttd->gainTable[0]), (agcConfig.SilenceGainFall + gain_adj));
                tmp32 = INNOTALK_SPL_RSHIFT_W32(tmp32, 8);
            }
            gains[k + 1] = sttd->gainTable[0] + tmp32;//JT:噪声信号时，gain倾向固定最小增益，全语音时候，倾向取上述计算的gain
        }
    }


    //Record=gate;//stt->vadNearend.stdShortTerm;//gate;
    // Limit gain to avoid overload distortion JT:防止过失真
    for (k = 0; k < 8; k++)
    {
        // To prevent wrap around
        zeros = 10;
        if (gains[k + 1] > 47453132)
        {
            zeros = 16 - InnoTalkSpl_NormW32(gains[k + 1]);
        }
        gain32 = INNOTALK_SPL_RSHIFT_W32(gains[k+1], zeros) + 1;
        gain32 = INNOTALK_SPL_MUL(gain32, gain32);
        // check for overflow
        while (AGC_MUL32(INNOTALK_SPL_RSHIFT_W32(env[k], 12) + 1, gain32)> INNOTALK_SPL_SHIFT_W32((int32_t)32767, 2 * (1 - zeros + 10)))
        {
            // multiply by 253/256 ==> -0.1 dB
            if (gains[k + 1] > 8388607)
            {
                // Prevent wrap around
                gains[k + 1] = INNOTALK_SPL_MUL(INNOTALK_SPL_RSHIFT_W32(gains[k+1], 8), 253);
            } else
            {
                gains[k + 1] = INNOTALK_SPL_RSHIFT_W32(INNOTALK_SPL_MUL(gains[k+1], 253), 8);
            }
            gain32 = INNOTALK_SPL_RSHIFT_W32(gains[k+1], zeros) + 1;
            gain32 = INNOTALK_SPL_MUL(gain32, gain32);
        }
    }
    // gain reductions should be done 1 ms earlier than gain increases//JT:增益上升要比增益下降迟缓1ms
    for (k = 1; k < 8; k++)
    {
       if (gains[k] > gains[k + 1])
       {
            gains[k] = gains[k + 1];
        }
    }
    // save start gain for next frame
    sttd->gain = gains[8];


    // Apply gain
    // handle first sub frame separately
    delta = INNOTALK_SPL_LSHIFT_W32(gains[1] - gains[0], (4 - L2));
    gain32 = INNOTALK_SPL_LSHIFT_W32(gains[0], 4);
    // iterate over samples
    for (n = 0; n < L; n++)
    {
        // For lower band
        tmp32 = INNOTALK_SPL_MUL((int32_t)out[n], INNOTALK_SPL_RSHIFT_W32(gain32 + 127, 7));
        out_tmp = INNOTALK_SPL_RSHIFT_W32(tmp32 , 16);
        if (out_tmp > 4095)
        {
            out[n] = (int16_t)32767;
            // RecOutL[n]=Record;
        } else if (out_tmp < -4096)
        {
            out[n] = (int16_t)-32768;
            //RecOutL[n]=Record;
        } else
        {
           tmp32 = INNOTALK_SPL_MUL((int32_t)out[n], INNOTALK_SPL_RSHIFT_W32(gain32, 4));
           out[n] = (int16_t)INNOTALK_SPL_RSHIFT_W32(tmp32 , 16);
           // RecOutL[n]=Record;
        }
        gain32 += delta;
    }
    // iterate over subframes//JT：处理之后的9子帧
    for (k = 1; k < 8; k++)
    {
        delta = INNOTALK_SPL_LSHIFT_W32(gains[k+1] - gains[k], (4 - L2));
        gain32 = INNOTALK_SPL_LSHIFT_W32(gains[k], 4);
        // iterate over samples
        for (n = 0; n < L; n++)
        {
           // For lower band
           tmp32 = INNOTALK_SPL_MUL((int32_t)out[k * L + n],INNOTALK_SPL_RSHIFT_W32(gain32, 4));
           out[k * L + n] = (int16_t)INNOTALK_SPL_RSHIFT_W32(tmp32 , 16);
           // RecOutL[k * L + n]=Record;
           gain32 += delta;
        }
    }

        //////////////WQY end

    return 0;
}


