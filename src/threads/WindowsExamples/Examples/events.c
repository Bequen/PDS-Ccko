#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

char resource[100];
HANDLE event_handle;

DWORD WINAPI events(LPVOID param) {
    int number = (int)param;
    DWORD thread_id = GetCurrentThreadId();

    WaitForSingleObject(event_handle, INFINITE); 

    snprintf(resource, sizeof(resource), "Thread %lu wrote %d", thread_id, number);
    printf("%s\n", resource);

    SetEvent(event_handle); 

    return 0;
}