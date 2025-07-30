#ifndef ZEPHYER_DRIVERS_BUTTON_H_
#define ZEPHYER_DRIVERS_BUTTON_H_

#include <zephyr/drivers/gpio.h>

// Because we're not using sensor's predefined API, we need to declare our own
struct button_api {
    int (*get)(const struct device *dev, uint8_t *state);
};

struct button_config {
    struct gpio_dt_spec btn;
    uint32_t id;
};

#endif