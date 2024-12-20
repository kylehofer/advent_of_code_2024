#include "../../inputs/day4.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "math.h"
#include "../../utilities/common.h"

#define DAY 4

char stuff[] = {'X', 'M', 'A', 'S'};

bool inline part1XmasCheck(char x, char m, char a, char s)
{
    return (x == 'X' && m == 'M' && a == 'A' && s == 'S') ||
           (x == 'S' && m == 'A' && a == 'M' && s == 'X');
}

bool inline part2XmasCheck(char m, char a, char s)
{
    return (m == 'M' && a == 'A' && s == 'S') ||
           (m == 'S' && a == 'A' && s == 'M');
}

uint32_t part1(const char *input)
{
    size_t width = 0;
    while (input[width++] != '\n')
    {
    }
    size_t next = width + 1;
    size_t size = strlen(input);
    size_t rightCutoff = width - 4;
    size_t height = size / (width + 1);
    size_t y1 = next - 1;
    size_t y2 = y1 << 1;
    size_t y3 = y1 + y2;

    size_t leftCutoff = 3;
    size_t bottomCutoff = height - 3;

    uint32_t count = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (input[i] == '\n' || input[i] == 'M' || input[i] == 'A')
        {
            continue;
        }
        if (input[i] == '\0')
        {
            return count;
        }
        size_t x = i % y1;
        size_t y = i / y1;

        size_t canRight = (x < rightCutoff);
        size_t canLeft = (x >= leftCutoff);
        size_t canBottom = (y <= bottomCutoff);
        count += canRight && part1XmasCheck(input[i], input[i + 1], input[i + 2], input[i + 3]);
        count += canBottom && part1XmasCheck(input[i], input[i + y1], input[i + y2], input[i + y3]);
        count += canBottom && canRight && part1XmasCheck(input[i], input[i + y1 + 1], input[i + y2 + 2], input[i + y3 + 3]);
        count += canBottom && canLeft && part1XmasCheck(input[i], input[(i + y1) - 1], input[(i + y2) - 2], input[(i + y3) - 3]);
    }

    return count;
}

uint32_t part2(const char *input)
{
    size_t width = 0;
    while (input[width++] != '\n')
    {
    }
    size_t next = width + 1;
    size_t size = strlen(input);
    size_t rightCutoff = width - 3;
    size_t height = size / (width + 1);
    size_t y1 = next - 1;
    size_t y2 = y1 << 1;
    size_t y3 = y1 + y2;

    size_t leftCutoff = 2;
    size_t bottomCutoff = height - 2;

    uint32_t count = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (input[i] == '\n' || input[i] == 'X' || input[i] == 'A')
        {
            continue;
        }
        if (input[i] == '\0')
        {
            return count;
        }
        size_t x = i % y1;
        size_t y = i / y1;

        size_t canRight = (x < rightCutoff);
        size_t canBottom = (y <= bottomCutoff);
        count += canRight && canBottom &&
                 part2XmasCheck(input[i], input[i + 1 + y1], input[i + 2 + y2]) &&
                 part2XmasCheck(input[i + y2], input[i + 1 + y1], input[i + 2]);
    }

    return count;
}

int main()
{
#ifdef PICO_RP2040
    stdio_init_all();
    while (true)
    {
        sleep_ms(1000);
#endif
        measureExecutionTime<uint32_t>(DAY, 1, true, []() -> uint32_t
                                       { return part1(TEST_INPUT); });
        measureExecutionTime<uint32_t>(DAY, 1, false, []() -> uint32_t
                                       { return part1(INPUT); });
        measureExecutionTime<uint32_t>(DAY, 2, true, []() -> uint32_t
                                       { return part2(TEST_INPUT); });
        measureExecutionTime<uint32_t>(DAY, 2, false, []() -> uint32_t
                                       { return part2(INPUT); });
#ifdef PICO_RP2040
    }
#endif
}