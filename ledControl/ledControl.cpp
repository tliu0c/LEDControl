// ledControl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ledControl.h"
#include "CPUStatus.h"
#include "HDDStatus.h"

typedef HANDLE(*pfn_wha_open)();
typedef QWORD(*pfn_wha_close)(HANDLE handle);
typedef QWORD(*pfn_wha_SensorObjectCreate)(HANDLE handle, tag_sensor_type ss_type, tag_sensor_location ss_location, tag_sensor_info &ss_info);
typedef QWORD(*pfn_wha_SensorObjectDestroy)(HANDLE handle, tag_sensor_info &ss_info);
typedef QWORD(*pfn_wha_GetSensorInformation)(HANDLE handle, tag_sensor_info &ss_info);
typedef QWORD(*pfn_wha_SetSensorStatus)(HANDLE handle, tag_sensor_info &ss_info);

pfn_wha_open wha_open = 0;
pfn_wha_SensorObjectCreate wha_SensorObjectCreate = 0;
pfn_wha_SensorObjectDestroy wha_SensorObjectDestroy = 0;
pfn_wha_GetSensorInformation wha_GetSensorInformation = 0;
pfn_wha_SetSensorStatus wha_SetSensorStatus = 0;

void GetDLLAPI(HMODULE WNASDLL_BASE) {
    wha_open = (pfn_wha_open)GetProcAddress(WNASDLL_BASE, "wha_open");
    wha_SensorObjectCreate = (pfn_wha_SensorObjectCreate)GetProcAddress(WNASDLL_BASE, "wha_SensorObjectCreate");
    wha_SensorObjectDestroy = (pfn_wha_SensorObjectDestroy)GetProcAddress(WNASDLL_BASE, "wha_SensorObjectDestroy");
    wha_GetSensorInformation = (pfn_wha_GetSensorInformation)GetProcAddress(WNASDLL_BASE, "wha_GetSensorInformation");
    wha_SetSensorStatus = (pfn_wha_SetSensorStatus)GetProcAddress(WNASDLL_BASE, "wha_SetSensorStatus");
}

HANDLE hLed;
void ControlLed(tag_sensor_location sl, tag_sensor_type st, LEDColorState color) {
    tag_sensor_info ss_info;
    ss_info.ss_location = sl;
    ss_info.ss_type = st;
    ss_info.ss_status = color;
    wha_SetSensorStatus(hLed, ss_info);
}

int main() {
    //Close all other instances
    DWORD pID = GetProcessId(GetCurrentProcess());
    char cmd[256] = { 0 };
    sprintf(cmd, "taskkill /im ledControl.exe /FI \"pid NE %d\" /f /t", pID);
    system(cmd);
    //Hide the console window
    FreeConsole();
    //Load the LED driver interface DLL to control LED
    HMODULE WNASDLL_BASE = LoadLibraryA("WNASDLLATL.dll");
    if (!WNASDLL_BASE)
        return MessageBox(NULL, L"Could not locate WNASDLLATL.dll", L"Fatal error", MB_OK | MB_ICONEXCLAMATION);
    GetDLLAPI(WNASDLL_BASE);
    hLed = wha_open();
    ControlLed(SL_LED_POWER_STATUS, ST_LED, LED_GREEN);
    if (!hLed)
        return MessageBox(NULL, L"Could not open wha handle", L"Fatal error", MB_OK | MB_ICONEXCLAMATION);
    //Start HDD and CPU monitoring routines
    GetCPUInfoInit();
    GetHDDInfoInit();
    return 0;
}
