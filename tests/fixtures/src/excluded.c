/* C file used to test the excludes input - this file should be excluded from coverage */
#include <stdio.h>

int excluded_function(int x) {
    /* This function is in a file that will be excluded */
    return x * 3;
}
