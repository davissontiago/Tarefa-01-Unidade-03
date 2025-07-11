# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(BH1750_ServoMotor C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

set(MODULES_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules)
set(DISPLAY_MODULE_DIR ${MODULES_BASE_DIR}/display)
set(SERVO_MODULE_DIR ${MODULES_BASE_DIR}/servo)
set(BH1750_MODULE_DIR ${MODULES_BASE_DIR}/bh1750)
set(LEDS_MODULE_DIR ${MODULES_BASE_DIR}/leds)

# Add executable. Default name is the project name, version 0.1
add_executable(BH1750_ServoMotor 
            BH1750_ServoMotor.c
            ${DISPLAY_MODULE_DIR}/display.c
            ${DISPLAY_MODULE_DIR}/inc/ssd1306_i2c.c
            ${SERVO_MODULE_DIR}/servo.c
            ${BH1750_MODULE_DIR}/bh1750.c
            ${LEDS_MODULE_DIR}/leds.c)

pico_set_program_name(BH1750_ServoMotor "BH1750_ServoMotor")
pico_set_program_version(BH1750_ServoMotor "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(BH1750_ServoMotor 0)
pico_enable_stdio_usb(BH1750_ServoMotor 1)

# Add the standard library to the build
target_link_libraries(BH1750_ServoMotor
        pico_stdlib
        hardware_pwm
        hardware_adc
)

# Add the standard include files to the build
target_include_directories(BH1750_ServoMotor PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${DISPLAY_MODULE_DIR}               
        ${DISPLAY_MODULE_DIR}/inc   
        ${SERVO_MODULE_DIR} 
        ${BH1750_MODULE_DIR}
        ${LEDS_MODULE_DIR}
)

# Add any user requested libraries
target_link_libraries(BH1750_ServoMotor 
        hardware_i2c
        )

pico_add_extra_outputs(BH1750_ServoMotor)

