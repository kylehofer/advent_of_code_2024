#include "../../inputs/day1.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "math.h"

#ifdef PICO_RP2040
#include "pico/stdlib.h"
#endif

inline bool contains(size_t index, size_t *values, size_t valuesLength)
{
    size_t i = 0;
    bool result = true;
    while (i < valuesLength && (result = values[i++] != index))
    {
    }
    return !result;
}

size_t largerOrEqual(size_t start, size_t indexes[], size_t indexLength, uint32_t index, const uint32_t *source, size_t length)
{
    size_t result = length;
    uint32_t current = 0xFFFFFFFF;
    uint32_t value = index < length ? source[index] : 0;

    for (size_t i = start; i < length; i += 2)
    {
        if (contains(i, indexes, indexLength))
        {
            continue;
        }
        uint32_t temp = source[i];
        if (temp < value || temp > current)
        {
            continue;
        }
        result = i;
        current = temp;
    }
    // printf("Finished %lu (%u)\n", result, source[result]);
    return result;
}

void updateIndex(size_t **cache, size_t *cacheSize, size_t index, const uint32_t *input, size_t length)
{
    if (*cache == nullptr)
    {
        *cache = (size_t *)malloc(1);
        *cache[0] = index;
        *cacheSize = 1;
        return;
    }

    if (input[(*cache)[0]] == input[index])
    {
        *cache = (size_t *)realloc(*cache, *cacheSize + 1);
        (*cache)[*cacheSize] = index;
        *cacheSize += 1;
        return;
    }
    free(*cache);

    *cache = (size_t *)malloc(1);
    *cache[0] = index;
    *cacheSize = 1;
}

void part1()
{
    size_t start, leftCacheSize, rightCacheSize;
    size_t *leftCache, *rightCache;

    start = leftCacheSize = rightCacheSize = 0;
    leftCache = rightCache = nullptr;

    size_t left = largerOrEqual(0, leftCache, leftCacheSize, INPUT_SIZE, INPUT, INPUT_SIZE);
    size_t right = largerOrEqual(1, rightCache, rightCacheSize, INPUT_SIZE, INPUT, INPUT_SIZE);
    uint32_t sum = 0;

    while (start < INPUT_SIZE)
    {
        uint32_t diff = INPUT[left] < INPUT[right] ? INPUT[right] - INPUT[left] : INPUT[left] - INPUT[right];

        sum += diff;

        updateIndex(&leftCache, &leftCacheSize, left, INPUT, INPUT_SIZE);
        updateIndex(&rightCache, &rightCacheSize, right, INPUT, INPUT_SIZE);

        left = largerOrEqual(0, leftCache, leftCacheSize, left, INPUT, INPUT_SIZE);
        right = largerOrEqual(1, rightCache, rightCacheSize, right, INPUT, INPUT_SIZE);

        start += 2;
    }

    printf("Part 1: Result: %u.\n");
}

void part2()
{
    uint32_t sum = 0;
    for (size_t l = 0; l < INPUT_SIZE; l += 2)
    {
        for (size_t r = 1; r < INPUT_SIZE; r += 2)
        {
            sum += (INPUT[l] == INPUT[r]) * INPUT[l];
        }
    }
    printf("Part 2: Result: %u.\n");
}

int main()
{
#ifdef PICO_RP2040
    stdio_init_all();
#endif
    part1();
    part2();

    return 0;
}