#ifndef COMMON
#define COMMON

#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include <cstdint>
#include <iostream>

#ifndef PICO_RP2040
#include <chrono>
#else
#include "pico/stdlib.h"
#endif

template <typename T>
inline void xorSwap(T &x, T &y)
{
    x ^= y;
    y ^= x;
    x ^= y;
}

template <typename T>
inline void swap(T &x, T &y)
{
    T temp = x;
    x = y;
    y = temp;
}

// Assumes little endian
void printBits(size_t const size, void const *const ptr)
{
    unsigned char *b = (unsigned char *)ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--)
    {
        for (j = 0; j < 8; j++)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

uint16_t countBits(size_t const size, void const *const ptr)
{
    uint16_t count = 0;
    unsigned char *b = (unsigned char *)ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--)
    {
        for (j = 0; j < 8; j++)
        {
            count += ((b[i] >> j) & 1);
        }
    }
    return count;
}

inline size_t resetMask(uint8_t *&mask, size_t size, bool initialValue = true)
{
    size_t bytes = (size / 8);
    size_t bits = (size % 8);

    memset(mask, 0, bytes + 1);
    if (initialValue)
    {
        memset(mask, 0xFF, bytes);
        memset(&mask[bytes], 0xFF << (8 - bits), 1);
    }

    return bytes + 1;
}

inline size_t createBitMask(uint8_t *&mask, size_t size, bool initialValue = true)
{
    size_t bytes = (size / 8);
    size_t bits = (size % 8);

    mask = (uint8_t *)malloc(bytes + 1);

    memset(mask, 0, bytes + 1);
    if (initialValue)
    {
        memset(mask, 0xFF, bytes);
        memset(&mask[bytes], 0xFF << (8 - bits), 1);
    }

    return bytes + 1;
}

inline bool getBit(uint8_t *mask, size_t input)
{
    size_t byte = (input / 8);
    uint8_t bit = (input % 8);

    bit = 1 << (7 - bit);

    return mask[byte] & bit;
}

inline void flagBit(uint8_t *mask, size_t input)
{
    size_t byte = (input / 8);
    uint8_t bit = (input % 8);

    bit = 1 << (7 - bit);

    uint8_t value = mask[byte];

    mask[byte] = value | bit;
}

inline void clearBit(uint8_t *mask, size_t input)
{
    size_t byte = (input / 8);
    uint8_t bit = (input % 8);

    bit = 1 << (7 - bit);

    uint8_t value = mask[byte];

    mask[byte] = value & ~bit;
}

inline void clearBits(uint8_t *mask, size_t start, size_t length)
{
    for (size_t i = start; i <= (length + start); i++)
    {
        flagBit(mask, i);
    }
}

inline void flagBits(uint8_t *mask, size_t start, size_t length)
{
    for (size_t i = start; i <= (length + start); i++)
    {
        flagBit(mask, i);
    }
}

template <typename T>
void measureExecutionTime(int day, int part, bool test, T func())
{
    uint64_t timeResult;
    T testResult;
    std::cout << "---- Day " << day << " part " << part << (test ? " TEST" : " ") << "----\n";
#ifdef PICO_RP2040
    uint64_t startTime, endTime;
    startTime = time_us_64();
    testResult = func();
    endTime = time_us_64();
    timeResult = endTime - startTime;

#else
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::microseconds;

    auto startTime = high_resolution_clock::now();
    testResult = func();
    auto endTime = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    timeResult = duration_cast<microseconds>(endTime - startTime).count();
#endif
    std::cout << "Result: " << testResult << " in " << timeResult << "us\n";
}

template <typename T>
void printBuffer(const char *prefix, T *buffer, size_t length)
{
    std::cout << prefix << " = [";
    for (size_t i = 0; i < length; i++)
    {
        std::cout << buffer[i] << ", ";
    }
    std::cout << "]\n";
}

template <typename T>
T readNumber(const char *&position, char end)
{
    char c;
    T value = 0;
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

template <typename T>
bool readNumbers(const char *&source, T *&collection, size_t &collectionLength, const char seperator)
{
    char c;
    size_t length = 0;
    T value = 0;

    collection = (T *)malloc(length * sizeof(T));

    while (true)
    {
        c = *source;
        switch (c)
        {
        case '\0':
        case '\n':
            collection = (T *)realloc(collection, (++length) * sizeof(T));
            collection[length - 1] = value;
            collectionLength = length;
            source++;
            return c == '\n' ? 1 : 0;
            break;
        default:
            if (c == seperator)
            {
                collection = (T *)realloc(collection, (++length) * sizeof(T));
                collection[length - 1] = value;
                value = 0;
                break;
            }
            value = (value * 10) + (c - 48);
            break;
        }
        source++;
    }
    return 0;
}

template <typename T>
inline T concat(T a, T b)
{
    T x = b;
    while (x)
    {
        x /= 10;
        a *= 10;
    }
    return a + b;
}

#endif /* COMMON */
