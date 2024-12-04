#include <stdio.h>
#include <windows.h>

CONDITION_VARIABLE cond_var;
CRITICAL_SECTION cs;
volatile int count = 0;

DWORD WINAPI thread1_func(LPVOID param) {
    DWORD thread_id = GetCurrentThreadId();

    printf("Thread %ld is doing some work\n", thread_id);
    Sleep(1000);

    EnterCriticalSection(&cs);
    printf("Thread %ld is waiting for count to be greater or equal to 5\n", thread_id);
    while (count < 5)
    {
		SleepConditionVariableCS(&cond_var, &cs, INFINITE);
    }
    LeaveCriticalSection(&cs);

    printf("Thread %ld is continuing its work\n", thread_id);

    return 0;
}

DWORD WINAPI thread2_func(LPVOID param) {
    DWORD thread_id = GetCurrentThreadId();

    printf("Thread %ld is doing some work\n", thread_id);
    Sleep(3000);

	for (int i = 0; i < 5; i++)
	{
		EnterCriticalSection(&cs);    
		count++;
        printf("Thread %ld increases count to %d\n", thread_id, count);
        WakeConditionVariable(&cond_var);
		LeaveCriticalSection(&cs);
		Sleep(1000);
	}

    printf("Thread %ld is continuing its work\n", thread_id);

    return 0;
}

DWORD WINAPI condition_variable() {
    HANDLE thread1, thread2;

    InitializeCriticalSection(&cs);
    InitializeConditionVariable(&cond_var);

    thread1 = CreateThread(NULL, 0, thread1_func, NULL, 0, NULL);
    if (thread1 == NULL) {
        printf("Error creating thread: %ld\n", GetLastError());
        return 1;
    }

    thread2 = CreateThread(NULL, 0, thread2_func, NULL, 0, NULL);
    if (thread2 == NULL) {
        printf("Error creating thread: %ld\n", GetLastError());
        return 1;
    }

    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);

    CloseHandle(thread1);
    CloseHandle(thread2);

    DeleteCriticalSection(&cs);

    return 0;
}