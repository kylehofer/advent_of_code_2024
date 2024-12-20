#include "../../inputs/day6.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "math.h"
#include "../../utilities/common.h"

#define DAY 6

enum Direction
{
    UP,
    DOWN,
    RIGHT,
    LEFT
};

enum LineType
{
    HORIZONTAL,
    VERTICLE
};

size_t findStart(const char *input)
{
    size_t index = 0;
    while (input[index++] != '^')
    {
    }
    return index - 1;
}

inline LineType getLineType(
    uint16_t x1, uint16_t y1,
    uint16_t x2, uint16_t y2)
{
    return (x1 == x2)
               ? VERTICLE
               : HORIZONTAL;
}

inline void calculateOverlap(
    uint8_t *mask,
    LineType a,
    uint16_t ax1, uint16_t ay1,
    uint16_t ax2, uint16_t ay2,
    uint16_t bx1, uint16_t by1,
    uint16_t bx2, uint16_t by2)
{

    uint16_t value;

    if (a == HORIZONTAL)
    {
        // Either Lines are on a different axis
        // or lines lie outside each other
        if (ay1 != by1 || ax1 > bx2 || bx1 > ax2)
            return;
        // Line A is before line B
        if (ax1 < bx1)
        {
            clearBits(mask, bx1, (ax2 < bx2 ? ax2 - bx1 : bx2 - bx1));
            return;
        }

        clearBits(mask, 0, (ax2 < bx2 ? ax2 - ax1 : bx2 - ax1));
        return;
    }

    // Either Lines are on a different axis
    // or lines lie outside each other
    if (ax1 != bx1 || ay1 > by2 || by1 > ay2)
        return;

    if (ay1 < by1)
    {
        // printf("start: %u, length: %u\n", by1 - ay1, (ay2 < by2 ? ay2 - by1 : by2 - by1));
        clearBits(mask, by1 - ay1, (ay2 < by2 ? ay2 - by1 : by2 - by1));
        return;
    }
    // printf("h3\n");
    clearBits(mask, 0, (ay2 < by2 ? ay2 - ay1 : by2 - ay1));
}

inline void calculateIntersection(

    uint8_t *mask,
    LineType a,
    uint16_t ax1, uint16_t ay1,
    uint16_t ax2, uint16_t ay2,
    uint16_t bx1, uint16_t by1,
    uint16_t bx2, uint16_t by2)
{
    if (a == VERTICLE)
    {
        if (ax1 >= bx1 && ax1 <= bx2 && by1 >= ay1 && by1 <= ay2)
        {
            clearBit(mask, by1 - ay1);
        }
        return;
    }
    if (ay1 >= by1 && ay1 <= by2 && bx1 >= ax1 && bx1 <= ax2)
    {
        clearBit(mask, bx1 - ax1);
    }
}

inline void calculateCrossover(
    uint8_t *mask,
    LineType a,
    uint16_t ax1, uint16_t ay1,
    uint16_t ax2, uint16_t ay2,
    LineType b,
    uint16_t bx1, uint16_t by1,
    uint16_t bx2, uint16_t by2)
{
    (a == b)
        ? calculateOverlap(mask, a, ax1, ay1, ax2, ay2, bx1, by1, bx2, by2)
        : calculateIntersection(mask, a, ax1, ay1, ax2, ay2, bx1, by1, bx2, by2);
}

inline uint16_t lineLength(uint16_t x1, uint16_t y1,
                           uint16_t x2, uint16_t y2)
{
    return ((x1 < x2 ? x2 - x1 : x1 - x2) +
            (y1 < y2 ? y2 - y1 : y1 - y2)) +
           1;
}

inline size_t lineMask(uint8_t *&mask, uint16_t x1, uint16_t y1,
                       uint16_t x2, uint16_t y2)
{
    size_t size = ((x1 < x2 ? x2 - x1 : x1 - x2) +
                   (y1 < y2 ? y2 - y1 : y1 - y2)) +
                  1;

    return createBitMask(mask, size);
}

inline bool updatePosition(const char *input, size_t size, size_t &position, Direction direction, size_t yIncrement)
{
    switch (direction)
    {
    case UP:
        if (position < yIncrement)
        {
            return false;
        }
        else
        {
            position -= yIncrement;
        }
        break;
    case DOWN:
        position += yIncrement;
        if (position >= size)
        {
            position -= yIncrement;
            return false;
        }
        break;
    case LEFT:
        if (position == 0)
        {
            return false;
        }
        position -= 1;
        if (input[position] == '\n')
        {
            position += 1;
            return false;
        }
        break;
    case RIGHT:
        position += 1;
        if (position >= size || input[position] == '\n')
        {
            position -= 1;
            return false;
        }
        break;
    default:
        break;
    }
    return true;
}

inline Direction updateDirection(const char *input, size_t &position, Direction direction, size_t yIncrement)
{
    switch (direction)
    {
    case UP:
        position += yIncrement;
        return RIGHT;
    case DOWN:
        position -= yIncrement;
        return LEFT;
    case LEFT:
        position += 1;
        return UP;
    case RIGHT:
        position -= 1;
        return DOWN;
    default:
        break;
    }
    return direction;
}

inline uint32_t calculateLines(uint16_t *lines, size_t lineCount)
{
    uint32_t count = 0;

    for (size_t i = 0; i < lineCount - 2; i += 2)
    {
        uint16_t xa1 = lines[i];
        uint16_t ya1 = lines[i + 1];
        uint16_t xa2 = lines[i + 2];
        uint16_t ya2 = lines[i + 3];
        LineType a = getLineType(xa1, ya1, xa2, ya2);

        if (xa1 > xa2)
            xorSwap(xa1, xa2);
        else if (ya1 > ya2)
            xorSwap(ya1, ya2);

        uint8_t *mask;
        size_t length = lineMask(mask, xa1, ya1, xa2, ya2);

        for (size_t n = i + 2; n < (lineCount - 2); n += 2)
        {
            uint16_t xb1 = lines[n];
            uint16_t yb1 = lines[n + 1];
            uint16_t xb2 = lines[n + 2];
            uint16_t yb2 = lines[n + 3];

            if (xb1 > xb2)
                xorSwap(xb1, xb2);
            else if (yb1 > yb2)
                xorSwap(yb1, yb2);

            LineType b = getLineType(xb1, yb1, xb2, yb2);

            calculateCrossover(mask, a, xa1, ya1, xa2, ya2,
                               b, xb1, yb1, xb2, yb2);
        }

        uint16_t val = countBits(length, mask);

        count += val;

        free(mask);
    }

    free(lines);

    return count;
}

uint32_t part1(const char *input)
{
    size_t width = 0;
    while (input[width++] != '\n')
    {
    }
    size_t yIncrement = width;
    size_t xIncrement = 1;
    size_t size = strlen(input);

    size_t position = findStart(input);
    Direction direction = UP;

    size_t start,
        end = 0;
    start = position;

    size_t lineCount = 4;
    uint16_t *lines = (uint16_t *)malloc(sizeof(uint16_t) * 2 * lineCount);
    lines[0] = position % yIncrement;
    lines[1] = position / yIncrement;

    while (true)
    {
        if (!updatePosition(input, size, position, direction, yIncrement))
        {
            break;
        }

        if (input[position] == '#')
        {
            direction = updateDirection(input, position, direction, yIncrement);
            lines[lineCount - 2] = position % yIncrement;
            lines[lineCount - 1] = position / yIncrement;
            lineCount += 2;
            lines = (uint16_t *)realloc(lines, sizeof(uint16_t) * lineCount);
        }
    }

    lines[lineCount - 2] = position % yIncrement;
    lines[lineCount - 1] = position / yIncrement;

    return calculateLines(lines, lineCount);
}

struct CornerData
{
    size_t position;
    Direction direction;
};

void printMap(const char *input, size_t size, size_t start)
{
    size_t position = 0;
    while (position < size)
    {
        printf("%c", position == start ? 'X' : input[position]);
        position++;
    }
    puts("");
}

bool part2LoopCheck(const char *input, size_t start, size_t blocker, size_t yIncrement, size_t size)
{

    Direction direction = UP;
    size_t position = start;
    if (input[blocker] == '#')
    {
        return false;
    }

    size_t cornerCount = 0;

    CornerData *corners = (CornerData *)malloc(0);

    while (true)
    {
        if (!updatePosition(input, size, position, direction, yIncrement))
        {
            break;
        }
        if (input[position] == '#' || position == blocker)
        {
            for (size_t i = 0; i < cornerCount; i++)
            {
                if (position == corners[i].position && direction == corners[i].direction)
                {

                    free(corners);

                    return true;
                }
            }

            corners = (CornerData *)realloc(corners, (cornerCount + 1) * sizeof(CornerData));
            corners[cornerCount++] = {
                .position = position,
                .direction = direction};

            direction = updateDirection(input, position, direction, yIncrement);
        }
    }
    free(corners);
    return false;
}

uint32_t part2(const char *input)
{
    size_t size = strlen(input);

    size_t width = 0;
    while (input[width++] != '\n')
    {
    }
    size_t yIncrement = width;
    size_t xIncrement = 1;
    size_t height = size / (width + 1);

    size_t start = findStart(input);
    Direction direction = UP;

    size_t position = start;
    size_t previous = 0;

    uint32_t count = 0;
    size_t *loopPositions = (size_t *)malloc(0);
    size_t loopCount = 0;

    while (true)
    {
        if (!updatePosition(input, size, position, direction, yIncrement))
        {
            break;
        }

        if (input[position] == '#')
        {
            direction = updateDirection(input, position, direction, yIncrement);
        }

        bool loopExists = false;
        for (size_t i = 0; i < loopCount; i++)
        {
            if (loopPositions[i] == position)
            {
                loopExists = true;
                break;
            }
        }

        if (!loopExists && part2LoopCheck(input, start, position, yIncrement, size))
        {
            loopPositions = (size_t *)realloc(loopPositions, (++loopCount) * sizeof(size_t));
            loopPositions[loopCount - 1] = position;
        }
        previous = position;
    }

    return loopCount;
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