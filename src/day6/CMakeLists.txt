add_executable(day6
        main.cpp
)

IF(${BUILD_TARGET} STREQUAL "PICO")
        # pull in common dependencies
        target_link_libraries(day6 pico_stdlib)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(day6)

        # enable usb output, disable uart output
        pico_enable_stdio_usb(day6 1)
        pico_enable_stdio_uart(day6 0)
ENDIF()
