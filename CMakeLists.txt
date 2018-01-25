cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)

#-------------------------------------------------------------------------------
# User configuration
#-------------------------------------------------------------------------------
# Project name
set(PROJECT_NAME    ArduinoLibraryTM1638)

# Library directory
set(LIBRARY_DIR     C:/Users/User/Documents/Arduino/libraries)

# Configure board, default Arduino UNO
set(BOARD           uno)

# Configure sketch
set(SKETCH          examples/Example/Example.ino)

# Configure serial port, such as:
#     Windows: COM1
#     Linux:   /dev/ttyUSB0
set(SERIAL_PORT     COM1)

#-------------------------------------------------------------------------------
# Project configuation
#-------------------------------------------------------------------------------
# Configure project
project(${PROJECT_NAME})

set(TM1638_LIBRARY_DIR    ${LIBRARY_DIR}/${PROJECT_NAME}/src)

include_directories(${TM1638_LIBRARY_DIR})

generate_arduino_library(
        TM1638Lib
        SRCS
            ${TM1638_LIBRARY_DIR}/TM1638.cpp
        HDRS
            ${TM1638_LIBRARY_DIR}/TM1638.h
)

set(${CMAKE_PROJECT_NAME}_SKETCH    ${SKETCH})
set(${CMAKE_PROJECT_NAME}_LIBS      TM1638Lib)
set(${CMAKE_PROJECT_NAME}_BOARD     ${BOARD})
set(${CMAKE_PROJECT_NAME}_PORT      ${SERIAL_PORT})

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
