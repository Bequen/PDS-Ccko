#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

char resource[100];

DWORD WINAPI no_synchronization(LPVOID param) {
    int number = (int)param;
    DWORD thread_id = GetCurrentThreadId();

	Sleep(rand() * thread_id % 1000);
    snprintf(resource, sizeof(resource), "Thread %lu wrote %d", thread_id, number);
    Sleep(rand() * thread_id % 1000);
    printf("%s\n", resource);

    return 0;
}