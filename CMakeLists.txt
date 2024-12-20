cmake_minimum_required(VERSION 3.17)

SET(BUILD_TARGET LINUX CACHE BOOL "")

IF(${BUILD_TARGET} STREQUAL "PICO")
    # initialize the SDK based on PICO_SDK_PATH
    # note: this must happen before project()
    include(pico_sdk_import.cmake)
ENDIF()

project(advent_of_code_2024 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

IF(${BUILD_TARGET} STREQUAL "PICO")
    add_definitions(-DPICO_RP2040)

    # initialize the Raspberry Pi Pico SDK
    pico_sdk_init()
ENDIF()

include_directories("./")

# rest of your project
add_subdirectory(src/day1)
add_subdirectory(src/day2)
add_subdirectory(src/day3)
add_subdirectory(src/day4)
add_subdirectory(src/day5)
add_subdirectory(src/day6)
add_subdirectory(src/day7)
add_subdirectory(src/day8)
add_subdirectory(src/day19)
