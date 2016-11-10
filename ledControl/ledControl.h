#pragma once
enum tag_sensor_type
{
	ST_ALL,
	ST_LED,
	ST_LED_INT_HDD,
	ST_LED_EXT_HDD0,
	ST_LED_EXT_HDD1,
	ST_LED_BRIGHTNESS,
	ST_FAN,
	ST_THERMAL,
	ST_VOLTAGE,
	ST_FWP,
	ST_INTRD,
	ST_TEMP
};

enum tag_sensor_location
{
	SL_TYPE_ALL,
	SL_LED_STATUS,
	SL_LED_HDD_0,
	SL_LED_HDD_1,
	SL_LED_HDD_2,
	SL_LED_HDD_3,
	SL_LED_BRIGHTNESS,
	SL_FAN_TACH_UPPER,
	SL_FAN_TACH_BOTTOM,
	SL_THERMAL_CPU,
	SL_THERMAL_SYSTEM,
	SL_THERMAL_BACKPLANE,
	SL_VOLTAGE_VCCP,
	SL_VOLTAGE_18V,
	SL_VOLTAGE_33V,
	SL_VOLTAGE_5V,
	SL_VOLTAGE_15V_CORE,
	SL_VOLTAGE_AUX,
	SL_FWP_FLASH,
	SL_VOLTAGE_3SB,
	SL_VOLTAGE_3P0_BAT,
	SL_LED_POWER_STATUS,
	SL_LED_BACKUP_STATUS,
	SL_FAN_AUTO_CONFIG,
	SL_FAN_MANUAL_CONFIG,
	SL_INTRD,
	SL_LED_RAID_STATUS,
	SL_TEMP_CPU,
	SL_TEMP_PCH,
	SL_TEMP_DIMM0,
	SL_TEMP_DIMM1
};

enum LEDBrightnessState
{
	LEDBR_0 = 0,
	LEDBR_1 = 1,
	LEDBR_2 = 2,
	LEDBR_3 = 3,
	LEDBR_4 = 4,
	LEDBR_5 = 5,
	LEDBR_6 = 6,
	LEDBR_7 = 7,
	LEDBR_8 = 8,
	LEDBR_9 = 9,
	LEDBR_ERROR = -1
};

enum LEDColorState
{
	LED_OFF = 0x8000,
	LED_GREEN = 0x8001,
	LED_RED = 0x8002,
	LED_ORANGE = 0x8003,
	LED_BLINK_GREEN = 0x8011,	//Blinking can only be used on status LEDs
	LED_BLINK_RED = 0x8012,
	LED_BLINK_ORANGE = 0x8013
};

typedef struct
{
	QWORD ObjectNext;
	QWORD ObjectPrev;
	tag_sensor_type ss_type;
	tag_sensor_location ss_location;
	QWORD ss_status;
}tag_sensor_info;

void ControlLed(tag_sensor_location sl, tag_sensor_type st, LEDColorState color);