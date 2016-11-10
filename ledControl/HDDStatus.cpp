#include "stdafx.h"
#include "HDDStatus.h"
#include <pdh.h>
#include "ledControl.h"

#pragma comment(lib, "pdh.lib")

void UpdateHDDLED(tag_sensor_location sl, double bytesPerSec) {
    LEDColorState color = LED_GREEN;
    if (bytesPerSec > BYTES_RATE_RED)
        color = LED_RED;
    else if (bytesPerSec > BYTES_RATE_ORANGE)
        color = LED_ORANGE;
    ControlLed(sl, ST_LED_INT_HDD, color);
}

int GetHDDInfoInit(){
    HQUERY hQuery = NULL;
    PDH_STATUS pdhStatus;
    HCOUNTER hCounterDisk0, hCounterDisk1, hCounterDisk2, hCounterDisk3;
    // Open a query object.
    pdhStatus = PdhOpenQuery(NULL, 0, &hQuery);
    if (pdhStatus != ERROR_SUCCESS)
        return MessageBox(NULL, L"PdhOpenQuery failed.", L"Error", MB_OK | MB_ICONEXCLAMATION);
    // Get HDD activity data. My machine's second drive bay is empty.
    PDH_STATUS pdhStatus0 = PdhAddCounter(hQuery, COUNTER_PATH_DISK0, 0, &hCounterDisk0);
    PDH_STATUS pdhStatus1 = PdhAddCounter(hQuery, COUNTER_PATH_DISK1, 0, &hCounterDisk1);
    PDH_STATUS pdhStatus2 = PdhAddCounter(hQuery, COUNTER_PATH_DISK2, 0, &hCounterDisk2);
    PDH_STATUS pdhStatus3 = PdhAddCounter(hQuery, COUNTER_PATH_DISK3, 0, &hCounterDisk3);
    if (pdhStatus0 != ERROR_SUCCESS || pdhStatus1 != ERROR_SUCCESS || pdhStatus2 != ERROR_SUCCESS || pdhStatus3 != ERROR_SUCCESS)
        return MessageBox(NULL, L"PdhAddCounter failed.", L"Error", MB_OK|MB_ICONEXCLAMATION);
    pdhStatus = PdhCollectQueryData(hQuery);
    // Infinite loop for HDD activity data query
    for (;;){
        ULONG CounterType;
        PDH_FMT_COUNTERVALUE DisplayValue;

        pdhStatus = PdhCollectQueryData(hQuery);
        pdhStatus = PdhGetFormattedCounterValue(hCounterDisk3, PDH_FMT_DOUBLE, &CounterType, &DisplayValue);
        if (pdhStatus == ERROR_SUCCESS)
            UpdateHDDLED(SL_LED_HDD_3, DisplayValue.doubleValue);
        pdhStatus = PdhGetFormattedCounterValue(hCounterDisk2, PDH_FMT_DOUBLE, &CounterType, &DisplayValue);
        if (pdhStatus == ERROR_SUCCESS)
            UpdateHDDLED(SL_LED_HDD_2, DisplayValue.doubleValue);
        pdhStatus = PdhGetFormattedCounterValue(hCounterDisk1, PDH_FMT_DOUBLE, &CounterType, &DisplayValue);
        if (pdhStatus == ERROR_SUCCESS)
            UpdateHDDLED(SL_LED_HDD_1, DisplayValue.doubleValue);
        pdhStatus = PdhGetFormattedCounterValue(hCounterDisk0, PDH_FMT_DOUBLE, &CounterType, &DisplayValue);
        if (pdhStatus == ERROR_SUCCESS)
            UpdateHDDLED(SL_LED_HDD_0, DisplayValue.doubleValue);
        Sleep(UPDATE_INTERVAL_HDD);
    }
}

