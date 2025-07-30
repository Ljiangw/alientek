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
	struct ap3216c_data *drv_data = dev->data;
	const struct ap3216c_config *cfg = dev->config;

	__ASSERT_NO_MSG(chan == SENSOR_CHAN_ALL || chan == SENSOR_CHAN_AMBIENT_TEMP);

	// drv_data->t_sample = get_temp(&cfg->i2c);
	// LOG_INF("temp: %u", drv_data->t_sample);
	// drv_data->rh_sample = get_humi(&cfg->i2c);
	// LOG_INF("rh: %u", drv_data->rh_sample);

	return 0;
}

static int ap3216c_channel_get(const struct device *dev,
			    enum sensor_channel chan,
			    struct sensor_value *val)
{
	struct ap3216c_data *drv_data = dev->data;

	__ASSERT_NO_MSG(chan == SENSOR_CHAN_AMBIENT_TEMP ||
			chan == SENSOR_CHAN_HUMIDITY);

	// if (chan == SENSOR_CHAN_AMBIENT_TEMP) {
	// 	/* val = sample / 32 - 50 */
	// 	val->val1 = drv_data->t_sample / 32U - 50;
	// 	val->val2 = (drv_data->t_sample % 32) * (1000000 / 32);
	// } else if (chan == SENSOR_CHAN_HUMIDITY) {
	// 	/* val = sample / 16 -24 */
	// 	val->val1 = drv_data->rh_sample / 16U - 24;
	// 	val->val2 = (drv_data->rh_sample % 16) * (1000000 / 16);
	// } else {
	// 	return -ENOTSUP;
	// }

	return 0;
}

static DEVICE_API(sensor, ap3216c_driver_api) = {
	.sample_fetch = ap3216c_sample_fetch,
	.channel_get = ap3216c_channel_get,
};

static int ap3216c_init(const struct device *dev)
{
	const struct ap3216c_config *cfg = dev->config;

	if (!device_is_ready(cfg->i2c.bus)) {
		LOG_ERR("Bus device is not ready");
		return -ENODEV;
	}

    LOG_INF("ap3216c_init\r\n");

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