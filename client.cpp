#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define  FILE "Local\\FILE"
#define EV1 "Local\\EV1"
#define EV2 "Local\\EV2"
#include<Windows.h>
#include<tchar.h>
#include<stdbool.h>


int _tmain(int argc, _TCHAR* argv[]) {
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	HANDLE mp = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0, sizeof(double), FILE);
	HANDLE EVENT1 = CreateEvent(NULL, TRUE, TRUE, EV1);
	HANDLE EVENT2 = CreateEvent(NULL, TRUE, FALSE, EV2);
	TCHAR SecondProcess[] = "OS4_server";

	PROCESS_INFORMATION ProcessInfo;
	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));

	STARTUPINFO StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.dwFlags = STARTF_USESTDHANDLES;

	BOOL process = CreateProcess(NULL,
		SecondProcess,
		NULL, NULL, TRUE,
		CREATE_NEW_CONSOLE,
		NULL, NULL,
		&StartupInfo,
		&ProcessInfo);

	double l;
	BOOL isSuccess;
	double* symb = (double*)MapViewOfFile(mp, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(double));


	if (process == 1)
		printf("process true\n");
	else {
		printf("error\n");
		exit(1);
	}
	printf("Enter the count of number: ");
	int n;
	scanf("%d", &n);
	*symb = n;
	ResetEvent(EVENT1);
	SetEvent(EVENT2);
	WaitForSingleObject(EVENT1, INFINITE);
	for (int i = 0;i < n;i++) {
		scanf("%lf", &l);
		*symb = l;
		ResetEvent(EVENT1);
		SetEvent(EVENT2);
		WaitForSingleObject(EVENT1, INFINITE);
		l = *symb;
		printf("res: %f\n", l);
	}

	CloseHandle(ProcessInfo.hThread);
	CloseHandle(ProcessInfo.hProcess);
	CloseHandle(EVENT1);
	CloseHandle(EVENT2);
	CloseHandle(mp);
	system("pause");
	return 0;
}