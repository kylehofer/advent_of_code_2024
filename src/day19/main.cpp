#include "../../inputs/day19.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "math.h"
#include "memory.h"
#include "stddef.h"

#include <thread>
#include <future>
#include <vector>
#include <set>

#include "../../utilities/common.h"
#define DAY 19

inline size_t nextTowel(const char *&position, const char end)
{
    char c;
    const char *start = position;

    while (c = *(position))
    {
        if (c == end || c == '\0' || c == '\n')
        {
            return position - start;
        }
        position++;
    }
    return position - start;
}

inline bool checkTowel(const char *requiredTowel, size_t requiredTowelLength, const char *towel, size_t towelLength)
{
    if (requiredTowelLength < towelLength)
    {
        return false;
    }
    return memcmp(requiredTowel, towel, towelLength) == 0;
}

inline bool checkTowelStock(const char *requiredTowel, size_t requiredTowelLength, const char *input, uint8_t *mask)
{
    if (requiredTowelLength == 0)
    {
        return true;
    }

    if (getBit(mask, requiredTowelLength))
        return false;

    flagBit(mask, requiredTowelLength);

    const char *stock = input;

    while (true)
    {
        const char *towel = stock;
        size_t towelLength = nextTowel(stock, ',');

        if (
            checkTowel(requiredTowel, requiredTowelLength, towel, towelLength) &&
            checkTowelStock(requiredTowel + towelLength, requiredTowelLength - towelLength, input, mask))
        {
            return true;
        }
        if (*stock == '\n')
        {
            break;
        }
        stock += 2;
    }
    return false;
}

uint32_t part1(const char *input)
{
    const char *required = input;

    while (*(++required) != '\n' || *(required + 1) != '\n')
    {
    }

    required += 2;

    uint32_t count = 0;

    while (true)
    {
        const char *requiredTowel = required;
        size_t requiredTowelSize = nextTowel(required, '\n');

        uint8_t *mask;
        createBitMask(mask, requiredTowelSize, false);

        if (checkTowelStock(requiredTowel, requiredTowelSize, input, mask))
        {
            count++;
        }

        free(mask);

        if (*required == '\0')
        {
            break;
        }

        required += 1;
    }

    return count;
}

inline uint64_t checkTowelCombination(const char *requiredTowel, size_t requiredTowelLength, const char *stock, uint64_t *progressiveCount, size_t progressiveCountLength)
{
    if (requiredTowelLength == 0)
    {
        return 1;
    }

    size_t count = 0;
    progressiveCount[0] = 1;

    size_t memShiftSize = (progressiveCountLength - 1) * sizeof(uint64_t);

    for (size_t i = 0; i < requiredTowelLength; i++)
    {
        uint64_t currentPossibilities = progressiveCount[0];
        if (!currentPossibilities)
        {
            memmove(progressiveCount, progressiveCount + 1, memShiftSize);
            progressiveCount[progressiveCountLength - 1] = 0;
            requiredTowel++;
            continue;
        }
        const char *start = stock;

        while (true)
        {
            const char *towel = start;
            size_t towelLength = nextTowel(start, ',');

            progressiveCount[towelLength] += (currentPossibilities) * (requiredTowelLength >= towelLength && memcmp(requiredTowel, towel, towelLength) == 0);

            if (*start == '\n')
            {
                break;
            }

            start += 2;
        }
        requiredTowel++;

        memmove(progressiveCount, progressiveCount + 1, memShiftSize);
        progressiveCount[progressiveCountLength - 1] = 0;
    }

    return progressiveCount[0];
}

uint64_t part2(const char *input)
{
    const char *required = input;

    size_t stockCount;
    size_t countSize = 0;

    {
        const char *position = input;

        while (true)
        {
            size_t stockLength = nextTowel(position, ',');

            stockCount++;
            countSize = countSize > stockLength ? countSize : stockLength;

            if (*position == '\n')
            {
                break;
            }

            position += 2;
        }

        required = position + 2;
    }

    countSize++;

    uint64_t count = 0;

    uint64_t *progressiveCount = (uint64_t *)malloc(sizeof(uint64_t) * countSize);

    while (true)
    {

        const char *requiredTowel = required;
        size_t requiredTowelSize = nextTowel(required, '\n');

        memset(progressiveCount, 0, sizeof(uint64_t) * countSize);

        count += checkTowelCombination(requiredTowel, requiredTowelSize, input, progressiveCount, countSize);

        if (*required == '\0')
        {
            break;
        }

        required += 1;
    }

    free(progressiveCount);

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
        measureExecutionTime<uint64_t>(DAY, 2, true, []() -> uint64_t
                                       { return part2(TEST_INPUT); });
        measureExecutionTime<uint64_t>(DAY, 2, false, []() -> uint64_t
                                       { return part2(INPUT); });
#ifdef PICO_RP2040
    }
#endif
}