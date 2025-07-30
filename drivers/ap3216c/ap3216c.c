#define DT_DRV_COMPAT liteon_ap3216c

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/logging/log.h>

#include "ap3216c.h"


LOG_MODULE_REGISTER(ap3216c, CONFIG_SENSOR_LOG_LEVEL);


static int ap3216c_sample_fetch(const struct device *dev,
			     enum sensor_channel chan)
{
	struct ap3216c_data *data = dev->data;
	const struct ap3216c_config *cfg = dev->config;

	__ASSERT_NO_MSG(chan == SENSOR_CHAN_ALL || chan == SENSOR_CHAN_AMBIENT_TEMP);

	uint8_t buf[2];

    /* ALS 16-bit */
    i2c_burst_read_dt(&cfg->i2c, AP3216C_REG_ALS_DATA, buf, 2);
    data->als = sys_get_le16(buf);

    /* PS 10-bit (low 4 bits + high 6 bits) */
    i2c_burst_read_dt(&cfg->i2c, AP3216C_REG_PS_DATA, buf, 2);
    data->ps = ((buf[1] & 0x3F) << 4) | (buf[0] & 0x0F);

    /* IR 10-bit */
    i2c_burst_read_dt(&cfg->i2c, AP3216C_REG_IR_DATA, buf, 2);
    data->ir = ((buf[1] & 0x03) << 8) | buf[0];

	return 0;
}

static int ap3216c_channel_get(const struct device *dev,
			    enum sensor_channel chan,
			    struct sensor_value *val)
{
	struct ap3216c_data *drv_data = dev->data;

	__ASSERT_NO_MSG(chan == SENSOR_CHAN_AMBIENT_TEMP ||
			chan == SENSOR_CHAN_HUMIDITY);

	if (chan == SENSOR_CHAN_LIGHT) {
	
		val->val1 = drv_data->als;
        val->val2 = 0;

	} else if (chan == SENSOR_CHAN_PROX) {
		
		val->val1 = drv_data->ps;
        val->val2 = 0;

	} else if (chan == SENSOR_CHAN_IR) {
        
        val->val1 = drv_data->ir;
        val->val2 = 0;

    } else {
		return -ENOTSUP;
	}

	return 0;
}

static DEVICE_API(sensor, ap3216c_driver_api) = {
	.sample_fetch = ap3216c_sample_fetch,
	.channel_get = ap3216c_channel_get,
};

static int ap3216c_init(const struct device *dev)
{
    int ret;
	const struct ap3216c_config *cfg = dev->config;
    uint8_t mode = 0x03;                /* ALS+PS+IR 模式 */

	if (!device_is_ready(cfg->i2c.bus)) {
		LOG_ERR("Bus device is not ready");
		return -ENODEV;
	}

    /* 软复位 */
    ret = i2c_reg_write_byte_dt(&cfg->i2c, AP3216C_REG_SYS_CFG, 0x04);
    if (ret) return ret;
    k_msleep(10);

    /* 进入工作模式 */
    ret = i2c_reg_write_byte_dt(&cfg->i2c, AP3216C_REG_SYS_CFG, mode);
    if (ret) return ret;

	return 0;
}

#define AP3216C_DEFINE(inst)								\
	static struct ap3216c_data ap3216c_data_##inst;					\
											\
	static const struct ap3216c_config ap3216c_config_##inst = {				\
		.i2c = I2C_DT_SPEC_INST_GET(inst),					\
	};										\
											\
	SENSOR_DEVICE_DT_INST_DEFINE(inst, ap3216c_init, NULL,				\
			      &ap3216c_data_##inst, &ap3216c_config_##inst, POST_KERNEL,	\
			      CONFIG_SENSOR_INIT_PRIORITY, &ap3216c_driver_api);		\

DT_INST_FOREACH_STATUS_OKAY(AP3216C_DEFINE)