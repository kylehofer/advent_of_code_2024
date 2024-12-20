add_executable(day8
        main.cpp
)

IF(${BUILD_TARGET} STREQUAL "PICO")
        # pull in common dependencies
        target_link_libraries(day8 pico_stdlib)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(day8)

        # enable usb output, disable uart output
        pico_enable_stdio_usb(day8 1)
        pico_enable_stdio_uart(day8 0)
ENDIF()
