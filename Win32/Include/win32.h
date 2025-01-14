#ifndef MY_WIN32_H
#define MY_WIN32_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <string>

struct WindowInfo {
	HWND			Handle;
	std::wstring	Title;
	unsigned long	ProcessID;
};

void PrintWindowsInfo(bool printChildren = false);
void PrintWindowsInfoByProcessID(int processID);

void PrintWindowInfo(WindowInfo const&);
void PrintWindowInfo(HWND);


WindowInfo FindWindowByProcessID(unsigned long process_id);
WindowInfo FindWindowByTitle(std::wstring const& title);

struct handle_data {
	unsigned long process_id;
	HWND window_handle;
};

#endif