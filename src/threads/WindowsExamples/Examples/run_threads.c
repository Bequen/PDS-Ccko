#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

int run_threads(int nr_of_threads, void (*func)()) {
    HANDLE* threads = (HANDLE*)malloc(nr_of_threads * sizeof(HANDLE));

    if (threads == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < nr_of_threads; i++) {
        threads[i] = CreateThread(
            NULL,
            0,
            func,
            (LPVOID)i,
            0,
            NULL);

        if (threads[i] == NULL) {
            printf("Error creating thread %d: %ld\n", i, GetLastError());
            return 1;
        }
    }

    WaitForMultipleObjects(nr_of_threads, threads, TRUE, INFINITE);

    for (int i = 0; i < nr_of_threads; i++) {
        CloseHandle(threads[i]);
    }

    free(threads);

    return 0;
}
