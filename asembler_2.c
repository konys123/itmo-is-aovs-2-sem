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
            "mov    [rax + rdx*4], r11d;"
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

void find_duplicates(int *input_array, int input_size, int *output_array, int *output_counts, int *size) {
    asm volatile (
            ".intel_syntax noprefix;"

            "mov r10d, 0;"
            "mov r14, 0;"

            "while_1:;"
            "cmp r14d, %[n];"
            "je  while_1_end;"

            "mov r12d, 1;" // счетчик дубликатов

            "mov r8d, [rax + r14*4];"
            "inc r14;"

            "while_2:;"
            "cmp r14d, %[n];"
            "je while_2_end;"

            "mov r9d, [rax + r14*4];"
            "cmp r8d, r9d;"
            "jne while_2_end;"

            "inc r12d;"
            "inc r14;"
            "jmp while_2;"

            "while_2_end:;"
            "cmp r12d, 1;" // проверка есть ли дубликаты
            "je while_1;"

            "mov [rbx + r10*4], r8d;"
            "mov [rcx + r10*4], r12d;"
            "inc r10d;"

            "jmp while_1;"

            "while_1_end:;"

            "mov [rdx], r10d;"

            ".att_syntax;"
            :
            :"a" (input_array), "b" (output_array), "c" (output_counts), [n] "S"(input_size), "d"(size)
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
    int *out_arr = (int *) malloc(n * sizeof(int));
    int *count_arr = (int *) malloc(n * sizeof(int));
    int *size = (int *) malloc(sizeof(int));

    for (int i = 0; i < n; ++i) {
        fscanf(inputFile, "%d", &arr[i]);
    }
    fclose(inputFile);

    sort(arr, n);
    find_duplicates(arr, n, out_arr, count_arr, size);

    for (int i = 0; i < size[0]; ++i) {
        fprintf(outputFile, "%d - %d\n", out_arr[i], count_arr[i]);
    }

    if (size[0] == 0) {
        fprintf(outputFile, "%s", "None");
    }

    fclose(outputFile);

    free(arr);
    free(count_arr);
    free(out_arr);
    free(size);

    return 0;
}