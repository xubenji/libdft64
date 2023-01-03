#include <stdio.h>
#include <stdlib.h>

int main()
{
    int result = system("/usr/bin/ls");
    printf("Result: %d\n", result);
    return 0;
}