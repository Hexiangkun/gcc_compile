#include <stdio.h>

int main()
{
    #ifdef DEBUG
        printf("using -D, gcc d.cpp -o d -DDEBUG\n");
    #endif
    int a = 10;
    printf("gcc d.cpp -o d\n");
    return 0;
}