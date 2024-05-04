#include <stdio.h>

int fibonacci(int i) {
    if (i <= 1) {
        return 1;
    }
    return fibonacci(i-1) + fibonacci(i-2);
}



int main()
{
    int i;
    int y;
    int a;
    int b;
    a = 10;
    b = 20;
    i = 0;
    i = syscall(22);
    while (i <= 30) {
        printf("fibonacci(%2d) = %d\n", i, fibonacci(i));
        i = i + 1;
    }

    i=0;
    while (i <= 1000) {
        syscall(i);
        i = i + 1;
    }


    y = ((((b/a)+(b/a))*2)/2) / ((((b/a)+(b/a))*2)/2);
    printf("%d\n",y );
    return 0;
}
