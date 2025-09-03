# Custom Zephyr function that imports the button/ subdirectory if the Kconfig
# option CUSTOM_BUTTON is defined
add_subdirectory_ifdef(CONFIG_CUSTOM_BUTTON button)
add_subdirectory_ifdef(CONFIG_AP3216C       ap3216c)
