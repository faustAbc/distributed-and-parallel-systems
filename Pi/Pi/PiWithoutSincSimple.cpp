#include <windows.h>
#include <process.h>
#include <stdio.h>

#define p 2

double pi[p];
int n = 10000000;

DWORD WINAPI ThreadFunction(LPVOID pvParam)
{
	int nParam = (int)pvParam;
	int i, start;
	double h, sum, x;
	h = 1. / n;
	sum = 0.;
	start = nParam;

	for (i = start; i < n; i += p)
	{
		x = h * i;
		sum += 4. / (1. + x * x);
	}

	pi[nParam] = h * sum;
	return 0;
}

int main()
{
	HANDLE hThreads[p];
	int k;
	double sum;

	LARGE_INTEGER liFrequency, liStartTime, liFinishTime;
	QueryPerformanceCounter(&liStartTime);

	printf("Pi Without Sync Simple\n");

	for (k = 0; k < p; ++k)
	{
		hThreads[k] = (HANDLE)_beginthreadex(NULL, 0,
			(_beginthreadex_proc_type)ThreadFunction, (LPVOID)k, 0, NULL);
		if (hThreads[k] == NULL)
		{
			printf("Create Thread %d Error=%d\n", k, GetLastError());
			return -1;
		}
	}

	WaitForMultipleObjects(p, hThreads, TRUE, INFINITE);
	for (k = 0; k < p; ++k)
		CloseHandle(hThreads[k]);
	sum = 0.;
	for (k = 0; k < p; ++k)
		sum += pi[k];

	QueryPerformanceCounter(&liFinishTime);
	printf("PI = %.16f\n", sum);
	printf("TIME = %.16f ms\n", dElapsedTime);

	return 0;
}
