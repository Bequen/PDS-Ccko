#include <stdio.h>
#include <windows.h>

char resource[100];
HANDLE semaphore_handle;

DWORD WINAPI semaphore(LPVOID param) {
    int number = (int)param;
    DWORD thread_id = GetCurrentThreadId();

    WaitForSingleObject(semaphore_handle, INFINITE);

    Sleep(rand() * thread_id % 1000);
    snprintf(resource, sizeof(resource), "Thread %lu wrote %d", thread_id, number);
    Sleep(rand() * thread_id % 1000);
    printf("%s\n", resource);

    ReleaseSemaphore(semaphore_handle, 1, NULL);

    return 0;
}