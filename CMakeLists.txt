# Include the required subdirectories
add_subdirectory(drivers)

# Add subdirectories to the compiler's include search path
zephyr_include_directories(drivers)