#include <stdio.h>

int main() {
    printf(
        "fileno(stdin) = %d, fileno(stdout) = %d\n",
        fileno(stdin), fileno(stdout)
    );
    return 0;
}
