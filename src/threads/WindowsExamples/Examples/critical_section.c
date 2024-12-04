#include <stdio.h>
#include <windows.h>

char resource[100];
CRITICAL_SECTION critical_section_obj;

DWORD WINAPI critical_section(LPVOID param) {
    int number = (int)param;
    DWORD thread_id = GetCurrentThreadId();

    EnterCriticalSection(&critical_section_obj);

    snprintf(resource, sizeof(resource), "Thread %lu wrote %d", thread_id, number);
    printf("%s\n", resource);

    LeaveCriticalSection(&critical_section_obj);

    return 0;
}