/* Simple C program for coverage testing */
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int uncovered(int a) {
    /* This function is intentionally never called */
    return a * 2;
}

int main() {
    int result = add(3, 4);
    printf("3 + 4 = %d\n", result);

    int diff = subtract(10, 3);
    printf("10 - 3 = %d\n", diff);

    return 0;
}
