#ifndef ZEPHYER_DRIVERS_AP3216C_H_
#define ZEPHYER_DRIVERS_AP3216C_H_

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>

struct ap3216c_config {
	struct i2c_dt_spec i2c;
};

struct ap3216c_data {
	uint16_t als;      // 环境光亮度传感器数据
	uint16_t ps;       // 接近传感器数据
    uint16_t ir;       // 红外传感器数据
};

#endif