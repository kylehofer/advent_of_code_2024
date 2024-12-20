add_executable(day2
        main.cpp
)

IF(${BUILD_TARGET} STREQUAL "PICO")
        # pull in common dependencies
        target_link_libraries(day2 pico_stdlib)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(day2)

        # enable usb output, disable uart output
        pico_enable_stdio_usb(day2 1)
        pico_enable_stdio_uart(day2 0)
ENDIF()
