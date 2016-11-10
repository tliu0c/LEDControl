#pragma once
#include "GetCoreTempInfo.h"

#define RETRY_GETCPUINFO_INTERVAL 50
#define UPDATE_INTERVAL_CPU 1000
#define CPU_TEMP_ORANGE 55
#define CPU_TEMP_RED 75

void GetCPUInfoInit();