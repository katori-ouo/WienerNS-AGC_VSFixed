#ifndef INNOTALK_AUDIO_CONFIG_H_
#define INNOTALK_AUDIO_CONFIG_H_

#define FRAME128                0
#if FRAME128
#define ANAL_BLOCKL_MAX         256 /* Max analysis block length */
#define FRAME_LEN               128 /* Max frame length */
#define HALF_ANAL_BLOCKL        129 /* Half max analysis block length + 1 */
#else
#define ANAL_BLOCKL_MAX         128 /* Max analysis block length */
#define FRAME_LEN               64  /* Max frame length */
#define HALF_ANAL_BLOCKL        65  /* Half max analysis block length + 1 */
#endif
#define LEN1024_PACKET          1024
#define LEN512_PACKET           512
#define LEN256_PACKET           256
#define LEN128_PACKET           128
#define Q15MOD                  32768
#define Q31MOD                  2147483648
#define Q30MOD                  1073741824

// Define C99 equivalent types, since MSVC doesn't provide stdint.h.
typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef __int64             int64_t;
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned __int64    uint64_t;

#endif /* INNOTALK_AUDIO_CONFIG_H_ */