#include <stdio.h>
#include <windows.h>

char resource[100];
HANDLE mutex_obj;

DWORD WINAPI mutex(LPVOID param) {
    int number = (int)param;
    DWORD thread_id = GetCurrentThreadId();

    WaitForSingleObject(mutex_obj, INFINITE);

    Sleep(rand() * thread_id % 1000);
    snprintf(resource, sizeof(resource), "Thread %lu wrote %d", thread_id, number);
    Sleep(rand() * thread_id % 1000);
    printf("%s\n", resource);

    ReleaseMutex(mutex_obj);

    return 0;
}
