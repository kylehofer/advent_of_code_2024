#include "../../inputs/day3.h"
#include "stdint.h"
#include "stdio.h"
#include "../../utilities/common.h"

#define DAY 3

uint32_t readNumber(const char *&position, char end)
{
    char c;
    uint32_t value = 0;
    while (c = *(position))
    {
        if (c >= 48 && c <= 57)
        {
            value = (value * 10) + (c - 48);
            position += 1;
        }
        else if (c == end)
        {
            return value;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

#define READ_AND_VALIDATE(position, expected) \
    if (*position == expected)                \
        position += 1;                        \
    else                                      \
        return 0;

int readDoDont(const char *&position)
{
    READ_AND_VALIDATE(position, 'd')
    READ_AND_VALIDATE(position, 'o')
    if (*position == '(')
    {
        READ_AND_VALIDATE(position, '(')
        READ_AND_VALIDATE(position, ')')
        return 1;
    }

    READ_AND_VALIDATE(position, 'n')
    READ_AND_VALIDATE(position, '\'')
    READ_AND_VALIDATE(position, 't')
    READ_AND_VALIDATE(position, '(')
    READ_AND_VALIDATE(position, ')')

    return -1;
}

uint32_t readMult(const char *&position)
{
    READ_AND_VALIDATE(position, 'm')
    READ_AND_VALIDATE(position, 'u')
    READ_AND_VALIDATE(position, 'l')
    READ_AND_VALIDATE(position, '(')
    uint32_t x = readNumber(position, ',');
    READ_AND_VALIDATE(position, ',')
    uint32_t y = readNumber(position, ')');
    READ_AND_VALIDATE(position, ')')

    return x * y;
}

uint32_t part2(const char *input)
{
    char c;
    size_t i = 0;
    const char *position = input;
    uint32_t sum = 0;
    bool enabled = true;
    while (*position != '\0')
    {
        switch (*position)
        {
        case 'd':
        {
            switch (readDoDont(position))
            {
            case 1:
                enabled = true;
                break;
            case -1:
                enabled = false;
                break;
            default:
                break;
            }
        }
        case 'm':
            sum += enabled * readMult(position);
            break;
        default:
            position += 1;
            break;
        }
    }

    return sum;
}

uint32_t part1(const char *input)
{
    char c;
    size_t i = 0;
    const char *position = input;
    uint32_t sum = 0;
    while (*position != '\0')
    {
        switch (*position)
        {
        case 'm':
            sum += readMult(position);
            break;
        default:
            position += 1;
            break;
        }
    }

    return sum;
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
