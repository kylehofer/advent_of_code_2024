
#include "../../inputs/day2.h"
#include "../../utilities/common.h"

#include "stdint.h"
#include "memory.h"
#include <string>

#define DAY 2

size_t read(size_t start, const char *source, uint32_t **collection, size_t &collectionLength)
{
    char c;
    size_t i = start;
    size_t length = 0;
    uint32_t value = 0;

    uint32_t result[256] = {0};

    while (true)
    {
        c = source[i];
        switch (c)
        {
        case '\0':
        case '\n':
            result[length++] = value;
            *collection = (uint32_t *)malloc(length * sizeof(uint32_t));
            memcpy(*collection, result, length * sizeof(uint32_t));
            collectionLength = length;

            return c == '\n' ? i + 1 : i;
            break;
        case ' ':
            result[length++] = value;
            value = 0;
            break;
        default:
            value = (value * 10) + (c - 48);
            break;
        }
        i++;
    }
    return 0;
    // std::stoi();
}

bool part1Check(uint32_t *buffer, size_t length)
{
    int asc = 0;
    for (size_t i = 1; i < length; i++)
    {
        uint32_t x = buffer[i - 1];
        uint32_t y = buffer[i];
        if (x == y)
        {
            return false;
        }
        uint32_t diff;
        if (x > y)
        {
            if (asc == -1)
            {
                return false;
            }
            asc = 1;
            diff = x - y;
        }
        else
        {
            if (asc == 1)
            {
                return false;
            }
            asc = -1;
            diff = y - x;
        }
        if (diff == 0 || diff > 3)
        {
            return false;
        }
    }
    return true;
}

uint32_t part1(const char *input)
{
    uint32_t *buffer;
    size_t bufferLength = 0;
    size_t start = 0;
    uint32_t total = 0;

    while (input[start] != '\0' && (start = read(start, input, &buffer, bufferLength)))
    {
        total += part1Check(buffer, bufferLength);
        free(buffer);
    }

    return total;
}

bool part2Check(uint32_t *buffer, size_t length);

bool attemptRecovery(size_t index, uint32_t *buffer, size_t length)
{
    uint32_t *beforeBuffer = (uint32_t *)malloc(length * sizeof(uint32_t));
    if (index > 0)
    {
        memcpy(beforeBuffer, buffer, index * sizeof(uint32_t));
    }
    memcpy(&beforeBuffer[index], &buffer[index + 1], (length - (1 + index)) * sizeof(uint32_t));

    if (part1Check(beforeBuffer, length - 1))
    {
        return true;
    }

    uint32_t *afterBuffer = (uint32_t *)malloc(length * sizeof(uint32_t));
    index = index + 1;
    memcpy(afterBuffer, buffer, index * sizeof(uint32_t));
    if (index < (length - 1))
    {
        memcpy(&afterBuffer[index], &buffer[index + 1], (length - (1 + index)) * sizeof(uint32_t));
    }
    return part1Check(afterBuffer, length - 1);
}

bool part2Check(uint32_t *buffer, size_t length)
{
    int asc = 0;
    bool canRecover = true;
    for (size_t i = 1; i < length; i++)
    {
        uint32_t x = buffer[i - 1];
        uint32_t y = buffer[i];
        if (x == y)
        {
            return attemptRecovery(i - 1, buffer, length);
        }
        uint32_t diff;
        if (x > y)
        {
            if (asc == -1)
            {
                return attemptRecovery(i - 1, buffer, length);
            }
            asc = 1;
            diff = x - y;
        }
        else
        {
            if (asc == 1)
            {
                return attemptRecovery(i - 1, buffer, length);
            }
            asc = -1;
            diff = y - x;
        }
        if (diff == 0 || diff > 3)
        {
            return attemptRecovery(i - 1, buffer, length);
        }
    }
    return true;
}

uint32_t part2(const char *input)
{
    uint32_t *buffer;
    size_t bufferLength = 0;
    size_t start = 0;
    uint32_t total = 0;

    while (input[start] != '\0' && (start = read(start, input, &buffer, bufferLength)))
    {
        if (!part1Check(buffer, bufferLength))
        {
            for (size_t i = 0; i < bufferLength; i++)
            {
                if (attemptRecovery(i, buffer, bufferLength))
                {
                    total++;
                    break;
                };
            }
        }
        else
        {
            total++;
        }
        free(buffer);
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