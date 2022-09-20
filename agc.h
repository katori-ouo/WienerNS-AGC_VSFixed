/*
 * agc.h
 *
 *  Created on: 2021年4月26日
 *      Author: w
 */
#include "audio_config.h"
#include "signal_processing_library.h"

#ifndef SRC_LIB_CODEC_AEC_CORE_TL4_AGC_H_
#define SRC_LIB_CODEC_AEC_CORE_TL4_AGC_H_

#define RXX_BUFFER_LEN  10
#define AGC_DEFAULT_TARGET_LEVEL 3
#define AGC_DEFAULT_COMP_GAIN 9
#define DIFF_REF_TO_ANALOG 5
#define ANALOG_TARGET_LEVEL 11
#define ANALOG_TARGET_LEVEL_2 5 // ANALOG_TARGET_LEVEL / 2
#define AGC_DEFAULT_TARGET_LEVEL 3
#define AGC_DEFAULT_COMP_GAIN 9
#define DIGITAL_REF_AT_0_COMP_GAIN 4
#define OFFSET_ENV_TO_RMS 9

static const int16_t kInitCheck = 42;

static const int16_t kAvgDecayTime = 250; // frames; < 3000


enum
{
    kAgcModeUnchanged,
    kAgcModeAdaptiveAnalog,
    kAgcModeAdaptiveDigital,
    kAgcModeFixedDigital
};
enum
{
    kAgcFalse = 0,
    kAgcTrue
};
enum { kGenFuncTableSize = 128 };


// the 32 most significant bits of A(19) * B(26) >> 13
#define AGC_MUL32(A, B)             (((B)>>13)*(A) + ( ((0x00001FFF & (B))*(A)) >> 13 ))
// C + the 32 most significant bits of A * B
#define AGC_SCALEDIFF32(A, B, C)    ((C) + ((B)>>16)*(A) + ( ((0x0000FFFF & (B))*(A)) >> 16 ))
// Multiply a 32-bit value with a 16-bit value and accumulate to another input:
#define MUL_ACCUM_1(a, b, c)              INNOTALK_SPL_SCALEDIFF32(a, b, c)
#define MUL_ACCUM_2(a, b, c)              INNOTALK_SPL_SCALEDIFF32(a, b, c)



typedef struct
{
    int16_t targetLevelDbfs;   // default 3 (-3 dBOv)
    int16_t compressionGaindB; // default 9 dB
    uint8_t limiterEnable;     // default kAgcTrue (on)
    int16_t SilenceGainFall;//静默段增益下降程度，推荐100
    int16_t AgcVadLowThr;      //AGC中Vad的语音判断低门限，推荐0
    int16_t AgcVadUppThr;      //AGC中Vad的语音判断高门限，推荐1024
    int16_t  GateLowThr;       //JT:Gate判定低门限，推荐值为0
    int16_t GateUppThr;       //JT:Gate判定高门限，推荐值为2500
    int16_t GateVadSensitivity; //JT:Gate计算对语音活性的敏感度，推荐值为6
} InnoTalkAgc_config_t;         //total = 17 bytes


typedef struct
{
    int32_t downState[8];
    int16_t HPstate;
    int16_t counter;
    int16_t logRatio; // log( P(active) / P(inactive) ) (Q10)
    int16_t meanLongTerm; // Q10
    int32_t varianceLongTerm; // Q8
    int16_t stdLongTerm; // Q10
    int16_t meanShortTerm; // Q10
    int32_t varianceShortTerm; // Q8
    int16_t stdShortTerm; // Q10
} AgcVad_t;                     // total = 54 bytes

typedef struct
{
    int32_t capacitorSlow;
    int32_t capacitorFast;
    int32_t gain;
    int32_t gainTable[32];  //128 bytes
    int16_t gatePrevious;
    int16_t agcMode;
    AgcVad_t      vadNearend;
} DigitalAgc_t;                // total = 54 + 128 + 16 = 198 bytes

typedef struct
{
    // Configurable parameters/variables
    uint32_t            fs;                 // Sampling frequency
    int16_t             compressionGaindB;  // Fixed gain level in dB
    int16_t             targetLevelDbfs;    // Target level in -dBfs of envelope (default -3)
    int16_t             agcMode;            // Hard coded mode (adaptAna/adaptDig/fixedDig)
    uint8_t             limiterEnable;      // Enabling limiter (on/off (default off))

    // General variables
    int16_t             initFlag;
    int16_t             lastError;

    // Target level parameters
    // Based on the above: analogTargetLevel = round((32767*10^(-22/20))^2*16/2^7)
    int32_t             analogTargetLevel;  // = RXX_BUFFER_LEN * 846805;       -22 dBfs
    int32_t             startUpperLimit;    // = RXX_BUFFER_LEN * 1066064;      -21 dBfs
    int32_t             startLowerLimit;    // = RXX_BUFFER_LEN * 672641;       -23 dBfs
    int32_t             upperPrimaryLimit;  // = RXX_BUFFER_LEN * 1342095;      -20 dBfs
    int32_t             lowerPrimaryLimit;  // = RXX_BUFFER_LEN * 534298;       -24 dBfs
    int32_t             upperSecondaryLimit;// = RXX_BUFFER_LEN * 2677832;      -17 dBfs
    int32_t             lowerSecondaryLimit;// = RXX_BUFFER_LEN * 267783;       -27 dBfs
    uint16_t            targetIdx;          // Table index for corresponding target level

    int16_t             analogTarget;       // Digital reference level in ENV scale

    // Analog AGC specific variables
    int32_t             filterState[8];     // For downsampling wb to nb
    int32_t             upperLimit;         // Upper limit for mic energy
    int32_t             lowerLimit;         // Lower limit for mic energy

    // Microphone level variables
    int32_t             micRef;             // Remember ref. mic level for virtual mic
    int32_t             micGainIdx;         // Gain index of mic level to increase slowly
    int32_t             micVol;             // Remember volume between frames
    int32_t             maxLevel;           // Max possible vol level, incl dig gain
    int32_t             maxAnalog;          // Maximum possible analog volume level
    int32_t             maxInit;            // Initial value of "max"
    int32_t             minLevel;           // Minimum possible volume level
    int32_t             minOutput;          // Minimum output volume level
    int32_t             zeroCtrlMax;        // Remember max gain => don't amp low input

    int16_t             scale;              // Scale factor for internal volume levels

    // Structs for VAD and digital_agc
    AgcVad_t            vadMic;
    DigitalAgc_t        digitalAgc;

    int16_t             lowLevelSignal;
} Agc_t;                                    // total = 380 bytes
#ifdef __cplusplus
extern "C" {
#endif
int InnoTalkAgc_Sat(short * x, short * y, short framesz, short a);
// void InnoTalkSpl_MemSetW32(int32_t *ptr, int32_t set_value, int length);
// int32_t InnoTalkSpl_DivW32W16(int32_t num, int16_t den);
// int16_t InnoTalkSpl_DivW32W16ResW16(int32_t num, int16_t den);
//int32_t InnoTalkSpl_SqrtLocal(int32_t in);
//int32_t InnoTalkSpl_Sqrt(int32_t value);
int32_t InnoTalkAgc_CalculateGainTable(int32_t *, int16_t,  int16_t , uint8_t , int16_t);
int InnoTalkAgc_set_config(void *agcInst, InnoTalkAgc_config_t agcConfig);
void InnoTalkAgc_InitVad(AgcVad_t *state);
int InnoTalkAgc_Create(void **agcInst);
//int32_t InnoTalkAgc_InitDigital(DigitalAgc_t *stt, int16_t agcMode);
int InnoTalkAgc_Init(void *agcInst, int32_t minLevel, int32_t maxLevel, uint32_t fs);
void InnoTalkAgc_UpdateAgcThresholds(Agc_t *stt);
int InnoTalkAgc_Process(void * , const int16_t *, int16_t *, InnoTalkAgc_config_t );

#ifdef __cplusplus
}
#endif
#endif /* SRC_LIB_CODEC_AEC_CORE_TL4_AGC_H_ */
