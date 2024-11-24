#include <stdio.h>
#include <stdlib.h>

void sort(int *arr, int n) {

    asm volatile (
            ".intel_syntax noprefix;"

            "while_outer:;"
            "cmp  ecx, %[n];"
            "je  while_outer_end;"

            "mov    edx, 0;"

            "while_inner:;"
            "dec  %[n];"
            "cmp  edx, %[n];"
            "je   while_inner_end;"
            "inc  %[n];"

            "mov    r10d, [rax + rdx*4];"
            "cmp    r10d, [rax + rdx*4 + 4];"
            "jng    if_end;"

            "mov    r10d, [rax + rdx*4];"
            "mov    r11d, [rax + rdx*4 + 4];"
            "mov    [rax + rdx*4], r11d;"              // swap
            "mov    [rax + rdx*4 + 4], r10d;"

            "if_end:;"
            "inc    edx;"
            "jmp    while_inner;"
            "while_inner_end:;"
            "inc    %[n];"

            "inc    ecx;"
            "jmp    while_outer;"
            "while_outer_end:;"

            ".att_syntax;"
            :
            :"a" (arr),[n] "S"(n), "c"(0), "d"(0)
    );
}

int main(void) {

    const char *inputPath = "";
    const char *outputPath = "";

    FILE *inputFile = fopen(inputPath, "r");
    FILE *outputFile = fopen(outputPath, "w");

    int n;
    fscanf(inputFile, "%d", &n);

    int *arr = (int *) malloc(n * sizeof(int));

    for (int i = 0; i < n; ++i) {
        fscanf(inputFile, "%d", &arr[i]);
    }
    fclose(inputFile);

    sort(arr, n);

    for (int i = 0; i < n; ++i) {
        fprintf(outputFile, "%d\n", arr[i]);
    }

    fclose(outputFile);
    free(arr);

    return 0;
}