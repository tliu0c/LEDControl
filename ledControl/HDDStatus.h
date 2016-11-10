#pragma once

CONST PWSTR COUNTER_PATH_DISK0 = L"\\PhysicalDisk(0 E:)\\Disk Bytes/sec";
CONST PWSTR COUNTER_PATH_DISK1 = L"\\PhysicalDisk(1 D:)\\Disk Bytes/sec";
CONST PWSTR COUNTER_PATH_DISK2 = L"\\PhysicalDisk(2 W:)\\Disk Bytes/sec";
CONST PWSTR COUNTER_PATH_DISK3 = L"\\PhysicalDisk(3 C:)\\Disk Bytes/sec";

#define UPDATE_INTERVAL_HDD 33
#define BYTES_RATE_RED 51200000
#define BYTES_RATE_ORANGE 1280000

int GetHDDInfoInit();