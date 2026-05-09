#include <stdint.h>
#include <stdio.h>

uint16_t factorial(uint16_t n) {
    if (n <= 1) {
        return 1;
    }

    return n * factorial(n - 1);
}

int main(void) {
    uint16_t n = 5;
    uint16_t result = factorial(n);

    printf("factorial(%u) = %u\n", n, result);
    return 0;
}