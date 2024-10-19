#include <stdio.h>

int fib(int x) {
    if (x > 1) {
        return fib(x-1) + fib(x-2);
    }
    return 1;
}

int main() {
    for (int i = 3; i < 1000; ++i) {
        int res = fib(i);
        if (res == 2584) {
            printf("%d\n", i);
            break;
        }
    }
}