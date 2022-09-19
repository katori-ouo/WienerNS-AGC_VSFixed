/*
 * nsx.h
 *
 *  Created on: 2021��5��4��
 *      Author: w
 */

#ifndef NSX_H_
#define NSX_H_

#include "signal_processing_library.h"
#include "typedefs.h"

#define CEVAOPT 1
#define CUT     0
#define FSmooth 0

#if CEVAOPT
#include "CEVA_common.h"
#include "ceva_typedef.h"
#endif

#define SMOOTH_APY16	  22937  // ƽ�������׵�ƽ������0.7, Q15
#define SMOOTH_APY16_S  9831   // Q15MOD - SMOOTH_APY16, Q15
#define SMOOTH_R16		  32702	 // ����ƽ����������Сֵ�ľ��鳣��0.998, Q15
#define SMOOTH_BETA16	  31457	 // ����ƽ����������Сֵ�ľ��鳣��0.96, Q15
#define AP16			      6554	 // �������ʸ���ϵ��0.2, Q15
#define AP16_S          26214  // Q15MOD - AP16, Q15
#define G16				      3277   // �������ʱ任ϵ��0.1, Q15
#define AF16			      32112  // ������������ȵľ���ϵ��0.98, Q15
#define AF16_S          656    // Q15MOD - AF16, Q15
#define B_LIM16         16384  // ���������������Ӽ����е���ֵ0.5, Q15
#define FILTER_SMOOTH   29491  // �˲�������ϵ��0.9, Q15
#define FILTER_SMOOTH_S 3277   // Q15MOD - FILTER_SMOOTH, Q15

#if FSmooth
#define ZETA_THR        13107  // ȥ�������������������ֵ0.4, Q15
#define PSI             20     // ȥ������������ƽ���̶�, Q0
#endif

typedef struct NsxInst_t_ {

  int                     initFlag;

  uint32_t                fs; //FIXME: ����Ƶ��û���õ�
  int                     blockLen; // �鳤��
  int16_t                 analysisBuffer[ANAL_BLOCKL_MAX];
  int16_t                 synthesisBuffer[ANAL_BLOCKL_MAX];

  int                     stages;
  int                     blockIndex;  // Frame index counter.
  int                     zeroInputSignal;  // Zero input signal flag.
  int                     scaleEnergyIn;
  uint64_t                energyIn;
  int                     norm;
  int                     normPrev;
  int16_t                 fftdata[ANAL_BLOCKL_MAX * 2];

  // ά���˲��й�
  int32_t         smooth32[HALF_ANAL_BLOCKL];
  int32_t         overdrive16;                         /* Q15 */
  int32_t         denoiseBound16;                      /* Q15 */
  int32_t         noisePrev32[HALF_ANAL_BLOCKL];       // ǰһ֡������������
  int32_t         pmagnPrev32[HALF_ANAL_BLOCKL];       // ǰһ֡��ƽ��������
  int32_t         probPrev32[HALF_ANAL_BLOCKL];        // ǰһ֡����������
  int32_t         signalPrev32[HALF_ANAL_BLOCKL];      // ǰһ֡�ɾ������ķ����׹���ֵ
  int32_t         minMagn32[HALF_ANAL_BLOCKL];         // ƽ�������׵���Сֵ

} NsxInst_t;
typedef struct NsxHandleT NsxHandle;

#if FSmooth
typedef struct {
    int32_t  size;
    int64_t  sum;
    int32_t* queue;
    int32_t  front;
    int32_t  rear;
} MovingAverage;
#endif


#ifdef __cplusplus
extern "C"
{
#endif
int InnoTalkNsx_Create(void** nsxInst);
int32_t InnoTalkNsx_InitCore(void* inst, uint32_t fs);
int InnoTalkNsx_ProcessCore(void* inst, short* inFrameLow, short* outFrameLow);
#ifdef __cplusplus
}
#endif
#endif /* NSX_H_ */
