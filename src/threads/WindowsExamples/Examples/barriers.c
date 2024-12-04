#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

SYNCHRONIZATION_BARRIER barrier_obj;

DWORD WINAPI barrier(LPVOID param) {
    DWORD thread_id = GetCurrentThreadId();

    Sleep(rand() * thread_id % 1000);
    printf("Thread %ld is working\n", thread_id);
    Sleep(rand() * thread_id % 1000);

    printf("Thread %ld reached barrier and is waiting for others\n", thread_id);
    EnterSynchronizationBarrier(&barrier_obj, 0);

    printf("Thread %ld continues its work\n", thread_id);

    return 0;
}
