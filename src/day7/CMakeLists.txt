add_executable(day7
        main.cpp
)

IF(${BUILD_TARGET} STREQUAL "PICO")
        # pull in common dependencies
        target_link_libraries(day7 pico_stdlib)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(day7)

        # enable usb output, disable uart output
        pico_enable_stdio_usb(day7 1)
        pico_enable_stdio_uart(day7 0)
ENDIF()
