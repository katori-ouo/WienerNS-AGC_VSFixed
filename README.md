# WienerNS-AGC_VSFixed
对音频进行维纳降噪和AGC自动增益控制的程序。VS版本定点代码，使用CEVA库

## 1. main.c

- 定义程序入口。读取指定的音频文件(.pcm)，进行降噪和AGC后再写入新的文件(.pcm)。
- 设定音频采样率16kHz，帧长128点。

## 2. audio_config.h

- 音频处理的一些参数，如采样率和帧长等。
- 定义数据格式。

## 3. nsx.h

- 维纳降噪算法的头文件。
- 定义了降噪算法中的各个参数值和结构体。

## 4. nsx.c

- 维纳降噪算法的源文件，实现各种功能函数。
- FFT和移位等函数均采用CEVA库。

## 5. agc.h

- AGC算法的头文件。
- 定义了AGC算法中的各个参数和结构体。

## 6. agc.c

- AGC算法的源文件，实现各种功能函数。
- 引用的函数来自signal_processing_library。

## 7. cevalib

- CEVA的信号处理库，包括FFT，数组移位，数组相乘等操作。
- 使用时需要引用"CEVA_common.h"和"CEVA_typedef.h"

## 8 signal_processing_library

- 定义了一些常用的信号处理函数。
- 函数的声明和部分内联函数在"signal_processing_library.h"和"spl_inl.h"。
- 其他函数的实现在"operations.c"中。
