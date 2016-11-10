#include "stdafx.h"
#include <math.h>
#include "CPUStatus.h"
#include "ledControl.h"

typedef bool (WINAPI *pfnGetCoreTempInfo)(CORE_TEMP_SHARED_DATA *pData);
pfnGetCoreTempInfo GetCoreTempInfo;
    
//Stock example from CoreTemp SDK. For debugging
void PrintCPUCoreInfo(CORE_TEMP_SHARED_DATA *CoreTempData) {
    //Print caption.
    printf("Core Temp shared memory reader:\n\n");
    ULONG index = 0;
    char tempType = CoreTempData->ucFahrenheit ? 'F' : 'C';
    //Now print the output.
    printf("CPU Name: %s\n", CoreTempData->sCPUName);
    printf("CPU Speed: %.2fMHz (%.2f x %.2f)\n", CoreTempData->fCPUSpeed, CoreTempData->fFSBSpeed, CoreTempData->fMultipier);
    printf("CPU VID: %.4fv\n", CoreTempData->fVID);
    printf("Physical CPUs: %d\n", CoreTempData->uiCPUCnt);
    printf("Cores per CPU: %d\n", CoreTempData->uiCoreCnt);
    for (UINT i = 0; i < CoreTempData->uiCPUCnt; i++)
    {
        printf("CPU #%d\n", i);
        printf("Tj.Max: %d%c\n", CoreTempData->uiTjMax[i], tempType);
        for (UINT g = 0; g < CoreTempData->uiCoreCnt; g++)
        {
            index = g + (i * CoreTempData->uiCoreCnt);
            if (CoreTempData->ucDeltaToTjMax)
            {
                printf("Core #%d: %.2f%c to TjMax, %d%% Load\n",
                    index, CoreTempData->fTemp[index], tempType, CoreTempData->uiLoad[index]);
            }
            else
            {
                printf("Core #%d: %.2f%c, %d%% Load\n",
                    index, CoreTempData->fTemp[index], tempType, CoreTempData->uiLoad[index]);
            }
        }
    }
}

void UpdateStatusLED() {
    CORE_TEMP_SHARED_DATA CoreTempData;
    LEDColorState color;
    float CPUtemp;
    UINT CPUload;
    UINT sleepTime;
    UINT flashCount;
    while (1)
    {	
        //First refresh CPU info
        if (!GetCoreTempInfo(&CoreTempData)) {
            ControlLed(SL_LED_STATUS, ST_LED, LED_OFF);
            Sleep(RETRY_GETCPUINFO_INTERVAL);
            continue;
        }
        color = LED_GREEN;
        //My I3-2120 has 2 cores
        CPUtemp = ((CoreTempData.fTemp[0]) + (CoreTempData.fTemp[1])) / 2;
        CPUload = ((CoreTempData.uiLoad[0]) + (CoreTempData.uiLoad[1])) / 2;
        //Set status LED color based on CPU temp
        if (CPUtemp > CPU_TEMP_RED)
            color = LED_RED;
        else if (CPUtemp > CPU_TEMP_ORANGE)
            color = LED_ORANGE;
        ControlLed(SL_LED_STATUS, ST_LED, color);

        //Flash status LED based on CPU load, the higher the load, the higher flashing frequency
        //No flash if load < 5
        if (CPUload < 5) {
            Sleep(UPDATE_INTERVAL_CPU);
            continue;
        }
        //flash the current color (%load ^ 1.1 / 10) times per second
        flashCount = (CPUload > 8) ? (pow(CPUload, 1.1) / 10) : 1;
        sleepTime = UPDATE_INTERVAL_CPU / (flashCount * 2);
        for (int i = 0; i < flashCount; i++) {
            ControlLed(SL_LED_STATUS, ST_LED, LED_OFF);
            Sleep(sleepTime);
            ControlLed(SL_LED_STATUS, ST_LED, color);
            Sleep(sleepTime);
        }
    }
}

void GetCPUInfoInit()
{
    HMODULE hCT;
    CORE_TEMP_SHARED_DATA CoreTempData;
    memset(&CoreTempData, 0, sizeof(CORE_TEMP_SHARED_DATA));

    hCT = LoadLibrary(L"CoreTemp.dll");
    if (!hCT)
        MessageBox(NULL, L"Could not locate CoreTemp.dll", L"Error", MB_OK | MB_ICONEXCLAMATION);
    GetCoreTempInfo = (pfnGetCoreTempInfo)GetProcAddress(hCT, "fnGetCoreTempInfoAlt");
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)UpdateStatusLED, NULL, NULL, NULL);
}