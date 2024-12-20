// #include "../../inputs/day8.h"
// #include "string.h"
// #include "stdint.h"
// #include "stdio.h"
// #include "math.h"
// #include "memory.h"
// #include "stddef.h"
// #include "pico/stdlib.h"
#include <stdio.h>
#include <stdint.h>

// #include "../../utilities/common.h"

// #define DAY 8

// uint32_t part1(const char *input)
// {
//     return 0;
// }

// uint32_t part2(const char *input)
// {
//     return 0;
// }

int main()
{
#ifdef PICO_RP2040
    stdio_init_all();
#endif
    // measureExecutionTime<uint32_t>(DAY, 1, true, []() -> uint32_t
    //                                { return part1(TEST_INPUT); });
    // measureExecutionTime<uint32_t>(DAY, 1, false, []() -> uint32_t
    //                                { return part1(INPUT); });
    // measureExecutionTime<uint32_t>(DAY, 2, true, []() -> uint32_t
    //                                { return part2(TEST_INPUT); });
    // measureExecutionTime<uint32_t>(DAY, 2, false, []() -> uint32_t
    //                                { return part2(INPUT); });

    int i = 0;
    while (true)
    {

        printf("HI! %d\n", i++);
    }
}