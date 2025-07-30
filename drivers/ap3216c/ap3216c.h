#ifndef ZEPHYER_DRIVERS_AP3216C_H_
#define ZEPHYER_DRIVERS_AP3216C_H_

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>

#define AP3216C_I2C_ADDR        0x1E
#define AP3216C_REG_SYS_CFG     0x00
#define AP3216C_REG_ALS_DATA    0x0C
#define AP3216C_REG_PS_DATA     0x0E
#define AP3216C_REG_IR_DATA     0x0A

struct ap3216c_config {
	struct i2c_dt_spec i2c;
};

struct ap3216c_data {
	uint16_t als;      // 环境光亮度传感器数据
	uint16_t ps;       // 接近传感器数据
    uint16_t ir;       // 红外传感器数据
};

#endif