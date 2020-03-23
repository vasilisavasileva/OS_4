//#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#include<Windows.h>
#include<tchar.h>
#define  FILE "Local\\FILE"
#define EV1 "Local\\EV1"
#define EV2 "Local\\EV2"

int _tmain(int argc, _TCHAR* argv[]) {
	HANDLE mp = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, FILE);
	double* symb = (double*)MapViewOfFile(mp, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(double));
	HANDLE EV_1 = OpenEvent(EVENT_ALL_ACCESS, FALSE, EV1);
	HANDLE EV_2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, EV2);
	double l;
	int n;
	WaitForSingleObject(EV_2, INFINITE);
	n = *symb;
	ResetEvent(EV_2);
	SetEvent(EV_1);
	for (int i = 0;i < n;i++) {
		WaitForSingleObject(EV_2, INFINITE);
		l = *symb;
		l = sqrt(l);
		*symb = l;
		ResetEvent(EV_2);
		SetEvent(EV_1);
	}
	CloseHandle(EV_1);
	CloseHandle(EV_2);
	CloseHandle(mp);
	return 0;
}