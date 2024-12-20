#include "../../inputs/day5.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "math.h"
#include "../../utilities/common.h"

#define DAY 5

inline const char *findList(const char *input)
{
    size_t position = 0;
    while (input[position++] != '\n' || input[position] != '\n')
    {
    }
    return &input[position + 1];
}

uint8_t readNumber(const char *&position, char end)
{
    char c;
    uint8_t value = 0;
    while (c = *(position))
    {
        if (c >= 48 && c <= 57)
        {
            value = (value * 10) + (c - 48);
            position += 1;
        }
        else if (c == end || c == '\n')
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

size_t read(size_t start, const char *source, uint8_t **collection, size_t &collectionLength, const char seperator)
{
    char c;
    size_t i = start;
    size_t length = 0;
    uint8_t value = 0;

    uint8_t result[256] = {0};

    while (true)
    {
        c = source[i];
        switch (c)
        {
        case '\0':
        case '\n':
            result[length++] = value;
            *collection = (uint8_t *)malloc(length * sizeof(uint8_t));
            memcpy(*collection, result, length * sizeof(uint8_t));
            collectionLength = length;

            return c == '\n' ? i + 1 : i;
            break;
        default:
            if (c == seperator)
            {
                result[length++] = value;
                value = 0;
                break;
            }
            value = (value * 10) + (c - 48);
            break;
        }
        i++;
    }
    return 0;
}

inline uint8_t part1Check(uint8_t *pageBuffer, size_t pageBufferLength, const char *ruleInput)
{
    const char *position = ruleInput;

    bool valid = true;

    do
    {
        bool hasStart = false;
        bool hasEnd = false;
        uint8_t start = readNumber(position, '|');
        position++;
        uint8_t end = readNumber(position, '\n');
        for (size_t i = 0; i < pageBufferLength; i++)
        {
            if (!hasStart)
            {
                if (pageBuffer[i] == start)
                {
                    valid = !hasEnd;
                    hasStart = true;
                }
                hasStart |= pageBuffer[i] == start;
            }
            if (!hasEnd)
            {
                hasEnd |= pageBuffer[i] == end;
            }
        }

    } while (valid && *(++position) != '\n');

    return valid
               ? pageBuffer[(pageBufferLength / 2)]
               : 0;
}

uint32_t part1(const char *input)
{
    uint8_t *pageBuffer;
    size_t pageBufferLength = 0;
    size_t start = 0;
    uint32_t total = 0;

    const char *pageList = findList(input);

    while (pageList[start] != '\0' && (start = read(start, pageList, &pageBuffer, pageBufferLength, ',')))
    {
        total += part1Check(pageBuffer, pageBufferLength, input);
        free(pageBuffer);
    }

    return total;
}

inline uint8_t part2Check(uint8_t *pageBuffer, size_t pageBufferLength, const char *ruleInput)
{
    const char *position = ruleInput;

    bool valid = true;

    do
    {
        int startIndex = -1;
        int endIndex = -1;
        uint8_t start = readNumber(position, '|');
        position++;
        uint8_t end = readNumber(position, '\n');
        for (size_t i = 0; i < pageBufferLength; i++)
        {
            if (startIndex == -1)
            {
                if (pageBuffer[i] == start && endIndex > -1)
                {
                    // XOR Swap
                    pageBuffer[i] ^= pageBuffer[endIndex];
                    pageBuffer[endIndex] ^= pageBuffer[i];
                    pageBuffer[i] ^= pageBuffer[endIndex];
                    valid = false;
                    position = ruleInput;
                    break;
                }
            }
            if (endIndex == -1 && pageBuffer[i] == end)
            {
                endIndex = i;
            }
        }

    } while (*(++position) != '\n');

    return !valid
               ? pageBuffer[(pageBufferLength / 2)]
               : 0;
}

uint32_t part2(const char *input)
{
    uint8_t *pageBuffer;
    size_t pageBufferLength = 0;
    size_t start = 0;
    uint32_t total = 0;

    const char *pageList = findList(input);

    while (pageList[start] != '\0' && (start = read(start, pageList, &pageBuffer, pageBufferLength, ',')))
    {
        total += part2Check(pageBuffer, pageBufferLength, input);
        free(pageBuffer);
    }

    return total;
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
