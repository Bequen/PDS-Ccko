#include <stdio.h>
#include <windows.h>
#include <stdlib.h>


HANDLE mutex_obj;
HANDLE semaphore_handle;
CRITICAL_SECTION critical_section_obj;
HANDLE event_handle;
SYNCHRONIZATION_BARRIER barrier_obj;

DWORD WINAPI no_synchronization(LPVOID param);
DWORD WINAPI mutex(LPVOID param);
DWORD WINAPI semaphore(LPVOID param);
DWORD WINAPI critical_section(LPVOID param);
DWORD WINAPI events(LPVOID param);
DWORD WINAPI barrier(LPVOID param);
DWORD WINAPI condition_variable();

int main() {
	int nr_of_threads = 10;

	printf("No synchronization:\n");
	run_threads(nr_of_threads, no_synchronization);

	mutex_obj = CreateMutex(NULL, FALSE, NULL);
	printf("\nMutex:\n");
	run_threads(nr_of_threads, mutex);
	CloseHandle(mutex_obj);

	semaphore_handle = CreateSemaphore(NULL, 1, 1, NULL);
	printf("\nSemaphore:\n");
	run_threads(nr_of_threads, semaphore);
	CloseHandle(semaphore_handle);

	InitializeCriticalSection(&critical_section_obj);
	printf("\nCritical section:\n");
	run_threads(nr_of_threads, critical_section);
	DeleteCriticalSection(&critical_section_obj);

	event_handle = CreateEvent(NULL, FALSE, TRUE, NULL);
	printf("\nEvent:\n");
	run_threads(nr_of_threads, events);
	CloseHandle(event_handle);

	InitializeSynchronizationBarrier(&barrier_obj, nr_of_threads, -1);
	printf("\nBarrier:\n");
	run_threads(nr_of_threads, barrier);
	DeleteSynchronizationBarrier(&barrier_obj);

	printf("\nCondition variable:\n");
	condition_variable();

    return 0;
}