cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_PATH /home/test/opensrc/pico/pico-sdk-1.5.1)
set(RASP_PICO_SDK_PATH /home/test/opensrc/pico/pico-sdk-1.5.1)

include(${RASP_PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_demo)

pico_sdk_init()

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

add_subdirectory(pico)
