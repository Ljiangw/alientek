# Declare the current directory as a Zephyr library
# If no name is given, the name is derived from the directory name
zephyr_library()

# List the source files for the button driver
zephyr_library_sources(button.c)

# Add header files to the include directories
zephyr_include_directories(.)