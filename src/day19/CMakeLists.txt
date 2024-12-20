add_executable(day19
        main.cpp
)

IF(${BUILD_TARGET} STREQUAL "PICO")
        # pull in common dependencies
        target_link_libraries(day19 pico_stdlib)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(day19)

        # enable usb output, disable uart output
        pico_enable_stdio_usb(day19 1)
        pico_enable_stdio_uart(day19 0)
ENDIF()
