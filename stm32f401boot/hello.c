#include <stdio.h>

int fibonacci(int i) {
    if (i <= 1) {
        return 1;
    }
    return fibonacci(i-1) + fibonacci(i-2);
}

int fact(int n) {
    int r;
    r = 1;
    while (n > 0) {
        r = r * n;
        printf("n = %d, r = %d\n", n, r);
        --n;
    }
    return r;
}


int inc(int x) {
    return x + 1;
}

int add2(int x) {
    return inc(inc(x));
}


int main(int argc, char **argv)
{
    int i;
    int y;
    int a;
    int b;
    a = 10;
    b = 20;
    i = 0;
    //i = syscall(22);
    while (i <= 30) {
        printf("fibonacci(%2d) = %d\n", i, fibonacci(i));
        i = i + 1;
    }
    
     printf("%d\n", fact(17));
    i=0;
    while (i <= 1000) {
        //syscall(i);
        i = i + 1;
    }

    printf("16000 == 16000 : %d\n", 16000 == 16000);
    printf("16000 == 17000 : %d\n", 16000 == 17000);
    printf("2 == 2         : %d\n", 2 == 2);
    printf("0 == 0         : %d\n", 0 == 0);
    printf("-1 == -1       : %d\n", -1 == -1);
    printf("\n");

    printf("16000 != 16000 : %d\n", 16000 != 16000);
    printf("16000 != 17000 : %d\n", 16000 != 17000);
    printf("2 != 2         : %d\n", 2 != 2);
    printf("0 != 0         : %d\n", 0 != 0);
    printf("-1 != -1       : %d\n", -1 != -1);
    printf("\n");

    printf("17000 > 16000  : %d\n", 17000 > 16000);
    printf("17000 > 17000  : %d\n", 17000 > 17000);
    printf("16000 > 17000  : %d\n", 16000 > 17000);
    printf("16000 > -17000 : %d\n", 16000 > -17000);
    printf("-16000 > -17000 : %d\n", -16000 > -17000);
    printf("\n");

    printf("17000 >= 16000  : %d\n", 17000 >= 16000);
    printf("17000 >= 17000  : %d\n", 17000 >= 17000);
    printf("16000 >= 17000  : %d\n", 16000 >= 17000);
    printf("16000 >= -17000 : %d\n", 16000 >= -17000);
    printf("-16000 >= -17000 : %d\n", -16000 >= -17000);
    printf("\n");
    
    y = ((((b/a)+(b/a))*2)/2) / ((((b/a)+(b/a))*2)/2);
    printf("%d %d\n",y ,add2(argc));
    return 0;
}
