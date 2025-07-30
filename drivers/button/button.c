// Ties to the 'compatible = "custon,button"' node in the Devicetree
#define DT_DRV_COMPAT custom_button

#include <errno.h>
#include <zephyr/logging/log.h>

#include "button.h"

// Enable logging at CONFIG_LOG_DEFAULT_LEVEL
LOG_MODULE_REGISTER(button);



static int button_init(const struct device *dev);
static int button_state_get(const struct device *dev, uint8_t *state);


// Initialize the button
static int button_init(const struct device *dev)
{
    const struct button_config *config = dev->config;
    const struct gpio_dt_spec *btn = &config->btn;

    LOG_DBG("Initializing button (instance ID: %u)\r\n", config->id);

    // Check if the button GPIO is ready
    if (!gpio_is_ready_dt(btn)) {
        LOG_ERR("Button GPIO is not ready");
        return -ENODEV;
    }

    // Configure the GPIO pin as input with pull-up
    int ret = gpio_pin_configure_dt(btn, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        LOG_ERR("Failed to configure GPIO pin: %d", ret);
        return -ENODEV;
    }

    LOG_INF("Button initialized on pin %d", config->btn.pin);
    return 0;
}

static int button_state_get(const struct device *dev, uint8_t *state)
{
    const struct button_config *config = dev->config;
    const struct gpio_dt_spec *btn = &config->btn;

    // Read the button state
    int ret = gpio_pin_get_dt(btn);
    if (ret < 0) {
        LOG_ERR("Failed to read button state: %d", ret);
        return -EIO;
    }

    // Invert the state because pull-up is used
    *state = (ret == 0) ? 1 : 0;

    LOG_DBG("Button state: %d", *state);
    return 0;
}


// Devicetree handling

static const struct button_api button_driver_api = {
    .get = button_state_get,
};

// Expansion macro to define driver instances
#define BUTTON_DEFINE(inst) \
    static const struct button_config button_config_##inst = { \
        .btn = GPIO_DT_SPEC_GET(DT_PHANDLE(DT_INST(inst, custom_button), pin), gpios), \
        .id = inst, \
    }; \
    DEVICE_DT_INST_DEFINE(inst, button_init, NULL, NULL, &button_config_##inst, \
                          POST_KERNEL, CONFIG_GPIO_INIT_PRIORITY, \
                          &button_driver_api); 

DT_INST_FOREACH_STATUS_OKAY(BUTTON_DEFINE)