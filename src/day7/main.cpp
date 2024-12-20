#include "../../inputs/day7.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "math.h"
#include "memory.h"
#include "stddef.h"

#include "../../utilities/common.h"

#define DAY 7

template <typename T, typename U>
T part2Check(T expectedResult, U *numbers, size_t numbersCount, T currentValue, size_t index = 0)
{
    index++;
    if (index == numbersCount)
    {
        return currentValue == expectedResult ? expectedResult : 0;
    }
    T result = part2Check(expectedResult, numbers, numbersCount, currentValue * numbers[index], index);
    result = result ? result : part2Check(expectedResult, numbers, numbersCount, currentValue + numbers[index], index);
    return result ? result : part2Check(expectedResult, numbers, numbersCount, concat(currentValue, (T)numbers[index]), index);
}

uint64_t part2(const char *input)
{
    const char *position = input;

    bool valid = true;

    bool hasData = false;
    uint64_t result = 0;

    do
    {
        bool hasStart = false;
        bool hasEnd = false;
        uint64_t expectedResult = readNumber<uint64_t>(position, ':');
        position += 2;
        uint16_t *numbers;
        size_t numbersCount;
        hasData = readNumbers(position, numbers, numbersCount, ' ');

        result += part2Check(expectedResult, numbers, numbersCount, (uint64_t)numbers[0]);

    } while (hasData);

    return result;
}

template <typename T, typename U>
T part1Check(T expectedResult, U *numbers, size_t numbersCount, T currentValue, size_t index = 0)
{
    index++;
    if (index == numbersCount)
    {
        return currentValue == expectedResult ? expectedResult : 0;
    }
    T result = part1Check(expectedResult, numbers, numbersCount, currentValue * numbers[index], index);
    return result ? result : part1Check(expectedResult, numbers, numbersCount, currentValue + numbers[index], index);
}

uint64_t part1(const char *input)
{
    const char *position = input;

    bool valid = true;

    bool hasData = false;
    uint64_t result = 0;

    do
    {
        bool hasStart = false;
        bool hasEnd = false;
        uint64_t expectedResult = readNumber<uint64_t>(position, ':');
        position += 2;
        uint16_t *numbers;
        size_t numbersCount;
        hasData = readNumbers(position, numbers, numbersCount, ' ');

        result += part1Check(expectedResult, numbers, numbersCount, (uint64_t)numbers[0]);

    } while (hasData);

    return result;
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