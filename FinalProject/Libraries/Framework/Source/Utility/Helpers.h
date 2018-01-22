#ifndef __Helpers_H__
#define __Helpers_H__

struct Point
{
    int x, y;

    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};

void OutputMessage(const char* message, ...);
char* LoadCompleteFile(const char* filename, long* length);
double GetSystemTime();

#endif //__Helpers_H__
